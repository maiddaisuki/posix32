/**
 * Copyright 2025-2026 Kirill Makurin
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-internal.h"
#endif

#include <assert.h>
#include <errno.h>
#include <locale.h>
#include <mbctype.h> /* _setmbcp */
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "crt-internal.h"
#include "ctype-internal.h"
#include "locale-internal.h"
#include "string-internal.h"
#include "uchar-internal.h"
#include "wchar-internal.h"

/**
 * File Summary:
 *
 * This file implements the following functions declared in locale.h:
 *
 * - setlocale
 * - newlocale
 * - duplocale
 * - freelocale
 * - uselocale
 * - getlocalename_l
 *
 * NOTE: all references to `TLS` in this file mean `Thread Local Storage` and
 * not `Thread Locale State`.
 */

/**
 * Set when creating `locale_t` object to be used for Global Locale.
 */
#define NEWLOCALE_GLOBAL (1)

/**
 * Set when initializing from CRT's locale.
 */
#define NEWLOCALE_INIT (1 << 1)

/**
 * Use user's default locale instead of quering LC_* and LANG environment
 * variables.
 */
#define NEWLOCALE_USER_DEFAULT (1 << 2)

/**
 * Create new `locale_t` object.
 */
static locale_t P32NewLocale (int mask, const wchar_t *localeString, locale_t base, uintptr_t heap, int flags);

/**
 * Create copy of `locale_t` object `locale`.
 */
static locale_t P32DupLocale (locale_t locale, uintptr_t heap);

/**
 * Free `locale_t` object `locale`.
 */
static void P32FreeLocale (locale_t locale, uintptr_t heap);

/*******************************************************************************
 * Locale Objects
 */

/**
 * This value is stored in `locale_t` object to verify that it is a valid
 * locale object which was created by `newlocale`,
 */
#define P32_LOCALE_MAGIC 0x010CA1EF

/**
 * Verify that `locale` is a valid `locale_t` object .
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32IsValidLocale (locale_t locale) {
  /**
   * `LC_GLOBAL_LOCALE` is not a valid `locale_t` object.
   */
  if (locale == NULL || locale == LC_GLOBAL_LOCALE) {
    return false;
  }

  _ASSERT_EXPR (
    locale->Magic == P32_LOCALE_MAGIC && locale->Mask == LC_ALL_MASK,
    L"Attempt to use `locale_t` object which was not created by uselocale.\n"
  );

  return locale->Magic == P32_LOCALE_MAGIC && locale->Mask == LC_ALL_MASK;
}

/*******************************************************************************
 * Thread Locale State
 *
 * Starting with msvcr80.dll, CRT natively supports thread locales.
 *
 * A thread can switch to using thread locale by calling
 *
 *  _configthreadlocale (_ENABLE_PER_THREAD_LOCALE)
 *
 * and can switch back to using global locale by calling
 *
 *  _configthreadlocale (_DISABLE_PER_THREAD_LOCALE)
 *
 * Unfortunately, Microsoft did not provide a separate function to set thread's
 * locale, instead, standard `[_w]setlocale` is used for this purpose.
 *
 * This means that:
 *
 * - calling `setlocale` to set global locale may modify current thread's
 *   locale instead of global locale
 *
 * - calling `setlocale (LC_*, NULL)` may return locale string for current
 *   thread's locale instead of global locale
 *
 * For older CRTs (msvcr71.dll and older, including msvcrt.dll), which do not
 * support thread locales natively, we emulate them.
 */

/**
 * Structure to keep track of thread locale state.
 */
typedef struct ThreadLocaleState {
  int PreviousState;
  int CurrentState;
} ThreadLocaleState;

#if P32_CRT >= P32_MSVCR80
/**
 * Get current thread locale state and store it in `threadLocaleState`.
 */
static void P32GetThreadLocaleState (ThreadLocaleState *threadLocaleState) {
  int currentState = _configthreadlocale (0);

  assert (currentState == _ENABLE_PER_THREAD_LOCALE || currentState == _DISABLE_PER_THREAD_LOCALE);

  if (currentState == -1) {
    p32_terminate (L"Failed to query thread locale state.");
  }

  threadLocaleState->PreviousState = currentState;
  threadLocaleState->CurrentState  = currentState;
}

/**
 * Set thread locale state to `state`.
 *
 * Store previous thread locale state in `threadLocaleState`, if not `NULL`.
 *
 * Return `true` on success, and `false` otherwise.
 */
static bool P32SetThreadLocaleState (int state, ThreadLocaleState *threadLocaleState) {
  int oldState = _configthreadlocale (state);

  if (oldState == -1) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"Call to _configthreadlocale(%X) has failed.\n", state);
#endif

    return false;
  }

  if (threadLocaleState != NULL) {
    threadLocaleState->PreviousState = oldState;
    threadLocaleState->CurrentState  = state;
  }

  return true;
}

/**
 * Restore thread locale state from `threadLocaleState`.
 *
 * Return `true` on success, and `false` otherwise.
 */
static bool P32RestoreThreadLocaleState (ThreadLocaleState *threadLocaleState) {
  /**
   * No changes required.
   */
  if (threadLocaleState->CurrentState == threadLocaleState->PreviousState) {
    return true;
  }

  int oldState = _configthreadlocale (threadLocaleState->PreviousState);

  if (oldState == -1) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"Call to _configthreadlocale(%X) has failed.\n", threadLocaleState->PreviousState);
#endif

    return false;
  }

  threadLocaleState->CurrentState  = threadLocaleState->PreviousState;
  threadLocaleState->PreviousState = oldState;

  return true;
}
#endif

/*******************************************************************************
 * Default and Global Locale
 *
 * Default Locale is "C" ("POSIX") locale.
 *
 * Global Locale is one set by `setlocale`. This is the active locale for
 * threads which did not call `uselocale` or set thread locale to global locale
 * by calling `uselocale (LC_GLOBAL_LOCALE)`.
 */

/**
 * Destroy Global Locale.
 */
static int __cdecl P32DestroyGlobalLocale (void);

/**
 * Structure to store Default and Global Locale.
 */
typedef struct {
  pthread_once_t DefaultInit;
  pthread_once_t GlobalInit;
  /**
   * Synchronization for access to Global Locale:
   *
   * - `setlocale` acquires write lock
   * - `duplocale (LC_GLOBAL_LOCALE)` acquires read lock
   */
  pthread_rwlock_t GlobalLock;
  /**
   * Private heap.
   */
  uintptr_t Heap;
  /**
   * `locale_t` object representing Default Locale
   */
  locale_t DefaultLocale;
  /**
   * `locale_t` object representing Global Locale
   */
  locale_t GlobalLocale;
} GlobalLocale;

/**
 * Default and Global locale.
 */
static GlobalLocale P32GlobalLocale = {
  .DefaultInit   = PTHREAD_ONCE_INIT,
  .GlobalInit    = PTHREAD_ONCE_INIT,
  .GlobalLock    = PTHREAD_RWLOCK_INITIALIZER,
  .Heap          = 0,
  .DefaultLocale = NULL,
  .GlobalLocale  = NULL
};

/**
 * Initialize Default Locale.
 */
static void P32InitDefaultLocale (void) {
  /**
   * Create private heap used by Default and Global Locale.
   */
  HANDLE heapHandle = HeapCreate (HEAP_GENERATE_EXCEPTIONS, 4096, 0);

  if (heapHandle == NULL) {
    p32_terminate (L"Global locale: failed to create private heap.");
  }

  /**
   * Request Low-Fragmentation Heap.
   */
  ULONG lowFragmentationHeap = 2;

  HeapSetInformation (heapHandle, HeapCompatibilityInformation, &lowFragmentationHeap, sizeof (ULONG));

  /**
   * Request termination if heap corruption has occured.
   */
  HeapSetInformation (heapHandle, HeapEnableTerminationOnCorruption, NULL, 0);

  P32GlobalLocale.Heap          = (uintptr_t) heapHandle;
  P32GlobalLocale.DefaultLocale = P32NewLocale (LC_ALL_MASK, L"C", NULL, P32GlobalLocale.Heap, 0);

  if (P32GlobalLocale.DefaultLocale == NULL) {
    p32_terminate (L"Global locale: initialization has failed.");
  }

#ifndef LIBPOSIX32_DLL
  _onexit (P32DestroyGlobalLocale);
#endif
}

/**
 * Destroy Default Locale.
 */
static void P32DestroyDefaultLocale (void) {
  if (P32GlobalLocale.Heap != 0) {
    uintptr_t heap       = P32GlobalLocale.Heap;
    HANDLE    heapHandle = (HANDLE) heap;

    P32GlobalLocale.Heap = 0;

    if (P32GlobalLocale.DefaultLocale != NULL) {
      P32FreeLocale (P32GlobalLocale.DefaultLocale, heap);
      P32GlobalLocale.DefaultLocale = NULL;
    }

#if defined(LIBPOSIX32_TEST) && defined(_DEBUG)
    HEAP_SUMMARY heapSummary = {0};
    heapSummary.cb           = sizeof (heapSummary);

    if (HeapSummary (heapHandle, 0, &heapSummary)) {
      _RPTW1 (_CRT_WARN, L"Global locale: heap <%p> is about to be destroyed.\n", heapHandle);
      _RPTW1 (_CRT_WARN, L"  cbAllocated=%zu\n", heapSummary.cbAllocated);
      _RPTW1 (_CRT_WARN, L"  cbCommitted=%zu\n", heapSummary.cbCommitted);
      _RPTW1 (_CRT_WARN, L"  cbMaxReserve=%zu\n", heapSummary.cbMaxReserve);
      _RPTW1 (_CRT_WARN, L"  cbReserved=%zu\n", heapSummary.cbReserved);
    }
#endif

    if (!HeapDestroy (heapHandle)) {
      p32_terminate (L"Global locale: failed destroy private heap.");
    }
  }
}

locale_t p32_default_locale (void) {
  pthread_once (&P32GlobalLocale.DefaultInit, P32InitDefaultLocale);
  return P32GlobalLocale.DefaultLocale;
}

/**
 * Initialize Global Locale.
 */
static void P32InitGlobalLocale (void) {
  pthread_once (&P32GlobalLocale.DefaultInit, P32InitDefaultLocale);

  assert (P32GlobalLocale.Heap != 0);
  HANDLE heapHandle = (HANDLE) P32GlobalLocale.Heap;

  if (pthread_rwlock_init (&P32GlobalLocale.GlobalLock, NULL) != 0) {
    p32_terminate (L"Global locale: failed to initialize pthread_rwlock_t object.");
  }

#if P32_CRT >= P32_MSVCR80
  /**
   * CRT's thread locale.
   */
  wchar_t *threadLocale = NULL;

  /**
   * Thread locale state.
   */
  ThreadLocaleState state = {0};

  P32GetThreadLocaleState (&state);

  /**
   * We're initialzing Global Locale from a thread which uses thread locale.
   *
   * We need to change thread locale state to `_DISABLE_PER_THREAD_LOCALE`,
   * and then restore both thread locale state and CRT's thread locale.
   *
   * This is required since call to `_wsetlocale (LC_ALL, NULL)` will change
   * thread locale to global locale.
   */
  if (state.CurrentState == _ENABLE_PER_THREAD_LOCALE) {
    if (p32_private_wcsdup (&threadLocale, _wsetlocale (LC_ALL, NULL), P32GlobalLocale.Heap) == -1) {
      p32_terminate (L"Out of memory.");
    }

    if (!P32SetThreadLocaleState (_DISABLE_PER_THREAD_LOCALE, &state)) {
      p32_terminate (L"Global Locale: failed to change thread locale state.");
    }
  }
#endif

  /**
   * CRT's global locale.
   */
  wchar_t *globalLocale = NULL;

#if P32_CRT >= P32_MSVCRT20
  const wchar_t *crtGlobalLocale = _wsetlocale (LC_ALL, NULL);

  if (p32_private_wcsdup (&globalLocale, crtGlobalLocale, P32GlobalLocale.Heap) == -1) {
    p32_terminate (L"Out of memory.");
  }
#else
  uint32_t codePage = GetACP ();

  /**
   * crtdll.dll and msvcrt10.dll do not have `_wsetlocale` function.
   *
   * This means that we have to convert string returned by `setlocale` to
   * wide character string. This may fail horribly...
   */
  const char *crtGlobalLocale = setlocale (LC_ALL, NULL);

  if (p32_private_mbstowcs (&globalLocale, crtGlobalLocale, P32GlobalLocale.Heap, codePage) == -1) {
    p32_terminate (L"Global Locale: initialization has failed.");
  }
#endif

#if defined(LIBPOSIX32_TEST)
  _RPTW1 (_CRT_WARN, L"Initializing Global Locale from \"%s\".\n", globalLocale);
#endif

  /**
   * `P32NewLocale` with `LC_ALL_MASK` requires that second argument is either
   * single locale string or a list containing locale string for each category.
   *
   * Since CRT does not support `LC_MESSAGES`, this may result in spurious
   * failures if some of CRT's global locale's categories are set to different
   * locales.
   */
  int mask = LC_ALL_MASK;

  if (wcschr (globalLocale, L';') != NULL) {
    mask &= ~LC_MESSAGES_MASK;
  }

  P32GlobalLocale.GlobalLocale = P32NewLocale (
    mask, globalLocale, P32GlobalLocale.DefaultLocale, P32GlobalLocale.Heap, NEWLOCALE_GLOBAL | NEWLOCALE_INIT
  );

  if (P32GlobalLocale.GlobalLocale == NULL) {
    p32_terminate (L"Global Locale: initialization has failed.");
  }

  HeapFree (heapHandle, 0, globalLocale);

#if P32_CRT >= P32_MSVCR80
  if (!P32RestoreThreadLocaleState (&state)) {
    p32_terminate (L"Global Locale: failed to restore thread locale state.");
  }

  /**
   * If previous thread locale state was `_ENABLE_PER_THREAD_LOCALE`,
   * restore previous thread locale.
   */
  if (state.CurrentState == _ENABLE_PER_THREAD_LOCALE) {
    assert (threadLocale != NULL);

    if (_wsetlocale (LC_ALL, threadLocale) == NULL) {
      p32_terminate (L"Thread Locale: failed to restore previous locale.");
    }

    HeapFree (heapHandle, 0, threadLocale);
  }
#endif
}

static int P32DestroyGlobalLocale (void) {
  if (P32GlobalLocale.GlobalLocale != NULL) {
    P32FreeLocale (P32GlobalLocale.GlobalLocale, P32GlobalLocale.Heap);
    P32GlobalLocale.GlobalLocale = NULL;
  }

  if (pthread_rwlock_destroy (&P32GlobalLocale.GlobalLock) != 0) {
    p32_terminate (L"Global locale: failed to destroy pthread_rwlock_t object.");
  }

  P32DestroyDefaultLocale ();

  return 0;
}

#ifdef LIBPOSIX32_DLL
void p32_destroy_global_locale (void) {
  P32DestroyGlobalLocale ();
}
#endif

locale_t p32_global_locale (void) {
#ifndef LIBPOSIX32_TEST
  pthread_once (&P32GlobalLocale.GlobalInit, P32InitGlobalLocale);
#endif
  return P32GlobalLocale.GlobalLocale;
}

/*******************************************************************************
 * Thread Locale
 */

/**
 * Structure to store Thread Locale.
 */
struct ThreadLocale {
  /**
   * `locale_t` object representing current thread's locale.
   *
   * If this fields is set to `LC_GLOBAL_LOCALE`, then current thread is using
   * Global Locale.
   */
  locale_t Locale;
  /**
   * User-supplied `locale_t` object to `uselocale`.
   * This object is returned to the caller when `uselocale` succeeds.
   *
   * This object is initialized to `LC_GLOBAL_LOCALE` and can be set to this
   * value by calling `uselocale (LC_GLOBAL_LOCALE)`.
   */
  locale_t Handle;
};

/**
 * Initialize Thread Locale structure in TLS.
 */
static bool P32InitThreadLocale (ThreadStorage *tls) {
  HANDLE heapHandle = (HANDLE) tls->Heap;

  /**
   * Allocate structure for Thread Locale.
   */
  ThreadLocale *threadLocale = (ThreadLocale *) HeapAlloc (heapHandle, 0, sizeof (ThreadLocale));

  if (threadLocale == NULL) {
    _set_errno (ENOMEM);
    return false;
  }

  threadLocale->Handle = LC_GLOBAL_LOCALE;
  threadLocale->Locale = LC_GLOBAL_LOCALE;

  tls->ThreadLocale = threadLocale;

  return true;
}

#if P32_CRT >= P32_MSVCR80
/**
 * Initialize Thread Locale from CRT's thread locale.
 */
static P32_NOINLINE void P32InitThreadLocaleUnsafe (ThreadStorage *tls) {
  pthread_once (&P32GlobalLocale.DefaultInit, P32InitDefaultLocale);

  HANDLE heapHandle = (HANDLE) tls->Heap;

  /**
   * Allocate structure for Thread Locale.
   */
  ThreadLocale *threadLocale = (ThreadLocale *) HeapAlloc (heapHandle, 0, sizeof (ThreadLocale));

  if (threadLocale == NULL) {
    p32_terminate (L"TLS: failed to allocate storage for thread locale.");
  }

  threadLocale->Handle = LC_GLOBAL_LOCALE;

  /**
   * No need to `wcsdup`, returned string is current thread's locale.
   */
  const wchar_t *localeString = _wsetlocale (LC_ALL, NULL);

#ifdef LIBPOSIX32_TEST
  _RPTW1 (_CRT_WARN, L"Initializing Thread Locale from \"%s\"\n", localeString);
#endif

  /**
   * See comment regarding `LC_MESSAGES` in `P32InitGlobalLocale`.
   */
  int mask = LC_ALL_MASK;

  if (wcschr (localeString, L';') != NULL) {
    mask &= ~LC_MESSAGES_MASK;
  }

  threadLocale->Locale = P32NewLocale (mask, localeString, P32GlobalLocale.DefaultLocale, tls->Heap, NEWLOCALE_INIT);

  if (threadLocale->Locale == NULL) {
    p32_terminate (L"Thread Locale: initialization has failed.");
  }

  tls->ThreadLocale = threadLocale;
}
#endif

/**
 * Destroy Thread Locale stored in `tls`.
 */
static void P32DestroyThreadLocale (ThreadStorage *tls) {
  HANDLE heapHandle = (HANDLE) tls->Heap;

  /**
   * No need to free Thread Locale.
   */
  if (tls->ThreadLocale == NULL) {
    return;
  }

  if (tls->ThreadLocale->Locale != LC_GLOBAL_LOCALE) {
    assert (tls->ThreadLocale->Locale != NULL);
    P32FreeLocale (tls->ThreadLocale->Locale, tls->Heap);
  }

  if (!HeapFree (heapHandle, 0, tls->ThreadLocale)) {
    p32_terminate (L"TLS: failed to deallocate storage for thread locale.");
  }

  tls->ThreadLocale = NULL;
}

#ifdef LIBPOSIX32_DLL
void p32_destroy_thread_locale (ThreadStorage *tls) {
  P32DestroyThreadLocale (tls);
}
#endif

/**
 * Return `locale_t` object for active Thread Locale.
 *
 * If `useThreadLocaleState` is `true`, this functions takes CRT's
 * thread locale state into account.
 *
 * This function returns `LC_GLOBAL_LOCALE` if current thread is using
 * Global Locale.
 */
static locale_t P32GetThreadLocale (bool useThreadLocaleState) {
#if P32_CRT >= P32_MSVCR80
  ThreadLocaleState state = {0};

  P32GetThreadLocaleState (&state);

  bool usingGlobalLocale = (state.CurrentState == _DISABLE_PER_THREAD_LOCALE);

  /**
   * Return `LC_GLOBAL_LOCALE` to inidicate that thread is using Global Locale.
   */
  if (useThreadLocaleState && usingGlobalLocale) {
    return LC_GLOBAL_LOCALE;
  }

  ThreadStorage *tls = p32_tls (usingGlobalLocale);
#else
  ThreadStorage *tls = p32_tls (true);
#endif

  /**
   * Current thread's TLS was not initialized.
   */
  if (tls == NULL) {
    return LC_GLOBAL_LOCALE;
  }

  /**
   * If `tls->ThreadLocale == NULL`, then `uselocale` was not called from the
   * current thread.
   */
  if (tls->ThreadLocale == NULL) {
#if P32_CRT >= P32_MSVCR80
    /**
     * Thread locale was set by CRT's `setlocale`.
     * Try to initialized Thread Locale from CRT's thread locale.
     */
    if (!usingGlobalLocale) {
      P32InitThreadLocaleUnsafe (tls);
      return tls->ThreadLocale->Locale;
    }
#endif

    return LC_GLOBAL_LOCALE;
  }

  return tls->ThreadLocale->Locale;
  UNREFERENCED_PARAMETER (useThreadLocaleState);
}

/*******************************************************************************
 * Active Locale
 */

/**
 * Get current thread's active locale.
 *
 * If `useThreadLocaleState` is `true`, take CRT's thread locale state into
 * account.
 *
 * This function may return `NULL` if neither Global nor Thread Locale was set.
 */
static locale_t P32GetActiveLocale (bool useThreadLocaleState) {
  locale_t activeLocale = P32GetThreadLocale (useThreadLocaleState);

  if (activeLocale == LC_GLOBAL_LOCALE) {
    return p32_global_locale ();
  }

  return activeLocale;
}

locale_t p32_active_locale (void) {
  return P32GetActiveLocale (true);
}

/*******************************************************************************
 * Internal Functions
 */

/**
 * For each locale category in `mask`, construct `Locale` object using
 * single locale string `localeStrings.W.LcAll`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32ResolveLocaleSingleString (locale_t locale, uintptr_t heap, LocaleStrings *localeStrings, int mask) {
  bool success = false;

  Locale    resolvedLocale = {0};
  LocaleMap localeMap      = {0};

  if (!p32_locale_map (&localeMap, localeStrings->W.LcAll, heap)) {
    goto fail;
  }

  if (!p32_winlocale_resolve (&resolvedLocale, heap, &localeMap)) {
    goto fail;
  }

  if (mask & LC_COLLATE_MASK) {
    if (!p32_winlocale_copy (&locale->WinLocale.LcCollate, heap, &resolvedLocale)) {
      goto fail_destroy;
    }

    locale->Mask |= LC_COLLATE_MASK;
  }

  if (mask & LC_CTYPE_MASK) {
    if (!p32_winlocale_copy (&locale->WinLocale.LcCtype, heap, &resolvedLocale)) {
      goto fail_destroy;
    }

    locale->Mask |= LC_CTYPE_MASK;
  }

  if (mask & LC_MESSAGES_MASK) {
    if (!p32_winlocale_copy (&locale->WinLocale.LcMessages, heap, &resolvedLocale)) {
      goto fail_destroy;
    }

    locale->Mask |= LC_MESSAGES_MASK;
  }

  if (mask & LC_MONETARY_MASK) {
    if (!p32_winlocale_copy (&locale->WinLocale.LcMonetary, heap, &resolvedLocale)) {
      goto fail_destroy;
    }

    locale->Mask |= LC_MONETARY_MASK;
  }

  if (mask & LC_NUMERIC_MASK) {
    if (!p32_winlocale_copy (&locale->WinLocale.LcNumeric, heap, &resolvedLocale)) {
      goto fail_destroy;
    }

    locale->Mask |= LC_NUMERIC_MASK;
  }

  if (mask & LC_TIME_MASK) {
    if (!p32_winlocale_copy (&locale->WinLocale.LcTime, heap, &resolvedLocale)) {
      goto fail_destroy;
    }

    locale->Mask |= LC_TIME_MASK;
  }

  if (mask & LC_CTYPE_MASK) {
    locale->Charset.CodePage = localeMap.CodePage;
  }

  success = true;

fail_destroy:
  p32_winlocale_destroy (&resolvedLocale, heap);

fail:
  return success;
}

/**
 * For each locale category in `mask`, construct `Locale` object using
 * corresponding locale string in `localeStrings`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32ResolveLocale (locale_t locale, uintptr_t heap, LocaleStrings *localeStrings, int mask) {
  bool success = false;

  /**
   * Code page requested for LC_CTYPE locale category.
   */
  uint32_t requestedCodePage = 0;

  if ((mask & LC_COLLATE_MASK)) {
    LocaleMap localeMap = {0};

    assert (localeStrings->Mask & LC_COLLATE_MASK && localeStrings->W.LcCollate != NULL);

    if (!p32_locale_map (&localeMap, localeStrings->W.LcCollate, heap)) {
      goto fail;
    }

    if (!p32_winlocale_resolve (&locale->WinLocale.LcCollate, heap, &localeMap)) {
      goto fail;
    }

    locale->Mask |= LC_COLLATE_MASK;
  }

  if ((mask & LC_CTYPE_MASK)) {
    LocaleMap localeMap = {0};

    assert (localeStrings->Mask & LC_CTYPE_MASK && localeStrings->W.LcCtype != NULL);

    if (!p32_locale_map (&localeMap, localeStrings->W.LcCtype, heap)) {
      goto fail;
    }

    requestedCodePage = localeMap.CodePage;

    if (!p32_winlocale_resolve (&locale->WinLocale.LcCtype, heap, &localeMap)) {
      goto fail;
    }

    locale->Mask |= LC_CTYPE_MASK;
  }

  if ((mask & LC_MESSAGES_MASK)) {
    LocaleMap localeMap = {0};

    assert (localeStrings->Mask & LC_MESSAGES_MASK && localeStrings->W.LcMessages != NULL);

    if (!p32_locale_map (&localeMap, localeStrings->W.LcMessages, heap)) {
      goto fail;
    }

    if (!p32_winlocale_resolve (&locale->WinLocale.LcMessages, heap, &localeMap)) {
      goto fail;
    }

    locale->Mask |= LC_MESSAGES_MASK;
  }

  if ((mask & LC_MONETARY_MASK)) {
    LocaleMap localeMap = {0};

    assert (localeStrings->Mask & LC_MONETARY_MASK && localeStrings->W.LcMonetary != NULL);

    if (!p32_locale_map (&localeMap, localeStrings->W.LcMonetary, heap)) {
      goto fail;
    }

    if (!p32_winlocale_resolve (&locale->WinLocale.LcMonetary, heap, &localeMap)) {
      goto fail;
    }

    locale->Mask |= LC_MONETARY_MASK;
  }

  if ((mask & LC_NUMERIC_MASK)) {
    LocaleMap localeMap = {0};

    assert (localeStrings->Mask & LC_NUMERIC_MASK && localeStrings->W.LcNumeric != NULL);

    if (!p32_locale_map (&localeMap, localeStrings->W.LcNumeric, heap)) {
      goto fail;
    }

    if (!p32_winlocale_resolve (&locale->WinLocale.LcNumeric, heap, &localeMap)) {
      goto fail;
    }

    locale->Mask |= LC_NUMERIC_MASK;
  }

  if ((mask & LC_TIME_MASK)) {
    LocaleMap localeMap = {0};

    assert (localeStrings->Mask & LC_TIME_MASK && localeStrings->W.LcTime != NULL);

    if (!p32_locale_map (&localeMap, localeStrings->W.LcTime, heap)) {
      goto fail;
    }

    if (!p32_winlocale_resolve (&locale->WinLocale.LcTime, heap, &localeMap)) {
      goto fail;
    }

    locale->Mask |= LC_TIME_MASK;
  }

  if (mask & LC_CTYPE_MASK) {
    locale->Charset.CodePage = requestedCodePage;
  }

  success = true;

fail:
  return success;
}

/**
 * For each locale category in `mask`, copy corresponding `Locale` object
 * from `base`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32CopyLocale (locale_t locale, uintptr_t heap, locale_t base, int mask) {
  bool success = false;

  if ((mask & LC_COLLATE_MASK)) {
    assert ((locale->Mask & LC_COLLATE_MASK) == 0);

    if (!p32_winlocale_copy (&locale->WinLocale.LcCollate, heap, &base->WinLocale.LcCollate)) {
      goto fail;
    }

    locale->Mask |= LC_COLLATE_MASK;
  }

  if ((mask & LC_CTYPE_MASK)) {
    assert ((locale->Mask & LC_CTYPE_MASK) == 0);

    if (!p32_winlocale_copy (&locale->WinLocale.LcCtype, heap, &base->WinLocale.LcCtype)) {
      goto fail;
    }

    locale->Mask |= LC_CTYPE_MASK;
  }

  if ((mask & LC_MESSAGES_MASK)) {
    assert ((locale->Mask & LC_MESSAGES_MASK) == 0);

    if (!p32_winlocale_copy (&locale->WinLocale.LcMessages, heap, &base->WinLocale.LcMessages)) {
      goto fail;
    }

    locale->Mask |= LC_MESSAGES_MASK;
  }

  if ((mask & LC_MONETARY_MASK)) {
    assert ((locale->Mask & LC_MONETARY_MASK) == 0);

    if (!p32_winlocale_copy (&locale->WinLocale.LcMonetary, heap, &base->WinLocale.LcMonetary)) {
      goto fail;
    }

    locale->Mask |= LC_MONETARY_MASK;
  }

  if ((mask & LC_NUMERIC_MASK)) {
    assert ((locale->Mask & LC_NUMERIC_MASK) == 0);

    if (!p32_winlocale_copy (&locale->WinLocale.LcNumeric, heap, &base->WinLocale.LcNumeric)) {
      goto fail;
    }

    locale->Mask |= LC_NUMERIC_MASK;
  }

  if ((mask & LC_TIME_MASK)) {
    assert ((locale->Mask & LC_TIME_MASK) == 0);

    if (!p32_winlocale_copy (&locale->WinLocale.LcTime, heap, &base->WinLocale.LcTime)) {
      goto fail;
    }

    locale->Mask |= LC_TIME_MASK;
  }

  if (mask & LC_CTYPE_MASK) {
    locale->Charset.CodePage = base->Charset.CodePage;
  }

  success = true;

fail:
  return success;
}

/**
 * Gather information about requested code page and fill in `locale->Charset`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32LocaleCharset (locale_t locale, int flags) {
  /**
   * Code page requested for LC_CTYPE category.
   *
   * Value of zero means that code page was not explicitly requested.
   */
  uint32_t requestedCodePage = locale->Charset.CodePage;

  /**
   * Active ANSI code page.
   */
  uint32_t activeCodePage = GetACP ();

  /**
   * Code page to use.
   */
  uint32_t codePage = 0;

  /**
   * Whether locale requires unicode.
   */
  bool localeRequiresUnicode = false;

  /**
   * `GetLocaleInfo[Ex]` returns ANSI code page as 0 (CP_ACP) and
   * OEM code page as 1 (CP_OEMCP) for locales which require unicode.
   */
  if (locale->WinLocale.LcCtype.CodePage.Ansi == CP_ACP || locale->WinLocale.LcCtype.CodePage.Oem == CP_OEMCP) {
    localeRequiresUnicode = true;
  }

  /**
   * If caller requested locale's default ANSI or OEM code page, we need to
   * choose actual code page to use.
   *
   * If locale does not require unicode, we use locale's default ANSI or OEM
   * code page as returned by `GetLocaleInfo[Ex]`.
   *
   * If locale requires unicode, use code page 65001 (CP_UTF8) only if active
   * ANSI code page is also 65001. Otherwise, fail.
   *
   * Note that in case when locale requires unicode, CRT will always use
   * active ANSI code page, which may turn out to be 65001. This behavior is
   * inconsistent and our implementation does not follow it.
   */

  /**
   * Caller requested locale's default ANSI code page.
   */
  if (requestedCodePage == P32_CODEPAGE_ACP) {
    if (localeRequiresUnicode && activeCodePage == CP_UTF8) {
      /**
       * When testing older CRTs, act as if system cannot have code page 65001
       * set as active ANSI code page.
       */
#if !defined(LIBPOSIX32_TEST) || P32_CRT >= P32_UCRT
      requestedCodePage = CP_UTF8;
#else
      return false;
#endif
    } else if (!localeRequiresUnicode) {
      requestedCodePage = locale->WinLocale.LcCtype.CodePage.Ansi;
    } else {
      return false;
    }

    /**
     * Caller requested locale's default OEM code page.
     */
  } else if (requestedCodePage == P32_CODEPAGE_OCP) {
    if (localeRequiresUnicode && activeCodePage == CP_UTF8) {
      /**
       * When testing older CRTs, act as if system cannot have code page 65001
       * set as active ANSI code page.
       */
#if !defined(LIBPOSIX32_TEST) || P32_CRT >= P32_UCRT
      requestedCodePage = CP_UTF8;
#else
      return false;
#endif
    } else if (!localeRequiresUnicode) {
      requestedCodePage = locale->WinLocale.LcCtype.CodePage.Oem;
    } else {
      return false;
    }
  }

  /**
   * Caller did not request code page for LC_CTYPE locale category.
   *
   * This has special meaning when initializing from CRT's locale.
   */
  if (requestedCodePage == 0) {
    /**
     * This may only occur when we're initializing locale from CRT's locale
     * which uses locale names.
     *
     * In this case we must use locale's default ANSI code page. However,
     * if locale's ANSI code page is 0 (CP_ACP), we must use active ANSI code
     * page (GetACP) instead of code page 65001 (CP_UTF8).
     */
    if (flags & (NEWLOCALE_INIT)) {
      if (localeRequiresUnicode) {
        codePage = activeCodePage;
      } else {
        codePage = locale->WinLocale.LcCtype.CodePage.Ansi;
      }
      /**
       * When testing older CRTs, act as if system cannot have code page 65001
       * set as active ANSI code page.
       */
#if !defined(LIBPOSIX32_TEST) || P32_CRT >= P32_UCRT
    } else if (activeCodePage == CP_UTF8) {
      codePage = CP_UTF8;
#endif
    } else if (!localeRequiresUnicode) {
      codePage = P32_LOCALE_CODEPAGE (locale->WinLocale.LcCtype);
    } else {
      return false;
    }

    /**
     * Use requested code page.
     */
  } else {
    codePage = requestedCodePage;
  }

  assert (codePage != 0);

  /**
   * Check if `codePage` is supported and can be used.
   */
  int rejectMask = 0;

  if (flags & NEWLOCALE_GLOBAL) {
    rejectMask |= (P32_CHARSET_REJECT_CRT | P32_CHARSET_REJECT_GLOBAL);
  }

  if (!p32_charset_usable (codePage, rejectMask, 0)) {
    return false;
  }

  locale->Charset.CodePage = codePage;

  if (!p32_charset_info (&locale->Charset)) {
    return false;
  }

  return true;
}

/**
 * Format locale strings.
 */
static bool P32LocaleStrings (locale_t locale, uintptr_t heap) {
  /**
   * Format locale string using Windows Format.
   */
  locale->WindowsLocaleStrings.Format     = LOCALE_STRING_FORMAT_WINDOWS;
  locale->WindowsLocaleStrings.A.CodePage = P32_CODEPAGE_ASCII;
  locale->WindowsLocaleStrings.W.CodePage = locale->Charset.CodePage;

  if (!p32_localestr_format (&locale->WindowsLocaleStrings, heap, &locale->WinLocale)) {
    goto fail;
  }

  /**
   * Format locale string using CRT Format.
   */
  locale->CrtLocaleStrings.Format     = LOCALE_STRING_FORMAT_CRT;
  locale->CrtLocaleStrings.A.CodePage = locale->WinLocale.LcCtype.CodePage.Ansi;
  locale->CrtLocaleStrings.W.CodePage = locale->Charset.CodePage;

#if P32_CRT < P32_UCRT
  /**
   * We emulate UTF-8 locales for CRTs which do not support it.
   * In this case, we set CRT's locale to "C" locale.
   */
  if (locale->Charset.CodePage == CP_UTF8) {
    locale->CrtLocaleStrings.Format = LOCALE_STRING_FORMAT_C;
  }
#endif

  /**
   * We want to format CRT locale string using locale's default ANSI code page.
   *
   * Windows APIs and CRT use active ANSI code page (GetACP) when locale's
   * ANSI code page is returned as 0 (CP_ACP).
   */
  if (locale->CrtLocaleStrings.A.CodePage == CP_ACP) {
#if P32_CRT >= P32_UCRT
    /**
     * This string is not used with UCRT.
     */
    locale->CrtLocaleStrings.A.CodePage = CP_UTF8;
#else
    locale->CrtLocaleStrings.A.CodePage = GetACP ();
#endif
  }

  if (!p32_localestr_format (&locale->CrtLocaleStrings, heap, &locale->WinLocale)) {
    goto fail;
  }

  /**
   * Format locale string using ISO Format.
   */
  locale->IsoLocaleStrings.Format = LOCALE_STRING_FORMAT_ISO;
  /**
   * TODO: should we use locale->Charset.CodePage?
   */
  locale->IsoLocaleStrings.A.CodePage = P32_CODEPAGE_ASCII;
  locale->IsoLocaleStrings.W.CodePage = locale->Charset.CodePage;

  if (!p32_localestr_format (&locale->IsoLocaleStrings, heap, &locale->WinLocale)) {
    goto fail;
  }

  return true;

fail:
  return false;
}

#if P32_CRT >= P32_MSVCR80 && defined(LIBPOSIX32_TEST)
/**
 * Wrapper around `_[w]create_locale` to create a single `_locale_t` object.
 */
static _locale_t P32CreateLocaleObject (int category, const wchar_t *localeStringW, const char *localeStringA) {
  _locale_t locale = NULL;

  /**
   * `_wcreate_locale` is available starting with msvcr110.dll.
   */
#if P32_CRT >= P32_MSVCR110
  locale = _wcreate_locale (category, localeStringW);
#else
  locale = _create_locale (category, localeStringA);
#endif

  return locale;
  UNREFERENCED_PARAMETER (localeStringA);
  UNREFERENCED_PARAMETER (localeStringW);
}

/**
 * Create `_locale_t` object for each locale category supported by CRT.
 */
static void P32CreateLocale (locale_t locale) {
  locale->CrtLocale.LcCollate =
    P32CreateLocaleObject (LC_COLLATE, locale->CrtLocaleStrings.W.LcCollate, locale->CrtLocaleStrings.A.LcCollate);

  if (locale->CrtLocale.LcCollate == NULL) {
    _RPTW1 (_CRT_WARN, L"LC_COLLATE(%s): failed to create _locale_t object.\n", locale->CrtLocaleStrings.W.LcCollate);
  }

  locale->CrtLocale.LcCtype =
    P32CreateLocaleObject (LC_CTYPE, locale->CrtLocaleStrings.W.LcCtype, locale->CrtLocaleStrings.A.LcCtype);

  if (locale->CrtLocale.LcCtype == NULL) {
    _RPTW1 (_CRT_WARN, L"LC_CTYPE(%s): failed to create _locale_t object.\n", locale->CrtLocaleStrings.W.LcCtype);
  }

  locale->CrtLocale.LcMonetary =
    P32CreateLocaleObject (LC_MONETARY, locale->CrtLocaleStrings.W.LcMonetary, locale->CrtLocaleStrings.A.LcMonetary);

  if (locale->CrtLocale.LcMonetary == NULL) {
    _RPTW1 (_CRT_WARN, L"LC_MONETARY(%s): failed to create _locale_t object.\n", locale->CrtLocaleStrings.W.LcMonetary);
  }

  locale->CrtLocale.LcNumeric =
    P32CreateLocaleObject (LC_NUMERIC, locale->CrtLocaleStrings.W.LcNumeric, locale->CrtLocaleStrings.A.LcNumeric);

  if (locale->CrtLocale.LcNumeric == NULL) {
    _RPTW1 (_CRT_WARN, L"LC_NUMERIC(%s): failed to create _locale_t object.\n", locale->CrtLocaleStrings.W.LcNumeric);
  }

  locale->CrtLocale.LcTime =
    P32CreateLocaleObject (LC_TIME, locale->CrtLocaleStrings.W.LcTime, locale->CrtLocaleStrings.A.LcTime);

  if (locale->CrtLocale.LcTime == NULL) {
    _RPTW1 (_CRT_WARN, L"LC_TIME(%s): failed to create _locale_t object.\n", locale->CrtLocaleStrings.W.LcTime);
  }
}
#endif

/**
 * Get locale-specific functions.
 */
static void P32LocaleFunctions (locale_t locale) {
  p32_string_functions (locale);
  p32_uchar_functions (locale);
  p32_wchar_functions (locale);
  p32_wchar_c89_functions (locale);
  p32_wchar_c95_functions (locale);
}

/**
 * Get locale information.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32LocaleInfo (locale_t locale, uintptr_t heap, locale_t base, int baseMask) {
  /**
   * If `locale` and `base` use the same code page, we can copy locale
   * information for locale categories copied from `base`.
   */
  bool compatible = (base != NULL && locale->Charset.CodePage == base->Charset.CodePage);

  if (compatible && (baseMask & LC_COLLATE_MASK)) {
    if (!p32_localeinfo_collate_copy (&locale->LocaleInfo.LcCollate, heap, &base->LocaleInfo.LcCollate)) {
      goto fail_collate;
    }
  } else {
    if (!p32_localeinfo_collate (locale, heap)) {
      goto fail_collate;
    }
  }

  if (compatible && (baseMask & LC_CTYPE_MASK)) {
    if (!p32_localeinfo_ctype_copy (&locale->LocaleInfo.LcCtype, heap, &base->LocaleInfo.LcCtype)) {
      goto fail_ctype;
    }
  } else {
    if (!p32_localeinfo_ctype (locale, heap)) {
      goto fail_ctype;
    }
  }

  if (compatible && (baseMask & LC_MESSAGES_MASK)) {
    if (!p32_localeinfo_messages_copy (&locale->LocaleInfo.LcMessages, heap, &base->LocaleInfo.LcMessages)) {
      goto fail_messages;
    }
  } else {
    if (!p32_localeinfo_messages (locale, heap)) {
      goto fail_messages;
    }
  }

  if (compatible && (baseMask & LC_MONETARY_MASK)) {
    if (!p32_localeinfo_monetary_copy (&locale->LocaleInfo.LcMonetary, heap, &base->LocaleInfo.LcMonetary)) {
      goto fail_monetary;
    }
  } else {
    if (!p32_localeinfo_monetary (locale, heap)) {
      goto fail_monetary;
    }
  }

  if (compatible && (baseMask & LC_NUMERIC_MASK)) {
    if (!p32_localeinfo_numeric_copy (&locale->LocaleInfo.LcNumeric, heap, &base->LocaleInfo.LcNumeric)) {
      goto fail_numeric;
    }
  } else {
    if (!p32_localeinfo_numeric (locale, heap)) {
      goto fail_numeric;
    }
  }

  if (compatible && (baseMask & LC_TIME_MASK)) {
    if (!p32_localeinfo_time_copy (&locale->LocaleInfo.LcTime, heap, &base->LocaleInfo.LcTime)) {
      goto fail_time;
    }
  } else {
    if (!p32_localeinfo_time (locale, heap)) {
      goto fail_time;
    }
  }

  return true;

fail_time:
  p32_localeinfo_numeric_free (&locale->LocaleInfo.LcNumeric, heap);
fail_numeric:
  p32_localeinfo_monetary_free (&locale->LocaleInfo.LcMonetary, heap);
fail_monetary:
  p32_localeinfo_messages_free (&locale->LocaleInfo.LcMessages, heap);
fail_messages:
  p32_localeinfo_ctype_free (&locale->LocaleInfo.LcCtype, heap);
fail_ctype:
  p32_localeinfo_collate_free (&locale->LocaleInfo.LcCollate, heap);
fail_collate:
  return false;
}

/**
 * Set CRT locale to `locale`.
 *
 * This is a wrapper around `[_w]setlocale` and `_setmbcp`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32SetLocale (locale_t locale) {
  void *ret       = NULL;
  void *separator = NULL;

  /**
   * `_wsetlocale` is available starting with msvcrt20.dll.
   */
#if P32_CRT >= P32_MSVCRT20
  separator = wcsrchr (locale->CrtLocaleStrings.W.LcAll, L';');

  if (separator != NULL) {
    *(wchar_t *) separator = L'\0';
  }

  ret = _wsetlocale (LC_ALL, locale->CrtLocaleStrings.W.LcAll);

  if (separator != NULL) {
    *(wchar_t *) separator = L';';
  }
#endif
#if P32_CRT < P32_MSVCR110
  /**
   * In some cases `_wsetlocale` may fail to set locale, while `setlocale`
   * succeeds with the same string converted to locale's default ANSI code page.
   */
  if (ret == NULL) {
#if P32_CRT >= P32_MSVCRT20 && defined(LIBPOSIX32_TEST)
    _RPTW1 (_CRT_WARN, L"Call to _wsetlocale(LC_ALL, \"%s\") has failed.\n", locale->CrtLocaleStrings.W.LcAll);
#endif

    separator = strrchr (locale->CrtLocaleStrings.A.LcAll, ';');

    if (separator != NULL) {
      *(char *) separator = '\0';
    }

    ret = setlocale (LC_ALL, locale->CrtLocaleStrings.A.LcAll);

    if (separator != NULL) {
      *(char *) separator = ';';
    }
  }
#endif

  if (ret == NULL) {
#if defined(LIBPOSIX32_TEST)
    _RPTW1 (_CRT_WARN, L"LC_ALL(%s): failed to set locale\n", locale->CrtLocaleStrings.W.LcAll);
#endif

    goto fail;
  }

#if P32_CRT >= P32_MSVCRT20
  uint32_t codePage = locale->Charset.CodePage;

#if P32_CRT < P32_UCRT
  /**
   * Since old CRTs do not support UTF-8, we can't set multibyte code page
   * to 65001.
   *
   * Note that CRT's multibyte functions do not work with UTF-8 even when
   * multibyte code page is set to 65001. They act as if it was SBCS code page.
   */
  if (codePage == CP_UTF8) {
    codePage = P32_CODEPAGE_ASCII;
  }
#endif

  /**
   * Set code page used by CRT's multibyte functions.
   */
  if (_setmbcp (codePage) == -1) {
#if defined(LIBPOSIX32_TEST)
    _RPTW1 (_CRT_ERROR, L"Call to _setmbcp(%u) has failed.\n", locale->Charset.CodePage);
#endif

    goto fail;
  }
#endif

  return true;

fail:
  return false;
}

/**
 * Set Thread Tocale.
 */
static locale_t P32UseLocale (ThreadStorage *tls, ThreadLocaleState *threadLocaleState, locale_t locale) {
#if P32_CRT >= P32_MSVCR80
  /**
   * Check if `locale`'s code page is supported by CRT.
   */
  if (!p32_charset_usable (locale->Charset.CodePage, P32_CHARSET_REJECT_CRT, 0)) {
    return NULL;
  }
#endif

  locale_t oldThreadLocale = tls->ThreadLocale->Handle;
  locale_t newThreadLocale = P32DupLocale (locale, tls->Heap);

  if (newThreadLocale == NULL) {
    goto fail;
  }

#if P32_CRT >= P32_MSVCR80
  if (!P32SetThreadLocaleState (_ENABLE_PER_THREAD_LOCALE, threadLocaleState)) {
    goto fail_freelocale;
  }

  assert (threadLocaleState->PreviousState == _DISABLE_PER_THREAD_LOCALE || oldThreadLocale != LC_GLOBAL_LOCALE);

  /**
   * Set CRT's thread locale to `newThreadLocale`.
   */
  if (!P32SetLocale (newThreadLocale)) {
    goto fail_restore;
  }
#endif

  /**
   * Free previous Thread Locale.
   */
  if (tls->ThreadLocale->Locale != LC_GLOBAL_LOCALE) {
    P32FreeLocale (tls->ThreadLocale->Locale, tls->Heap);
  }

  tls->ThreadLocale->Locale = newThreadLocale;
  tls->ThreadLocale->Handle = locale;

  return oldThreadLocale;
  UNREFERENCED_PARAMETER (threadLocaleState);

#if P32_CRT >= P32_MSVCR80
fail_restore:
  if (!P32RestoreThreadLocaleState (threadLocaleState)) {
    p32_terminate (L"Thread Locale: failed to restore thread locale state.");
  }

  /**
   * If previous thread locale state was `_ENABLE_PER_THREAD_LOCALE`,
   * we need to restore previous CRT's thread locale.
   */
  if (threadLocaleState->CurrentState == _ENABLE_PER_THREAD_LOCALE) {
    assert (tls->ThreadLocale->Locale != LC_GLOBAL_LOCALE);

    if (!P32SetLocale (tls->ThreadLocale->Locale)) {
      p32_terminate (L"Thread Locale: failed to restore previous locale.");
    }
  }

fail_freelocale:
  P32FreeLocale (newThreadLocale, tls->Heap);
#endif

fail:
  return NULL;
}

/**
 * Set Thread Locale to Global Locale.
 */
static locale_t P32UseGlobalLocale (ThreadStorage *tls, ThreadLocaleState *threadLocaleState) {
#if P32_CRT >= P32_MSVCR80
  /**
   * If current thread locale state is `_ENABLE_PER_THREAD_LOCALE`,
   * set CRT's thread locale to "C" and change thread locale state to
   * `_DISABLE_PER_THREAD_LOCALE`.
   */
  if (threadLocaleState->CurrentState == _ENABLE_PER_THREAD_LOCALE) {
    assert (P32GlobalLocale.DefaultLocale != NULL);

    /**
     * TODO: this is redundant and introduces an unrecoverable error.
     * Set thread locale to "C" locale before switching to Global Locale.
     */
    if (!P32SetLocale (P32GlobalLocale.DefaultLocale)) {
      goto fail;
    }

    if (!P32SetThreadLocaleState (_DISABLE_PER_THREAD_LOCALE, NULL)) {
      goto fail;
    }
  }
#endif

  locale_t oldThreadLocale = LC_GLOBAL_LOCALE;

  /**
   * If TLS is initialized, update Thread Locale.
   */
  if (tls != NULL) {
    oldThreadLocale           = tls->ThreadLocale->Handle;
    tls->ThreadLocale->Handle = LC_GLOBAL_LOCALE;

#ifdef LIBPOSIX32_DLL
    if (tls->ThreadLocale->Locale != LC_GLOBAL_LOCALE) {
      P32FreeLocale (tls->ThreadLocale->Locale, tls->Heap);
      tls->ThreadLocale->Locale = LC_GLOBAL_LOCALE;
    }
#else
    P32DestroyThreadLocale (tls);
    p32_tls_check ();
#endif
  }

  return oldThreadLocale;
  UNREFERENCED_PARAMETER (threadLocaleState);

#if P32_CRT >= P32_MSVCR80
fail:
  /**
   * We need to make sure that CRT's thread locale is the same as it was
   * before calling `uselocale`.
   */
  if (!P32SetLocale (tls->ThreadLocale->Locale)) {
    p32_terminate (L"Thread Locale: failed to restore previous locale.");
  }

  return NULL;
#endif
}

/**
 * Free `locale_t` object `locale`.
 */
static void P32FreeLocale (locale_t locale, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  /**
   * Invalidate locale_t object.
   */
  locale->Magic = 0;
  locale->Mask  = 0;

  memset (&locale->Charset, 0, sizeof (Charset));
  memset (&locale->Functions, 0, sizeof (LocaleFunctions));

#if P32_CRT >= P32_MSVCR80 && defined(LIBPOSIX32_TEST)
  _free_locale (locale->CrtLocale.LcCollate);
  _free_locale (locale->CrtLocale.LcCtype);
  _free_locale (locale->CrtLocale.LcMonetary);
  _free_locale (locale->CrtLocale.LcNumeric);
  _free_locale (locale->CrtLocale.LcTime);
#endif

  p32_winlocale_destroy (&locale->WinLocale.LcCollate, heap);
  p32_winlocale_destroy (&locale->WinLocale.LcCtype, heap);
  p32_winlocale_destroy (&locale->WinLocale.LcMessages, heap);
  p32_winlocale_destroy (&locale->WinLocale.LcMonetary, heap);
  p32_winlocale_destroy (&locale->WinLocale.LcNumeric, heap);
  p32_winlocale_destroy (&locale->WinLocale.LcTime, heap);

  p32_localestr_free (&locale->WindowsLocaleStrings, heap);
  p32_localestr_free (&locale->CrtLocaleStrings, heap);
  p32_localestr_free (&locale->IsoLocaleStrings, heap);

  p32_localeinfo_collate_free (&locale->LocaleInfo.LcCollate, heap);
  p32_localeinfo_ctype_free (&locale->LocaleInfo.LcCtype, heap);
  p32_localeinfo_messages_free (&locale->LocaleInfo.LcMessages, heap);
  p32_localeinfo_monetary_free (&locale->LocaleInfo.LcMonetary, heap);
  p32_localeinfo_numeric_free (&locale->LocaleInfo.LcNumeric, heap);
  p32_localeinfo_time_free (&locale->LocaleInfo.LcTime, heap);

  HeapFree (heapHandle, 0, locale);
}

/**
 * Create copy of `locale_t` object `locale`.
 */
static locale_t P32DupLocale (locale_t locale, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  locale_t newLocale = (locale_t) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, sizeof (p32_locale_t));

  if (newLocale == NULL) {
    _set_errno (ENOMEM);
    return NULL;
  }

  newLocale->Magic   = P32_LOCALE_MAGIC;
  newLocale->Mask    = locale->Mask;
  newLocale->Charset = locale->Charset;

  if (!p32_winlocale_copy (&newLocale->WinLocale.LcCollate, heap, &locale->WinLocale.LcCollate)) {
    goto fail;
  }
  if (!p32_winlocale_copy (&newLocale->WinLocale.LcCtype, heap, &locale->WinLocale.LcCtype)) {
    goto fail;
  }
  if (!p32_winlocale_copy (&newLocale->WinLocale.LcMessages, heap, &locale->WinLocale.LcMessages)) {
    goto fail;
  }
  if (!p32_winlocale_copy (&newLocale->WinLocale.LcMonetary, heap, &locale->WinLocale.LcMonetary)) {
    goto fail;
  }
  if (!p32_winlocale_copy (&newLocale->WinLocale.LcNumeric, heap, &locale->WinLocale.LcNumeric)) {
    goto fail;
  }
  if (!p32_winlocale_copy (&newLocale->WinLocale.LcTime, heap, &locale->WinLocale.LcTime)) {
    goto fail;
  }

  if (!p32_localestr_copy (&newLocale->WindowsLocaleStrings, heap, &locale->WindowsLocaleStrings)) {
    goto fail;
  }
  if (!p32_localestr_copy (&newLocale->CrtLocaleStrings, heap, &locale->CrtLocaleStrings)) {
    goto fail;
  }
  if (!p32_localestr_copy (&newLocale->IsoLocaleStrings, heap, &locale->IsoLocaleStrings)) {
    goto fail;
  }

#if P32_CRT >= P32_MSVCR80 && defined(LIBPOSIX32_TEST)
  P32CreateLocale (newLocale);
#endif

  newLocale->Functions = locale->Functions;

  if (!p32_localeinfo_collate_copy (&newLocale->LocaleInfo.LcCollate, heap, &locale->LocaleInfo.LcCollate)) {
    goto fail;
  }
  if (!p32_localeinfo_ctype_copy (&newLocale->LocaleInfo.LcCtype, heap, &locale->LocaleInfo.LcCtype)) {
    goto fail;
  }
  if (!p32_localeinfo_messages_copy (&newLocale->LocaleInfo.LcMessages, heap, &locale->LocaleInfo.LcMessages)) {
    goto fail;
  }
  if (!p32_localeinfo_monetary_copy (&newLocale->LocaleInfo.LcMonetary, heap, &locale->LocaleInfo.LcMonetary)) {
    goto fail;
  }
  if (!p32_localeinfo_numeric_copy (&newLocale->LocaleInfo.LcNumeric, heap, &locale->LocaleInfo.LcNumeric)) {
    goto fail;
  }
  if (!p32_localeinfo_time_copy (&newLocale->LocaleInfo.LcTime, heap, &locale->LocaleInfo.LcTime)) {
    goto fail;
  }

  return newLocale;

fail:
  P32FreeLocale (newLocale, heap);

  return NULL;
}

/**
 * Create new `locale_t` object.
 */
static locale_t P32NewLocale (int mask, const wchar_t *localeString, locale_t base, uintptr_t heap, int flags) {
  HANDLE heapHandle = (HANDLE) heap;

  /**
   * Allocate memory for new `locale_t` object.
   */
  locale_t locale = (locale_t) HeapAlloc (heapHandle, HEAP_ZERO_MEMORY, sizeof (p32_locale_t));

  if (locale == NULL) {
    _set_errno (ENOMEM);
    return NULL;
  }

  /**
   * Locale for locale categories missing from `mask`.
   */
  locale_t baseLocale = base;

  /**
   * Locale categories to copy from `baseLocale`
   */
  int baseMask = 0;

  /**
   * If `baseLocale` is NULL, use default "C" locale.
   */
  if (baseLocale == NULL && mask != LC_ALL_MASK) {
    assert (P32GlobalLocale.DefaultLocale != NULL);
    baseLocale = P32GlobalLocale.DefaultLocale;
  }

  /**
   * Broken-down `localeString`.
   */
  LocaleStrings localeStrings = {0};

  if (localeString[0] == L'\0') {
    /**
     * Explicit request to use user's default locale.
     */
    if (flags & NEWLOCALE_USER_DEFAULT) {
      if (!p32_localestr_user_default (&localeStrings, heap)) {
        goto fail;
      }

      /**
       * Obtain locale from environment variables with fallback to user's
       * default locale.
       */
    } else {
      if (!p32_localestr_from_env (&localeStrings, heap, mask)) {
        goto fail;
      }
    }
  } else {
    /**
     * Split `localeString`.
     */
    if (!p32_localestr_split (&localeStrings, heap, localeString)) {
      goto fail;
    }
  }

  /**
   * Locale strings for all locale categories specified in `mask` must be
   * in `localeStrings` at this point.
   */
  if (localeStrings.Mask != LC_ALL && (localeStrings.Mask & mask) != mask) {
    goto fail_free_locale_strings;
  }

  if (localeStrings.Mask == LC_ALL) {
    if (!P32ResolveLocaleSingleString (locale, heap, &localeStrings, mask)) {
      goto fail_destroy_locales;
    }
  } else {
    if (!P32ResolveLocale (locale, heap, &localeStrings, mask)) {
      goto fail_destroy_locales;
    }
  }

  /**
   * When only some of locale categories are specified in `mask`,
   * copy remaining locale categories from `baseLocale`.
   */
  if (locale->Mask != LC_ALL_MASK) {
    baseMask = LC_ALL_MASK & ~locale->Mask;

    if (!P32CopyLocale (locale, heap, baseLocale, baseMask)) {
      goto fail_destroy_locales;
    }
  }

  assert (locale->Mask == LC_ALL_MASK);

  /**
   * Check if all locale categories use the same locale.
   */
  bool singleLocale = true;

  singleLocale &= p32_winlocale_equal (&locale->WinLocale.LcCtype, &locale->WinLocale.LcCollate);
  singleLocale &= p32_winlocale_equal (&locale->WinLocale.LcCtype, &locale->WinLocale.LcMessages);
  singleLocale &= p32_winlocale_equal (&locale->WinLocale.LcCtype, &locale->WinLocale.LcMonetary);
  singleLocale &= p32_winlocale_equal (&locale->WinLocale.LcCtype, &locale->WinLocale.LcNumeric);
  singleLocale &= p32_winlocale_equal (&locale->WinLocale.LcCtype, &locale->WinLocale.LcTime);

  /**
   * Figure out code page to use, and whether it is supported and usable.
   */
  if (!P32LocaleCharset (locale, flags)) {
    goto fail_destroy_locales;
  }

  /**
   * How to format LC_ALL locale string.
   *
   * LC_ALL: format using single locale string for all categories.
   * LC_ALL_MASK: format as list including locale string for each category.
   */
  int lcAllFormatMask = (singleLocale ? LC_ALL : LC_ALL_MASK);

  locale->WindowsLocaleStrings.Mask = lcAllFormatMask;
  locale->CrtLocaleStrings.Mask     = lcAllFormatMask;
  locale->IsoLocaleStrings.Mask     = lcAllFormatMask;

  /**
   * Format locale strings.
   */
  if (!P32LocaleStrings (locale, heap)) {
    goto fail_free_formatted_strings;
  }

#if P32_CRT >= P32_MSVCR80 && defined(LIBPOSIX32_TEST)
  /**
   * Create `_locale_t` objects to use with CRT functions.
   */
  P32CreateLocale (locale);
#endif

  /**
   * Get locale-specific functions.
   */
  P32LocaleFunctions (locale);

  /**
   * Get locale information.
   */
  if (!P32LocaleInfo (locale, heap, baseLocale, baseMask)) {
    goto fail_free_locales;
  }

  p32_localestr_free (&localeStrings, heap);

  locale->Magic = P32_LOCALE_MAGIC;

  return locale;

fail_free_locales:
#if P32_CRT >= P32_MSVCR80 && defined(LIBPOSIX32_TEST)
  if (locale->CrtLocale.LcCollate != NULL) {
    _free_locale (locale->CrtLocale.LcCollate);
  }
  if (locale->CrtLocale.LcCtype != NULL) {
    _free_locale (locale->CrtLocale.LcCtype);
  }
  if (locale->CrtLocale.LcMonetary != NULL) {
    _free_locale (locale->CrtLocale.LcMonetary);
  }
  if (locale->CrtLocale.LcNumeric != NULL) {
    _free_locale (locale->CrtLocale.LcNumeric);
  }
  if (locale->CrtLocale.LcTime != NULL) {
    _free_locale (locale->CrtLocale.LcTime);
  }
#endif

fail_free_formatted_strings:
  p32_localestr_free (&locale->WindowsLocaleStrings, heap);
  p32_localestr_free (&locale->CrtLocaleStrings, heap);
  p32_localestr_free (&locale->IsoLocaleStrings, heap);

fail_destroy_locales:
  if (locale->Mask & LC_COLLATE_MASK) {
    p32_winlocale_destroy (&locale->WinLocale.LcCollate, heap);
  }
  if (locale->Mask & LC_CTYPE_MASK) {
    p32_winlocale_destroy (&locale->WinLocale.LcCtype, heap);
  }
  if (locale->Mask & LC_MESSAGES_MASK) {
    p32_winlocale_destroy (&locale->WinLocale.LcMessages, heap);
  }
  if (locale->Mask & LC_MONETARY_MASK) {
    p32_winlocale_destroy (&locale->WinLocale.LcMonetary, heap);
  }
  if (locale->Mask & LC_NUMERIC_MASK) {
    p32_winlocale_destroy (&locale->WinLocale.LcNumeric, heap);
  }
  if (locale->Mask & LC_TIME_MASK) {
    p32_winlocale_destroy (&locale->WinLocale.LcTime, heap);
  }

fail_free_locale_strings:
  p32_localestr_free (&localeStrings, heap);

fail:
  HeapFree (heapHandle, 0, locale);

  return NULL;
}

/*******************************************************************************
 * External Functions
 */

char *p32_setlocale (int category, const char *localeString) {
  switch (category) {
    default:
      _set_errno (EINVAL);
      return NULL;

    case LC_ALL:
    case LC_COLLATE:
    case LC_CTYPE:
    case LC_MESSAGES:
    case LC_MONETARY:
    case LC_NUMERIC:
    case LC_TIME:
      break;
  }

  /**
   * Make sure Global Locale is initialized.
   */
  pthread_once (&P32GlobalLocale.GlobalInit, P32InitGlobalLocale);

  /**
   * Return locale string for `category`.
   */
  if (localeString == NULL) {
    switch (category) {
      case LC_ALL:
        return P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcAll;
      case LC_COLLATE:
        return P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcCollate;
      case LC_CTYPE:
        return P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcCtype;
      case LC_MESSAGES:
        return P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcMessages;
      case LC_MONETARY:
        return P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcMonetary;
      case LC_NUMERIC:
        return P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcNumeric;
      case LC_TIME:
        return P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcTime;
    }
  }

  /**
   * Obtain write lock for Global Locale.
   */
  if (pthread_rwlock_wrlock (&P32GlobalLocale.GlobalLock) != 0) {
    p32_terminate (L"Global Locale: failed to obtain write lock.\n");
  }

  uintptr_t heap       = P32GlobalLocale.Heap;
  HANDLE    heapHandle = (HANDLE) heap;

  /**
   * Return value.
   */
  char *ret = NULL;

  /**
   * Locale categories to set.
   */
  int mask = (category == LC_ALL ? LC_ALL_MASK : (1 << category));

  /**
   * New Global Locale.
   */
  locale_t locale = NULL;

  /**
   * Use LC_* and LANG environment variables.
   * If they are not set, use user's default locale.
   */
  if (localeString[0] == '\0') {
    locale = P32NewLocale (mask, L"", P32GlobalLocale.GlobalLocale, heap, NEWLOCALE_GLOBAL);

    /**
     * If `setlocale (LC_ALL, "")` has failed, try to explicitly fallback to
     * user's default locale.
     */
    if (locale == NULL && mask == LC_ALL_MASK) {
      locale = P32NewLocale (mask, L"", P32GlobalLocale.GlobalLocale, heap, NEWLOCALE_GLOBAL | NEWLOCALE_USER_DEFAULT);
    }
  } else {
    /**
     * We expect `localeString` to use active ANSI code page.
     */
    uint32_t codePage = GetACP ();

    /**
     * Converted `localeString`.
     */
    wchar_t *localeStringW = NULL;

    if (p32_private_mbstowcs (&localeStringW, localeString, heap, codePage) == -1) {
      goto unlock;
    }

    locale = P32NewLocale (mask, localeStringW, P32GlobalLocale.GlobalLocale, heap, NEWLOCALE_GLOBAL);

    HeapFree (heapHandle, 0, localeStringW);
  }

  if (locale == NULL) {
    goto unlock;
  }

#if P32_CRT >= P32_MSVCR80
  /**
   * If current thread locale state is `_ENABLE_PER_THREAD_LOCALE`,
   * we will need to restore CRT's thread locale afterwards.
   */
  ThreadLocaleState state = {0};

  P32GetThreadLocaleState (&state);

  /**
   * Active thread locale.
   */
  locale_t activeThreadLocale = LC_GLOBAL_LOCALE;

  if (state.CurrentState == _ENABLE_PER_THREAD_LOCALE) {
    activeThreadLocale = P32GetThreadLocale (true);
    assert (activeThreadLocale != LC_GLOBAL_LOCALE);
  }

  /**
   * `setlocale` must always operate on Global Locale.
   *
   * Change thread locale state to `_DISABLE_PER_THREAD_LOCALE`, and restore
   * it afterwards.
   */
  if (!P32SetThreadLocaleState (_DISABLE_PER_THREAD_LOCALE, &state)) {
    P32FreeLocale (locale, heap);
    goto unlock;
  }
#endif

  if (P32SetLocale (locale)) {
    locale_t oldLocale = InterlockedExchangePointer ((void *volatile *) &P32GlobalLocale.GlobalLocale, locale);
    assert (oldLocale != NULL);

    /**
     * Free previous Global Locale.
     */
    P32FreeLocale (oldLocale, heap);

    switch (category) {
      case LC_ALL:
        ret = P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcAll;
        break;
      case LC_COLLATE:
        ret = P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcCollate;
        break;
      case LC_CTYPE:
        ret = P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcCtype;
        break;
      case LC_MESSAGES:
        ret = P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcMessages;
        break;
      case LC_MONETARY:
        ret = P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcMonetary;
        break;
      case LC_NUMERIC:
        ret = P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcNumeric;
        break;
      case LC_TIME:
        ret = P32GlobalLocale.GlobalLocale->WindowsLocaleStrings.A.LcTime;
        break;
    }
  } else {
    P32FreeLocale (locale, heap);

    /**
     * Restore previous Global Locale.
     */
    if (!P32SetLocale (P32GlobalLocale.GlobalLocale)) {
      p32_terminate (L"Global Locale: failed to restore previous locale.");
    }
  }

#if P32_CRT >= P32_MSVCR80
  /**
   * Restore previous thread locale state.
   */
  if (!P32RestoreThreadLocaleState (&state)) {
    p32_terminate (L"Global Locale: failed to restore thread locale state.");
  }

  /**
   * If previous thread locale state was `_ENABLE_PER_THREAD_LOCALE`,
   * then call to `P32SetLocale` has also changed CRT's thread's locale.
   *
   * Restore previous CRT's thread locale.
   */
  if (state.CurrentState == _ENABLE_PER_THREAD_LOCALE) {
    assert (activeThreadLocale != LC_GLOBAL_LOCALE);

    if (!P32SetLocale (activeThreadLocale)) {
      p32_terminate (L"Ghread Locale: failed to restore previous locale.");
    }
  }
#endif

unlock:
  if (pthread_rwlock_unlock (&P32GlobalLocale.GlobalLock) != 0) {
    p32_terminate (L"Global Locale: failed to release write object.");
  }

  return ret;
}

locale_t p32_newlocale (int mask, const char *localeString, locale_t base) {
  if (mask == 0 || (mask & ~LC_ALL_MASK)) {
    _set_errno (EINVAL);
    return NULL;
  }

  if (localeString == NULL) {
    _set_errno (EINVAL);
    return NULL;
  }

  if (base != NULL && !P32IsValidLocale (base)) {
    _set_errno (EINVAL);
    return NULL;
  }

  /**
   * Make sure Default Locale is initialized.
   */
  pthread_once (&P32GlobalLocale.DefaultInit, P32InitDefaultLocale);

  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  /**
   * We expect `localeString` to use active ANSI code page.
   */
  uint32_t codePage = GetACP ();

  /**
   * Converted `localeString`.
   */
  wchar_t *localeStringW = NULL;

  if (p32_private_mbstowcs (&localeStringW, localeString, heap, codePage) == -1) {
    return NULL;
  }

  locale_t locale = P32NewLocale (mask, localeStringW, base, heap, 0);

  /**
   * Free `base` if new locale_t object was successfully created.
   */
  if (locale != NULL && base != NULL) {
    P32FreeLocale (base, heap);
  }

  HeapFree (heapHandle, 0, localeStringW);

  return locale;
}

locale_t p32_duplocale (locale_t locale) {
  if (locale != LC_GLOBAL_LOCALE && !P32IsValidLocale (locale)) {
    _set_errno (EINVAL);
    return NULL;
  }

  /**
   * Actual `locale_t` object to copy.
   */
  locale_t baseLocale = locale;

  /**
   * Create copy of Global Gocale.
   */
  if (locale == LC_GLOBAL_LOCALE) {
    pthread_once (&P32GlobalLocale.GlobalInit, P32InitGlobalLocale);

    if (pthread_rwlock_rdlock (&P32GlobalLocale.GlobalLock) != 0) {
      p32_terminate (L"Global Locale: failed to obtain read lock.\n");
    }

    assert (P32GlobalLocale.GlobalLocale != NULL);
    baseLocale = P32GlobalLocale.GlobalLocale;
  }

  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  /**
   * Copied `locale_t` object.
   */
  locale_t newLocale = P32DupLocale (baseLocale, heap);

  if (locale == LC_GLOBAL_LOCALE) {
    if (pthread_rwlock_unlock (&P32GlobalLocale.GlobalLock) != 0) {
      p32_terminate (L"Global Locale: failed to release read lock.");
    }
  }

  return newLocale;
}

void p32_freelocale (locale_t locale) {
  if (!P32IsValidLocale (locale)) {
    return;
  }

  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  P32FreeLocale (locale, heap);
}

locale_t p32_uselocale (locale_t locale) {
  if (locale != LC_GLOBAL_LOCALE && locale != NULL) {
    if (!P32IsValidLocale (locale)) {
      _set_errno (EINVAL);
      return NULL;
    }
  }

  bool allowNullTls = (locale == LC_GLOBAL_LOCALE || locale == NULL);

  /**
   * Current thread locale state.
   */
  ThreadLocaleState state = {0};

#if P32_CRT >= P32_MSVCR80
  P32GetThreadLocaleState (&state);

  allowNullTls &= (state.CurrentState == _DISABLE_PER_THREAD_LOCALE);
#endif

  /**
   * Get TLS.
   */
  ThreadStorage *tls = p32_tls (allowNullTls);

  /**
   * Return current Thread Locale.
   */
  if (locale == NULL) {
    if (P32_CRT >= P32_MSVCR80 && state.CurrentState == _DISABLE_PER_THREAD_LOCALE) {
      return LC_GLOBAL_LOCALE;
    } else if (tls == NULL || tls->ThreadLocale == NULL) {
      return LC_GLOBAL_LOCALE;
    } else {
      return tls->ThreadLocale->Handle;
    }
  }

  /**
   * Initialize Thread Locale structure in TLS.
   */
  if (tls != NULL && tls->ThreadLocale == NULL) {
#if P32_CRT >= P32_MSVCR80
    if (state.CurrentState == _ENABLE_PER_THREAD_LOCALE) {
      P32InitThreadLocaleUnsafe (tls);
    } else if (!P32InitThreadLocale (tls)) {
      return NULL;
    }
#else
    if (!P32InitThreadLocale (tls)) {
      return NULL;
    }
#endif
  }

  /**
   * Previous Thread Locale.
   */
  locale_t oldLocale = NULL;

  if (locale == LC_GLOBAL_LOCALE) {
    oldLocale = P32UseGlobalLocale (tls, &state);
  } else {
    assert (tls != NULL && tls->ThreadLocale != NULL);
    oldLocale = P32UseLocale (tls, &state, locale);
  }

  return oldLocale;
}

const char *p32_getlocalename_l (int category, locale_t locale) {
  switch (category) {
    default:
      _set_errno (EINVAL);
      return NULL;

    case LC_ALL:
    case LC_COLLATE:
    case LC_CTYPE:
    case LC_MESSAGES:
    case LC_MONETARY:
    case LC_NUMERIC:
    case LC_TIME:
      break;
  }

  if (locale != LC_GLOBAL_LOCALE && !P32IsValidLocale (locale)) {
    _set_errno (EINVAL);
    return NULL;
  }

  if (locale == LC_GLOBAL_LOCALE) {
    pthread_once (&P32GlobalLocale.GlobalInit, P32InitGlobalLocale);

    assert (P32GlobalLocale.GlobalLocale != NULL);
    locale = P32GlobalLocale.GlobalLocale;
  }

  const char *localeName = NULL;

  switch (category) {
    case LC_ALL:
      localeName = locale->WindowsLocaleStrings.A.LcAll;
      break;
    case LC_COLLATE:
      localeName = locale->IsoLocaleStrings.A.LcCollate;
      break;
    case LC_CTYPE:
      localeName = locale->IsoLocaleStrings.A.LcCtype;
      break;
    case LC_MESSAGES:
      localeName = locale->IsoLocaleStrings.A.LcMessages;
      break;
    case LC_MONETARY:
      localeName = locale->IsoLocaleStrings.A.LcMonetary;
      break;
    case LC_NUMERIC:
      localeName = locale->IsoLocaleStrings.A.LcNumeric;
      break;
    case LC_TIME:
      localeName = locale->IsoLocaleStrings.A.LcTime;
      break;
  }

  assert (localeName != NULL);

  return localeName;
}
