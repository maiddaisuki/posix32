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
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "core-winver.h"

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines functions which manipulate `Locale` objects.
 * These objects are used to represent Windows locales.
 *
 * Windows provides two different sets of NLS APIs:
 *
 * 1. Locale IDs (`LCID`). This is the older mechanism to represent locales
 * which was used prior to Windows Vista.
 *
 * 2. Locale names. This is a newer mechanism which was intoduced in
 * Windows Vista.
 *
 * While locale names are preferred over `LCID` locales, locale name APIs were
 * only introduced in Windows Vista. Support for locale names in CRT was
 * first added in msvcr110.dll.
 *
 * If we're building for CRT which does not support locale names, we always
 * use `LCID` locales regardless of targeted Windows version.
 *
 * If we're building for CRT which supports locale names, we either:
 *
 * - compile-in only Locale Name based implementaion; this means we are
 *   targeting Windows Vista or later
 *
 * - compile-in both implementaions and choose appropriate implementation to
 *   use at runtime; this means we are targeting Windows XP or older
 */

/*******************************************************************************
 * Declarations.
 */

/**
 * Functions defined in `locale_win32/locale_id.c`.
 */
#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
/**
 * Wrapper around `GetLocaleInfoW` which used `Locale` object instead of
 * `LCID` object.
 *
 * Return value is the same as for `GetLocaleInfoW`.
 */
static int P32WinlocaleLCIDGetLocaleInfoW (Locale *, uint32_t, wchar_t *, int);

/**
 * Wrapper around `GetCalendarInfoW` which uses `Locale` object instead of
 * `LCID` object.
 *
 * Return value is the same as for `GetCalendarInfoW`.
 */
static int P32WinlocaleLCIDGetCalendarInfoW (Locale *, Calendar, uint32_t, wchar_t *, int, uint32_t *);

#if !(P32_LOCALE_API & P32_LOCALE_API_LN)
/**
 * Wrapper around `CompareStringA` which uses `Locale` object instead of
 * `LCID` object.
 *
 * Return value is the same as for `CompareStringA`.
 */
static int P32WinlocaleLCIDCompareStringA (Locale *, uint32_t, const char *, int, const char *, int);
#endif

/**
 * Wrapper around `CompareStringW` which uses `Locale` object instead of
 * `LCID` object.
 *
 * Return value is the same as for `CompareStringW`.
 */
static int P32WinlocaleLCIDCompareStringW (Locale *, uint32_t, const wchar_t *, int, const wchar_t *, int);

#if !(P32_LOCALE_API & P32_LOCALE_API_LN)
/**
 * Wrapper around `LCMapStringA` which uses `Locale` object instead of
 * `LCID` object.
 *
 * Return value is the same as for `LCMapStringA`.
 */
static int P32WinlocaleLCIDMapStringA (Locale *, uint32_t, const char *, int, char *, int);
#endif

/**
 * Wrapper around `LCMapStringW` which uses `Locale` object instead of
 * `LCID` object.
 *
 * Return value is the same as for `LCMapStringW`.
 */
static int P32WinlocaleLCIDMapStringW (Locale *, uint32_t, const wchar_t *, int, wchar_t *, int);

/**
 * Implementation for `p32_winlocale_system_default`.
 */
static bool P32WinlocaleLCIDSystemDefault (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_user_default`.
 */
static bool P32WinlocaleLCIDUserDefault (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_resolve`.
 */
static bool P32WinlocaleLCIDResolve (Locale *locale, uintptr_t heap, LocaleMap *localeMap);

/**
 * Implementation for `p32_winlocale_copy`.
 */
static bool P32WinlocaleLCIDCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale);

/**
 * Implementation for `p32_winlocale_destroy`.
 */
static void P32WinlocaleLCIDDestroy (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_equal`.
 */
static bool P32WinlocaleLCIDEqual (Locale *l1, Locale *l2);

#ifdef LIBPOSIX32_TEST
/**
 * Implementation for `p32_winlocale_enum_system_locales` using
 * `EnumSystemLocalesW`.
 */
static void P32WinlocaleLCIDEnumSystemLocalesW (EnumSystemLocalesCallback, uintptr_t, void *);
#endif
#endif

/**
 * Functions defined in `locale_win32/locale_name.c`.
 */
#if (P32_LOCALE_API & P32_LOCALE_API_LN)
/**
 * Wrapper around `GetLocaleInfoEx` which used `Locale` object instead of
 * Locale Name.
 *
 * Return value is the same as for `GetLocaleInfoEx`.
 */
static int P32WinlocaleLNGetLocaleInfoW (Locale *, uint32_t, wchar_t *, int);

/**
 * Wrapper around `GetCalendarInfoEx` which uses `Locale` object instead of
 * Locale Name.
 *
 * Return value is the same as for `GetCalendarInfoEx`.
 */
static int P32WinlocaleLNGetCalendarInfoW (Locale *, Calendar, uint32_t, wchar_t *, int, uint32_t *);

/**
 * Wrapper around `CompareStringEx` which uses `Locale` object instead of
 * Locale Name.
 *
 * Return value is the same as for `CompareStringEx`.
 */
static int P32WinlocaleLNCompareStringW (Locale *, uint32_t, const wchar_t *, int, const wchar_t *, int);

/**
 * Wrapper around `LCMapStringEx` which uses `Locale` object instead of
 * Locale Name.
 *
 * Return value is the same as for `LCMapStringEx`.
 */
static int P32WinlocaleLNMapStringW (Locale *, uint32_t, const wchar_t *, int, wchar_t *, int);

/**
 * Implementation for `p32_winlocale_system_default`.
 */
static bool P32WinlocaleLNSystemDefault (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_user_default`.
 */
static bool P32WinlocaleLNUserDefault (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_resolve`.
 */
static bool P32WinlocaleLNResolve (Locale *locale, uintptr_t heap, LocaleMap *localeMap);

/**
 * Implementation for `p32_winlocale_copy`.
 */
static bool P32WinlocaleLNCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale);

/**
 * Implementation for `p32_winlocale_destroy`.
 */
static void P32WinlocaleLNDestroy (Locale *locale, uintptr_t heap);

/**
 * Implementation for `p32_winlocale_equal`.
 */
static bool P32WinlocaleLNEqual (Locale *l1, Locale *l2);

/**
 * Implementation for `p32_winlocale_get_language_name`.
 */
static bool P32WinlocaleLNGetLanguageName (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Implementation for `p32_winlocale_get_country_name`.
 */
static bool P32WinlocaleLNGetCountryName (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Implementation for `p32_winlocale_get_language_code`.
 */
static bool P32WinlocaleLNGetLanguageCode (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Implementation for `p32_winlocale_get_country_code`.
 */
static bool P32WinlocaleLNGetCountryCode (wchar_t **address, uintptr_t heap, Locale *locale);

#ifdef LIBPOSIX32_TEST
/**
 * Implementation for `p32_winlocale_enum_system_locales` using
 * `EnumSystemLocalesEx`.
 */
static void P32WinlocaleLNEnumSystemLocalesW (EnumSystemLocalesCallback, uintptr_t, void *);
#endif
#endif

/**
 * Functions defined in `locale_win32/region_id.c`.
 */
#if (P32_GEO_API & P32_GEO_API_GEOID)
/**
 * Store `GEOID` object in `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WinlocaleGeo (Locale *locale, uintptr_t heap);

/**
 * Copy `GEOID` object from `srcLocale` to `destLocale`,
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WinlocaleGeoCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale);

/**
 * Destroy `GEOID` object stored in `locale`.
 */
static void P32WinlocaleGeoDestroy (Locale *locale, uintptr_t heap);
#endif

/**
 * Functions defined in `locale_win32/region_name.c`.
 */
#if (P32_GEO_API & P32_GEO_API_RN)
/**
 * Store Region Name in `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WinlocaleRegionName (Locale *locale, uintptr_t heap);

/**
 * Copy Region Name from `srcLocale` to `destLocale`,
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WinlocaleRegionNameCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale);

/**
 * Destroy Region Name stored in `locale`.
 */
static void P32WinlocaleRegionNameDestroy (Locale *locale, uintptr_t heap);
#endif

/*******************************************************************************
 * Structures, functions and macros to lookup functions at runtime.
 */

/**
 * Windows 9x systems lack Unicode support; use `GetModuleHandleA` instead of
 * `GetModuleHandleW`.
 */
#if P32_WIN9X
#define P32GetModuleHandle(module) GetModuleHandleA (module)
#else
#define P32GetModuleHandle(module) GetModuleHandleW (TEXT (module))
#endif

/**
 * Convenience wrapper for `GetProcAddress`.
 */
#define P32GetProcAddress(module, func) (Func##func) (UINT_PTR) GetProcAddress (module, #func)

/**
 * Suppress warnings about conversion between data and function pointers with
 * picky compilers.
 */
#define F(ptr) (PVOID) (UINT_PTR) ptr

/**
 * Convenience wrapper for `InterlockedExchangePointer`.
 */
#define P32AtomicExchange(target, source) InterlockedExchangePointer ((void *volatile *) target, F (source))

#if (P32_LOCALE_API & P32_LOCALE_API_LCID) && (P32_LOCALE_API & P32_LOCALE_API_LN)
#define DYNAMIC_CHECKS
#define DYNAMIC_IMPLEMENTATION

/**
 * Function type corresponding to `GetSystemDefaultLocaleName`.
 */
typedef INT (WINAPI *FuncGetSystemDefaultLocaleName) (LPWSTR, INT);

/**
 * Function type corresponding to `GetUserDefaultLocaleName`.
 */
typedef INT (WINAPI *FuncGetUserDefaultLocaleName) (LPWSTR, INT);

/**
 * Function type corresponding to `CompareStringEx`.
 */
typedef INT (WINAPI *FuncCompareStringEx) (
  LPCWSTR,
  DWORD,
  LPCWSTR,
  INT,
  LPCWSTR,
  INT,
  LPNLSVERSIONINFO,
  LPVOID,
  LPARAM
);

/**
 * Function type corresponding to `GetCalendarInfoEx`.
 */
typedef INT (WINAPI *FuncGetCalendarInfoEx) (LPCWSTR, CALID, LPCWSTR, CALTYPE, LPWSTR, INT, LPDWORD);

/**
 * Function type corresponding to `GetLocaleInfoEx`.
 */
typedef INT (WINAPI *FuncGetLocaleInfoEx) (LPCWSTR, LCTYPE, LPWSTR, INT);

/**
 * Function type corresponding to `LCMapStringEx`.
 */
typedef INT (WINAPI *FuncLCMapStringEx) (LPCWSTR, DWORD, LPCWSTR, INT, LPWSTR, INT, LPNLSVERSIONINFO, LPVOID, LPARAM);

/**
 * Function type corresponding to `EnumSystemLocalesEx`.
 */
typedef BOOL (WINAPI *FuncEnumSystemLocalesEx) (LOCALE_ENUMPROCEX, DWORD, LPARAM, LPVOID);
#endif /* LCID and Locale Name APIs */

#if P32_WINNT < P32_WINNT_WIN7
#if (P32_LOCALE_API & P32_LOCALE_API_LN)
#define DYNAMIC_CHECKS

/**
 * Function type corresponding to `ResolveLocaleName`.
 */
typedef INT (WINAPI *FuncResolveLocaleName) (LPCWSTR, LPWSTR, INT);

/**
 * Initialization thunk for `ResolveLocaleName`.
 */
static INT WINAPI P32InitResolveLocaleName (LPCWSTR, LPWSTR, INT);

/**
 * Stub to use if `ResolveLocaleName` is not available.
 */
static INT WINAPI P32ResolveLocaleName (LPCWSTR, LPWSTR, INT);
#endif /* Locale name APIs */
#endif /* P32_WINNT < Windows 7 */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
#define DYNAMIC_CHECKS
#define DYNAMIC_IMPLEMENTATION

/**
 * Function type corresponding to `GetGeoInfoEx`.
 */
typedef INT (WINAPI *FuncGetGeoInfoEx) (LPWSTR, GEOTYPE, LPWSTR, INT);
#endif /* P32_WINNT < Windows 10 1709 */

#ifdef DYNAMIC_CHECKS
/**
 * Pointers to Windows Locale APIs which are looked up at runtime.
 */
typedef struct LocaleApi {
  pthread_once_t Init;

/**
 * When the library is configured with support for both `LCID` locales and
 * Locale Names, we choose appropriate implementation at runtime.
 *
 * We will have to lookup Locale Name APIs at runtime since we cannot reference
 * them directly; this would prevent DLL from loading on old Windows versions.
 */
#if (P32_LOCALE_API & P32_LOCALE_API_LCID) && (P32_LOCALE_API & P32_LOCALE_API_LN)
  FuncGetSystemDefaultLocaleName PtrGetSystemDefaultLocaleName;
  FuncGetUserDefaultLocaleName   PtrGetUserDefaultLocaleName;
  FuncCompareStringEx            PtrCompareStringEx;
  FuncGetCalendarInfoEx          PtrGetCalendarInfoEx;
  FuncGetLocaleInfoEx            PtrGetLocaleInfoEx;
  FuncLCMapStringEx              PtrLCMapStringEx;
  FuncEnumSystemLocalesEx        PtrEnumSystemLocalesEx;
#endif /* LCID and Locale Name APIs */

#if P32_WINNT < P32_WINNT_WIN7
#if (P32_LOCALE_API & P32_LOCALE_API_LN)
  /**
   * `ResolveLocaleName` is available since Windows 7.
   *
   * We do not rely on this function during locale resolution since its
   * behavior does not satisfy our needs.
   *
   * See comments in `locale_name.c` for more details.
   */
  FuncResolveLocaleName PtrResolveLocaleName;
#endif /* Locale Name APIs */
#endif /* P32_WINNT < Windows 7 */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
  /**
   * `GetGeoInfoEx` was added in Windows 10 1709 to replace `GetGeoInfo`.
   */
  FuncGetGeoInfoEx PtrGetGeoInfoEx;
#endif /* P32_WINNT < Windows 10 1709 */
} LocaleApi;

/**
 * Windows Locale APIs.
 */
static LocaleApi P32LocaleApi = {
  .Init = PTHREAD_ONCE_INIT,

#if (P32_LOCALE_API & P32_LOCALE_API_LCID) && (P32_LOCALE_API & P32_LOCALE_API_LN)
  .PtrGetSystemDefaultLocaleName = NULL,
  .PtrGetUserDefaultLocaleName   = NULL,
  .PtrCompareStringEx            = NULL,
  .PtrGetCalendarInfoEx          = NULL,
  .PtrGetLocaleInfoEx            = NULL,
  .PtrLCMapStringEx              = NULL,
  .PtrEnumSystemLocalesEx        = NULL,
#endif /* LCID and Locale Name APIs */

#if P32_WINNT < P32_WINNT_WIN7
#if (P32_LOCALE_API & P32_LOCALE_API_LN)
  .PtrResolveLocaleName = P32InitResolveLocaleName,
#endif /* Locale Name APIs */
#endif /* P32_WINNT < Windows 7 */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
  .PtrGetGeoInfoEx = NULL,
#endif /* P32_WINNT < Windows 10 1709 */
};

/**
 * Initialize `P32LocaleApi`.
 */
static void P32InitLocaleApi (void) {
  HMODULE kernel32 = P32GetModuleHandle ("kernel32.dll");
  assert (kernel32 != NULL);

#if (P32_LOCALE_API & P32_LOCALE_API_LCID) && (P32_LOCALE_API & P32_LOCALE_API_LN)
  /**
   * Lookup Locale Name APIs.
   */
  FuncGetSystemDefaultLocaleName ptrGetSystemDefaultLocaleName = NULL;
  FuncGetUserDefaultLocaleName   ptrGetUserDefaultLocaleName   = NULL;
  FuncCompareStringEx            ptrCompareStringEx            = NULL;
  FuncGetCalendarInfoEx          ptrGetCalendarInfoEx          = NULL;
  FuncGetLocaleInfoEx            ptrGetLocaleInfoEx            = NULL;
  FuncLCMapStringEx              ptrLCMapStringEx              = NULL;
  FuncEnumSystemLocalesEx        ptrEnumSystemLocalesEx        = NULL;

  if (kernel32 != NULL) {
    if (P32_WINNT_CHECK (P32_WINNT_VISTA, WindowsNtVista)) {
      ptrGetSystemDefaultLocaleName = P32GetProcAddress (kernel32, GetSystemDefaultLocaleName);
      assert (ptrGetSystemDefaultLocaleName);
      ptrGetUserDefaultLocaleName = P32GetProcAddress (kernel32, GetUserDefaultLocaleName);
      assert (ptrGetUserDefaultLocaleName);
      ptrCompareStringEx = P32GetProcAddress (kernel32, CompareStringEx);
      assert (ptrCompareStringEx);
      ptrGetCalendarInfoEx = P32GetProcAddress (kernel32, GetCalendarInfoEx);
      assert (ptrGetCalendarInfoEx);
      ptrGetLocaleInfoEx = P32GetProcAddress (kernel32, GetLocaleInfoEx);
      assert (ptrGetLocaleInfoEx);
      ptrLCMapStringEx = P32GetProcAddress (kernel32, LCMapStringEx);
      assert (ptrLCMapStringEx);
      ptrEnumSystemLocalesEx = P32GetProcAddress (kernel32, EnumSystemLocalesEx);
      assert (ptrEnumSystemLocalesEx);
    }
  }

  if (ptrGetSystemDefaultLocaleName != NULL) {
    P32AtomicExchange (&P32LocaleApi.PtrGetSystemDefaultLocaleName, ptrGetSystemDefaultLocaleName);
  }

  if (ptrGetUserDefaultLocaleName != NULL) {
    P32AtomicExchange (&P32LocaleApi.PtrGetUserDefaultLocaleName, ptrGetUserDefaultLocaleName);
  }

  if (ptrCompareStringEx != NULL) {
    P32AtomicExchange (&P32LocaleApi.PtrCompareStringEx, ptrCompareStringEx);
  }

  if (ptrGetCalendarInfoEx != NULL) {
    P32AtomicExchange (&P32LocaleApi.PtrGetCalendarInfoEx, ptrGetCalendarInfoEx);
  }

  if (ptrGetLocaleInfoEx != NULL) {
    P32AtomicExchange (&P32LocaleApi.PtrGetLocaleInfoEx, ptrGetLocaleInfoEx);
  }

  if (ptrLCMapStringEx != NULL) {
    P32AtomicExchange (&P32LocaleApi.PtrLCMapStringEx, ptrLCMapStringEx);
  }

  if (ptrEnumSystemLocalesEx != NULL) {
    P32AtomicExchange (&P32LocaleApi.PtrEnumSystemLocalesEx, ptrEnumSystemLocalesEx);
  }
#endif /* LCID and Locale Name APIs */

#if P32_WINNT < P32_WINNT_WIN7
#if (P32_LOCALE_API & P32_LOCALE_API_LN)
  /**
   * Lookup `ResolveLocaleName`.
   */
  FuncResolveLocaleName ptrResolveLocaleName = NULL;

  if (kernel32 != NULL) {
    if (P32_WINNT_CHECK (P32_WINNT_WIN7, WindowsNt7)) {
      ptrResolveLocaleName = P32GetProcAddress (kernel32, ResolveLocaleName);
      assert (ptrResolveLocaleName != NULL);
    }
  }

  if (ptrResolveLocaleName != NULL) {
    P32AtomicExchange (&P32LocaleApi.PtrResolveLocaleName, ptrResolveLocaleName);
  } else {
    P32AtomicExchange (&P32LocaleApi.PtrResolveLocaleName, P32ResolveLocaleName);
  }
#endif /* Locale Name APIs */
#endif /* P32_WINNT < Windows 7 */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
  /**
   * Lookup `GetGeoInfoEx`.
   */
  FuncGetGeoInfoEx ptrGetGeoInfoEx = NULL;

  if (kernel32 != NULL) {
    if (P32_WINNT_CHECK (P32_WINNT_WIN10, WindowsNt10)) {
      ptrGetGeoInfoEx = P32GetProcAddress (kernel32, GetGeoInfoEx);
    }
  }

  if (ptrGetGeoInfoEx != NULL) {
    P32AtomicExchange (&P32LocaleApi.PtrGetGeoInfoEx, ptrGetGeoInfoEx);
  }
#endif /* P32_WINNT < Windows 10 1709 */
}
#endif /* DYNAMIC_CHECKS */

#if (P32_LOCALE_API & P32_LOCALE_API_LCID) && (P32_LOCALE_API & P32_LOCALE_API_LN)
#define GetSystemDefaultLocaleName P32LocaleApi.PtrGetSystemDefaultLocaleName
#define GetUserDefaultLocaleName   P32LocaleApi.PtrGetUserDefaultLocaleName
#define CompareStringEx            P32LocaleApi.PtrCompareStringEx
#define GetCalendarInfoEx          P32LocaleApi.PtrGetCalendarInfoEx
#define GetLocaleInfoEx            P32LocaleApi.PtrGetLocaleInfoEx
#define LCMapStringEx              P32LocaleApi.PtrLCMapStringEx
#define EnumSystemLocalesEx        P32LocaleApi.PtrEnumSystemLocalesEx
#endif /* LCID and Locale Name APIs */

#if P32_WINNT < P32_WINNT_WIN7
#if (P32_LOCALE_API & P32_LOCALE_API_LN)
#define ResolveLocaleName P32LocaleApi.PtrResolveLocaleName

static INT WINAPI P32InitResolveLocaleName (LPCWSTR localeName, LPWSTR buffer, INT bufferSize) {
  pthread_once (&P32LocaleApi.Init, P32InitLocaleApi);
  return ResolveLocaleName (localeName, buffer, bufferSize);
}

static INT WINAPI P32ResolveLocaleName (LPCWSTR localeName, LPWSTR buffer, INT bufferSize) {
  SetLastError (ERROR_CALL_NOT_IMPLEMENTED);
  return 0;
  UNREFERENCED_PARAMETER (localeName);
  UNREFERENCED_PARAMETER (buffer);
  UNREFERENCED_PARAMETER (bufferSize);
}
#endif /* Locale name APIs */
#endif /* P32_WINNT < Windows 7 */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
#define GetGeoInfoEx P32LocaleApi.PtrGetGeoInfoEx
#endif /* P32_WINNT < Windows 10 1709 */

/*******************************************************************************
 * Structures, functions and macros to call appropriate implementation.
 *
 * This segment defines `Winlocale*` macros; these macros redirect to
 * appropriate implementation of specific functions for the current library
 * configuration.
 */

#if (P32_LOCALE_API & P32_LOCALE_API_LCID) && (P32_LOCALE_API & P32_LOCALE_API_LN)
/**
 * Function type for `WinlocaleGetLocaleInfoW`;
 * a wrapper to call `GetLocaleInfoW` or `GetLocaleInfoEx`.
 */
typedef int (*FuncWinlocaleGetLocaleInfoW) (Locale *, uint32_t, wchar_t *, int);

/**
 * Function type for `WinlocaleGetCalendarInfoW`;
 * a wrapper to call `GetCalendarInfoW` or `GetCalendarInfoEx`.
 */
typedef int (*FuncWinlocaleGetCalendarInfoW) (Locale *, Calendar, uint32_t, wchar_t *, int, uint32_t *);

/**
 * Function type for `WinlocaleCompareStringW`;
 * a wrapper to call `CompareStringW` or `CompareStringEx`.
 */
typedef int (*FuncWinlocaleCompareStringW) (Locale *, uint32_t, const wchar_t *, int, const wchar_t *, int);

/**
 * Function type for `WinlocaleMapStringW`;
 * a wrapper to call `LCMapStringW` or `LCMapStringEx`.
 */
typedef int (*FuncWinlocaleMapStringW) (Locale *, uint32_t, const wchar_t *, int, wchar_t *, int);

/**
 * Function type for `WinlocaleGetLanguageName`;
 * implementation for `p32_winlocale_get_language_name`.
 */
typedef bool (*FuncWinlocaleGetLanguageName) (wchar_t **, uintptr_t, Locale *);

/**
 * Function type for `WinlocaleGetCountryName`;
 * implementation for `p32_winlocale_get_country_name`.
 */
typedef bool (*FuncWinlocaleGetCountryName) (wchar_t **, uintptr_t, Locale *);

/**
 * Function type for `WinlocaleGetLanguageCode`;
 * implementation for `p32_winlocale_get_language_code`.
 */
typedef bool (*FuncWinlocaleGetLanguageCode) (wchar_t **, uintptr_t, Locale *);

/**
 * Function type for `WinlocaleGetCountryCode`;
 * implementation for `p32_winlocale_get_country_code`.
 */
typedef bool (*FuncWinlocaleGetCountryCode) (wchar_t **, uintptr_t, Locale *);

/**
 * Function type for `WinlocaleSystemDefault`;
 * implementation for `p32_winlocale_system_default`.
 */
typedef bool (*FuncWinlocaleSystemDefault) (Locale *, uintptr_t);

/**
 * Function type for `WinlocaleUserDefault`;
 * implementation for `p32_winlocale_user_default`.
 */
typedef bool (*FuncWinlocaleUserDefault) (Locale *, uintptr_t);

/**
 * Function type for `WinlocaleResolve`;
 * implementation for `p32_winlocale_resolve`.
 */
typedef bool (*FuncWinlocaleResolve) (Locale *, uintptr_t, LocaleMap *);

/**
 * Function type for `WinlocaleCopy`;
 * implementation for `p32_winlocale_copy`.
 */
typedef bool (*FuncWinlocaleCopy) (Locale *, uintptr_t, Locale *);

/**
 * Function type for `WinlocaleCopy`;
 * implementation for `p32_winlocale_copy`.
 */
typedef bool (*FuncWinlocaleEqual) (Locale *, Locale *);

/**
 * Function type for `WinlocaleDestroy`;
 * implementation for `p32_winlocale_destroy`.
 */
typedef void (*FuncWinlocaleDestroy) (Locale *, uintptr_t);

#ifdef LIBPOSIX32_TEST
/**
 * Function type for `WinlocaleEnumSystemLocales`;
 * implementation for `p32_winlocale_enum_system_locales`.
 */
typedef void (*FuncWinlocaleEnumSystemLocales) (EnumSystemLocalesCallback, uintptr_t, void *);
#endif

/**
 * Initialization thunk for `WinlocaleGetLocaleInfoW`.
 */
static int P32InitWinlocaleGetLocaleInfoW (Locale *, uint32_t, wchar_t *, int);

/**
 * Initialization thunk for `WinlocaleGetCalendarInfoW`.
 */
static int P32InitWinlocaleGetCalendarInfoW (Locale *, Calendar, uint32_t, wchar_t *, int, uint32_t *);

/**
 * Initialization thunk for `WinlocaleCompareStringW`.
 */
static int P32InitWinlocaleCompareStringW (Locale *, uint32_t, const wchar_t *, int, const wchar_t *, int);

/**
 * Initialization thunk for `WinlocaleMapStringW`.
 */
static int P32InitWinlocaleMapStringW (Locale *, uint32_t, const wchar_t *, int, wchar_t *, int);

/**
 * Initialization thunk for `WinlocaleGetLanguageName `.
 */
static bool P32InitWinlocaleGetLanguageName (wchar_t **, uintptr_t, Locale *);

/**
 * Initialization thunk for `WinlocaleGetCountryName `.
 */
static bool P32InitWinlocaleGetCountryName (wchar_t **, uintptr_t, Locale *);

/**
 * Initialization thunk for `WinlocaleGetLanguageCode`.
 */
static bool P32InitWinlocaleGetLanguageCode (wchar_t **, uintptr_t, Locale *);

/**
 * Initialization thunk for `WinlocaleGetCountryCode`.
 */
static bool P32InitWinlocaleGetCountryCode (wchar_t **, uintptr_t, Locale *);

/**
 * Initialization thunk for `WinlocaleSystemDefault`.
 */
static bool P32InitWinlocaleSystemDefault (Locale *, uintptr_t);

/**
 * Initialization thunk for `WinlocaleUserDefault`.
 */
static bool P32InitWinlocaleUserDefault (Locale *, uintptr_t);

/**
 * Initialization thunk for `WinlocaleResolve`.
 */
static bool P32InitWinlocaleResolve (Locale *, uintptr_t, LocaleMap *);

/**
 * Initialization thunk for `WinlocaleCopy`.
 */
static bool P32InitWinlocaleCopy (Locale *, uintptr_t, Locale *);

/**
 * Initialization thunk for `WinlocaleEqual`.
 */
static bool P32InitWinlocaleEqual (Locale *, Locale *);

/**
 * Initialization thunk for `WinlocaleDestroy`.
 */
static void P32InitWinlocaleDestroy (Locale *, uintptr_t);

#ifdef LIBPOSIX32_TEST
/**
 * Initialization thunk for `WinlocaleEnumSystemLocales`.
 */
static void P32InitWinlocaleEnumSystemLocales (EnumSystemLocalesCallback, uintptr_t, void *);
#endif
#endif /* LCID and Locale Name APIs */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
/**
 * Function type for `WinlocaleGeo`;
 * corresponds to `P32WinlocaleGeo` and `P32WinlocaleRegionName`.
 */
typedef bool (*FuncWinlocaleGeo) (Locale *, uintptr_t);

/**
 * Function type for `WinlocaleGeoCopy`;
 * corresponds to `P32WinlocaleGeoCopy` and `P32WinlocaleRegionNameCopy`.
 */
typedef bool (*FuncWinlocaleGeoCopy) (Locale *, uintptr_t, Locale *);

/**
 * Function type for `WinlocaleGeoDestroy`;
 * corresponds to `P32WinlocaleGeoDestroy` and `P32WinlocaleRegionNameDestroy`.
 */
typedef void (*FuncWinlocaleGeoDestroy) (Locale *, uintptr_t);

/**
 * Initialization thunk for `WinlocaleGeo`.
 */
static bool P32InitWinlocaleGeo (Locale *, uintptr_t);

/**
 * Initialization thunk for `WinlocaleGeoCopy`.
 */
static bool P32InitWinlocaleGeoCopy (Locale *, uintptr_t, Locale *);

/**
 * Initialization thunk for `WinlocaleGeoDestroy`.
 */
static void P32InitWinlocaleGeoDestroy (Locale *, uintptr_t);
#endif /* P32_WINNT < Windows 10 1709 */

/**
 * Retrieve locale information as a string.
 *
 * Retrieved string is stored in `*request->OutputW`.
 *
 * If `P32_LOCALE_INFO_REQUEST_CONVERT` flag is set in `request->Flags`, then
 * string converted to `request->CodePage` is stored in `request->OutputA`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetTextualLocaleInfoW (LocaleInfoRequest *request, uintptr_t heap, Locale *locale);

/**
 * Retrieve locale information as an integer value.
 *
 * Retrieved value is stored in `*request->Output`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetNumericLocaleInfoW (LocaleInfoRequest *request, uintptr_t heap, Locale *locale);

/**
 * Retrieve calendar information as a string.
 *
 * Retrieved string is stored in `*request->OutputW`.
 *
 * If `P32_LOCALE_INFO_REQUEST_CONVERT` flag is set in `request->Flags`, then
 * string converted to `request->CodePage` is stored in `request->OutputA`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetTextualCalendarInfoW (CalendarInfoRequest *request, uintptr_t heap, Locale *locale);

/**
 * Retrieve calendar information as an integer value.
 *
 * Retrieved value is stored in `*request->Output`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetNumericCalendarInfoW (CalendarInfoRequest *request, uintptr_t heap, Locale *locale);

/**
 * Get language name for `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetLanguageNameFromLocale (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Get country name for `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetCountryNameFromLocale (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Get ISO-639 language code for `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetLanguageCodeFromLocale (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Get ISO-3166 country code for `locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetCountryCodeFromLocale (wchar_t **address, uintptr_t heap, Locale *locale);

/**
 * Finish construction of `Locale` object `locale`.
 *
 * Store the following information in `locale`:
 *
 * - Default ANSI and OEM code page used by `locale`
 * - Default and alternative calendar used by `locale`
 * - Geological information (`GEOID` or Region Name)
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32WinlocaleInfo (Locale *locale, uintptr_t heap);

#ifdef DYNAMIC_IMPLEMENTATION
/**
 * Pointers to implementation.
 */
typedef struct WinlocaleApi {
  pthread_once_t Init;

#if (P32_LOCALE_API & P32_LOCALE_API_LCID) && (P32_LOCALE_API & P32_LOCALE_API_LN)
  FuncWinlocaleGetLocaleInfoW   PtrWinlocaleGetLocaleInfoW;
  FuncWinlocaleGetCalendarInfoW PtrWinlocaleGetCalendarInfoW;
  FuncWinlocaleCompareStringW   PtrWinlocaleCompareStringW;
  FuncWinlocaleMapStringW       PtrWinlocaleMapStringW;
  FuncWinlocaleGetLanguageName  PtrWinlocaleGetLanguageName;
  FuncWinlocaleGetCountryName   PtrWinlocaleGetCountryName;
  FuncWinlocaleGetLanguageCode  PtrWinlocaleGetLanguageCode;
  FuncWinlocaleGetCountryCode   PtrWinlocaleGetCountryCode;
  FuncWinlocaleSystemDefault    PtrWinlocaleSystemDefault;
  FuncWinlocaleUserDefault      PtrWinlocaleUserDefault;
  FuncWinlocaleResolve          PtrWinlocaleResolve;
  FuncWinlocaleCopy             PtrWinlocaleCopy;
  FuncWinlocaleEqual            PtrWinlocaleEqual;
  FuncWinlocaleDestroy          PtrWinlocaleDestroy;

#ifdef LIBPOSIX32_TEST
  FuncWinlocaleEnumSystemLocales PtrWinlocaleEnumSystemLocales;
#endif
#endif /* LCID and Locale Name APIs */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
  FuncWinlocaleGeo        PtrWinlocaleGeo;
  FuncWinlocaleGeoCopy    PtrWinlocaleGeoCopy;
  FuncWinlocaleGeoDestroy PtrWinlocaleGeoDestroy;
#endif /* P32_WINNT < Windows 10 1709 */
} WinlocaleApi;

/**
 * Implementation.
 */
static WinlocaleApi P32WinlocaleApi = {
  .Init = PTHREAD_ONCE_INIT,

#if (P32_LOCALE_API & P32_LOCALE_API_LCID) && (P32_LOCALE_API & P32_LOCALE_API_LN)
  .PtrWinlocaleGetLocaleInfoW   = P32InitWinlocaleGetLocaleInfoW,
  .PtrWinlocaleGetCalendarInfoW = P32InitWinlocaleGetCalendarInfoW,
  .PtrWinlocaleCompareStringW   = P32InitWinlocaleCompareStringW,
  .PtrWinlocaleMapStringW       = P32InitWinlocaleMapStringW,
  .PtrWinlocaleGetLanguageName  = P32InitWinlocaleGetLanguageName,
  .PtrWinlocaleGetCountryName   = P32InitWinlocaleGetCountryName,
  .PtrWinlocaleGetLanguageCode  = P32InitWinlocaleGetLanguageCode,
  .PtrWinlocaleGetCountryCode   = P32InitWinlocaleGetCountryCode,
  .PtrWinlocaleSystemDefault    = P32InitWinlocaleSystemDefault,
  .PtrWinlocaleUserDefault      = P32InitWinlocaleUserDefault,
  .PtrWinlocaleResolve          = P32InitWinlocaleResolve,
  .PtrWinlocaleCopy             = P32InitWinlocaleCopy,
  .PtrWinlocaleEqual            = P32InitWinlocaleEqual,
  .PtrWinlocaleDestroy          = P32InitWinlocaleDestroy,

#ifdef LIBPOSIX32_TEST
  .PtrWinlocaleEnumSystemLocales = P32InitWinlocaleEnumSystemLocales,
#endif
#endif /* LCID and Locale Name APIs */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
  .PtrWinlocaleGeo        = P32InitWinlocaleGeo,
  .PtrWinlocaleGeoCopy    = P32InitWinlocaleGeoCopy,
  .PtrWinlocaleGeoDestroy = P32InitWinlocaleGeoDestroy,
#endif /* P32_WINNT < Windows 10 1709 */
};

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
static void P32InitGeoApi (void) {
  if (P32LocaleApi.PtrGetGeoInfoEx != NULL) {
    P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGeo, P32WinlocaleRegionName);
    P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGeoCopy, P32WinlocaleRegionNameCopy);
    P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGeoDestroy, P32WinlocaleRegionNameDestroy);
  } else {
    P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGeo, P32WinlocaleGeo);
    P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGeoCopy, P32WinlocaleGeoCopy);
    P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGeoDestroy, P32WinlocaleGeoDestroy);
  }
}
#endif /* P32_WINNT < Windows 10 1709 */

#if (P32_LOCALE_API & P32_LOCALE_API_LN)
/**
 * Store pointers to Locale Name based implementation in `P32WinlocaleApi`.
 *
 * This function returns `true` on success.
 *
 * This function returns `false` on failure. This may happen if any required
 * function is missing from `P32LocaleApi`; this should never happen under
 * normal circumstances.
 */
static bool P32InitLocaleNameApi (void) {
#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
  if (P32LocaleApi.PtrGetSystemDefaultLocaleName == NULL) {
    return false;
  }

  if (P32LocaleApi.PtrGetUserDefaultLocaleName == NULL) {
    return false;
  }

  if (P32LocaleApi.PtrCompareStringEx == NULL) {
    return false;
  }

  if (P32LocaleApi.PtrGetCalendarInfoEx == NULL) {
    return false;
  }

  if (P32LocaleApi.PtrGetLocaleInfoEx == NULL) {
    return false;
  }

  if (P32LocaleApi.PtrLCMapStringEx == NULL) {
    return false;
  }

#ifdef LIBPOSIX32_TEST
  if (P32LocaleApi.PtrEnumSystemLocalesEx == NULL) {
    return false;
  }
#endif

  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetLocaleInfoW, P32WinlocaleLNGetLocaleInfoW);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetCalendarInfoW, P32WinlocaleLNGetCalendarInfoW);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleCompareStringW, P32WinlocaleLNCompareStringW);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleMapStringW, P32WinlocaleLNMapStringW);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleSystemDefault, P32WinlocaleLNSystemDefault);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleUserDefault, P32WinlocaleLNUserDefault);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleResolve, P32WinlocaleLNResolve);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleCopy, P32WinlocaleLNCopy);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleEqual, P32WinlocaleLNEqual);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleDestroy, P32WinlocaleLNDestroy);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetLanguageName, P32WinlocaleLNGetLanguageName);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetCountryName, P32WinlocaleLNGetCountryName);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetLanguageCode, P32WinlocaleLNGetLanguageCode);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetCountryCode, P32WinlocaleLNGetCountryCode);

#ifdef LIBPOSIX32_TEST
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleEnumSystemLocales, P32WinlocaleLNEnumSystemLocalesW);
#endif
#endif /* LCID and Locale Name APIs */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
  P32InitGeoApi ();
#endif /* P32_WINNT < Windows 10 1709 */

  return true;
}
#endif /* Locale Name APIs */

#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
/**
 * Store pointers to `LCID` based implementation in `P32WinlocaleApi`.
 *
 * This function must only be called when code is running on Windows NT.
 */
static void P32InitLocaleApiWindowsNt (void) {
#if (P32_LOCALE_API & P32_LOCALE_API_LN)
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetLocaleInfoW, P32WinlocaleLCIDGetLocaleInfoW);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetCalendarInfoW, P32WinlocaleLCIDGetCalendarInfoW);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleCompareStringW, P32WinlocaleLCIDCompareStringW);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleMapStringW, P32WinlocaleLCIDMapStringW);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleSystemDefault, P32WinlocaleLCIDSystemDefault);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleUserDefault, P32WinlocaleLCIDUserDefault);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleResolve, P32WinlocaleLCIDResolve);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleCopy, P32WinlocaleLCIDCopy);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleEqual, P32WinlocaleLCIDEqual);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleDestroy, P32WinlocaleLCIDDestroy);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetLanguageName, P32GetLanguageNameFromLocale);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetCountryName, P32GetCountryNameFromLocale);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetLanguageCode, P32GetLanguageCodeFromLocale);
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleGetCountryCode, P32GetCountryCodeFromLocale);

#ifdef LIBPOSIX32_TEST
  P32AtomicExchange (&P32WinlocaleApi.PtrWinlocaleEnumSystemLocales, P32WinlocaleLCIDEnumSystemLocalesW);
#endif
#endif /* LCID and Locale Name APIs */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
  P32InitGeoApi ();
#endif /* P32_WINNT < Windows 10 1709 */
}
#endif /* LCID APIs */

/**
 * Initialize `P32WinlocaleApi`.
 */
static void P32InitWinlocaleApi (void) {
  pthread_once (&P32LocaleApi.Init, P32InitLocaleApi);

#if (P32_LOCALE_API & P32_LOCALE_API_LN)
  /**
   * If the code is running on Windows Vista or later, attempt to use
   * Locale Name APIs.
   */
  if (P32_WINNT_CHECK (P32_WINNT_VISTA, WindowsNtVista)) {
    if (P32InitLocaleNameApi ()) {
      return;
    }
  }
#endif

#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
  P32InitLocaleApiWindowsNt ();
#endif
}
#endif /* DYNAMIC_IMPLEMENTATION */

#define WinlocaleGetTextualLocaleInfo P32GetTextualLocaleInfoW
#define WinlocaleGetNumericLocaleInfo P32GetNumericLocaleInfoW

#define WinlocaleGetTextualCalendarInfo P32GetTextualCalendarInfoW
#define WinlocaleGetNumericCalendarInfo P32GetNumericCalendarInfoW

#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
#define WinlocaleCompareStringA P32WinlocaleLCIDCompareStringA
#define WinlocaleMapStringA     P32WinlocaleLCIDMapStringA
#endif

#if (P32_LOCALE_API & P32_LOCALE_API_LCID) && (P32_LOCALE_API & P32_LOCALE_API_LN)
#define WinlocaleEnumSystemLocales P32WinlocaleApi.PtrWinlocaleEnumSystemLocales
#define WinlocaleGetLocaleInfoW    P32WinlocaleApi.PtrWinlocaleGetLocaleInfoW
#define WinlocaleGetCalendarInfoW  P32WinlocaleApi.PtrWinlocaleGetCalendarInfoW
#define WinlocaleCompareStringW    P32WinlocaleApi.PtrWinlocaleCompareStringW
#define WinlocaleMapStringW        P32WinlocaleApi.PtrWinlocaleMapStringW
#define WinlocaleGetLanguageName   P32WinlocaleApi.PtrWinlocaleGetLanguageName
#define WinlocaleGetCountryName    P32WinlocaleApi.PtrWinlocaleGetCountryName
#define WinlocaleGetLanguageCode   P32WinlocaleApi.PtrWinlocaleGetLanguageCode
#define WinlocaleGetCountryCode    P32WinlocaleApi.PtrWinlocaleGetCountryCode
#define WinlocaleSystemDefault     P32WinlocaleApi.PtrWinlocaleSystemDefault
#define WinlocaleUserDefault       P32WinlocaleApi.PtrWinlocaleUserDefault
#define WinlocaleResolve           P32WinlocaleApi.PtrWinlocaleResolve
#define WinlocaleCopy              P32WinlocaleApi.PtrWinlocaleCopy
#define WinlocaleEqual             P32WinlocaleApi.PtrWinlocaleEqual
#define WinlocaleDestroy           P32WinlocaleApi.PtrWinlocaleDestroy

static int P32InitWinlocaleGetLocaleInfoW (Locale *locale, uint32_t info, wchar_t *buffer, int bufferSize) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleGetLocaleInfoW (locale, info, buffer, bufferSize);
}

static int P32InitWinlocaleGetCalendarInfoW (
  Locale   *locale,
  Calendar  calendar,
  uint32_t  info,
  wchar_t  *buffer,
  int       bufferSize,
  uint32_t *value
) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleGetCalendarInfoW (locale, calendar, info, buffer, bufferSize, value);
}

static int P32InitWinlocaleCompareStringW (
  Locale        *locale,
  uint32_t       flags,
  const wchar_t *wcs1,
  int            wcs1Length,
  const wchar_t *wcs2,
  int            wcs2Length
) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleCompareStringW (locale, flags, wcs1, wcs1Length, wcs2, wcs2Length);
}

static int P32InitWinlocaleMapStringW (
  Locale        *locale,
  uint32_t       flags,
  const wchar_t *wcs,
  int            wcsLength,
  wchar_t       *buffer,
  int            bufferSize
) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleMapStringW (locale, flags, wcs, wcsLength, buffer, bufferSize);
}

static bool P32InitWinlocaleGetLanguageName (wchar_t **address, uintptr_t heap, Locale *locale) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleGetLanguageName (address, heap, locale);
}

static bool P32InitWinlocaleGetCountryName (wchar_t **address, uintptr_t heap, Locale *locale) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleGetCountryName (address, heap, locale);
}

static bool P32InitWinlocaleGetLanguageCode (wchar_t **address, uintptr_t heap, Locale *locale) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleGetLanguageCode (address, heap, locale);
}

static bool P32InitWinlocaleGetCountryCode (wchar_t **address, uintptr_t heap, Locale *locale) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleGetCountryCode (address, heap, locale);
}

static bool P32InitWinlocaleSystemDefault (Locale *locale, uintptr_t heap) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleSystemDefault (locale, heap);
}

static bool P32InitWinlocaleUserDefault (Locale *locale, uintptr_t heap) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleUserDefault (locale, heap);
}

static bool P32InitWinlocaleResolve (Locale *locale, uintptr_t heap, LocaleMap *localeMap) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleResolve (locale, heap, localeMap);
}

static bool P32InitWinlocaleCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleCopy (destLocale, heap, srcLocale);
}

static bool P32InitWinlocaleEqual (Locale *l1, Locale *l2) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleEqual (l1, l2);
}

static void P32InitWinlocaleDestroy (Locale *locale, uintptr_t heap) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  WinlocaleDestroy (locale, heap);
}

#ifdef LIBPOSIX32_TEST
static void P32InitWinlocaleEnumSystemLocales (EnumSystemLocalesCallback callback, uintptr_t heap, void *data) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  WinlocaleEnumSystemLocales (callback, heap, data);
}
#endif
#else /* Only one implementation is compiled id */

#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
#define WinlocaleEnumSystemLocales P32WinlocaleLCIDEnumSystemLocalesW
#define WinlocaleGetLocaleInfoW    P32WinlocaleLCIDGetLocaleInfoW
#define WinlocaleGetCalendarInfoW  P32WinlocaleLCIDGetCalendarInfoW
#define WinlocaleCompareStringW    P32WinlocaleLCIDCompareStringW
#define WinlocaleMapStringW        P32WinlocaleLCIDMapStringW
#define WinlocaleGetLanguageName   P32GetLanguageNameFromLocale
#define WinlocaleGetCountryName    P32GetCountryNameFromLocale
#define WinlocaleGetLanguageCode   P32GetLanguageCodeFromLocale
#define WinlocaleGetCountryCode    P32GetCountryCodeFromLocale
#define WinlocaleSystemDefault     P32WinlocaleLCIDSystemDefault
#define WinlocaleUserDefault       P32WinlocaleLCIDUserDefault
#define WinlocaleResolve           P32WinlocaleLCIDResolve
#define WinlocaleCopy              P32WinlocaleLCIDCopy
#define WinlocaleEqual             P32WinlocaleLCIDEqual
#define WinlocaleDestroy           P32WinlocaleLCIDDestroy
#else /* Locale Name APIs */
#define WinlocaleEnumSystemLocales P32WinlocaleLNEnumSystemLocalesW
#define WinlocaleGetLocaleInfoW    P32WinlocaleLNGetLocaleInfoW
#define WinlocaleGetCalendarInfoW  P32WinlocaleLNGetCalendarInfoW
#define WinlocaleCompareStringW    P32WinlocaleLNCompareStringW
#define WinlocaleMapStringW        P32WinlocaleLNMapStringW
#define WinlocaleGetLanguageName   P32WinlocaleLNGetLanguageName
#define WinlocaleGetCountryName    P32WinlocaleLNGetCountryName
#define WinlocaleGetLanguageCode   P32WinlocaleLNGetLanguageCode
#define WinlocaleGetCountryCode    P32WinlocaleLNGetCountryCode
#define WinlocaleSystemDefault     P32WinlocaleLNSystemDefault
#define WinlocaleUserDefault       P32WinlocaleLNUserDefault
#define WinlocaleResolve           P32WinlocaleLNResolve
#define WinlocaleCopy              P32WinlocaleLNCopy
#define WinlocaleEqual             P32WinlocaleLNEqual
#define WinlocaleDestroy           P32WinlocaleLNDestroy
#endif /* Locale Name APIs */

#endif /* Only one implementation is compiled id */

#if P32_WINNT < P32_WINNT_WIN10 || (P32_WINNT == P32_WINNT_WIN10 && P32_NTDDI < NTDDI_WIN10_RS3)
#define WinlocaleGeo        P32WinlocaleApi.PtrWinlocaleGeo
#define WinlocaleGeoCopy    P32WinlocaleApi.PtrWinlocaleGeoCopy
#define WinlocaleGeoDestroy P32WinlocaleApi.PtrWinlocaleGeoDestroy

static bool P32InitWinlocaleGeo (Locale *locale, uintptr_t heap) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleGeo (locale, heap);
}

static bool P32InitWinlocaleGeoCopy (Locale *destLocale, uintptr_t heap, Locale *srcLocale) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  return WinlocaleGeoCopy (destLocale, heap, srcLocale);
}

static void P32InitWinlocaleGeoDestroy (Locale *locale, uintptr_t heap) {
  pthread_once (&P32WinlocaleApi.Init, P32InitWinlocaleApi);
  WinlocaleGeoDestroy (locale, heap);
}
#else /* Region Names APIs are always available */
#define WinlocaleGeo        P32WinlocaleRegionName
#define WinlocaleGeoCopy    P32WinlocaleRegionNameCopy
#define WinlocaleGeoDestroy P32WinlocaleRegionNameDestroy
#endif /* Region Names APIs are always available */

/*******************************************************************************
 * Functions to obtain locale information.
 */

static bool P32GetTextualLocaleInfoW (LocaleInfoRequest *request, uintptr_t heap, Locale *locale) {
  HANDLE heapHandle = (HANDLE) heap;

  LPWSTR buffer     = NULL;
  INT    bufferSize = 0;

  bufferSize = WinlocaleGetLocaleInfoW (locale, request->Info, NULL, 0);

  if (bufferSize == 0) {
    goto fail;
  }

  buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (WCHAR));

  if (buffer == NULL) {
    goto fail;
  }

  INT written = WinlocaleGetLocaleInfoW (locale, request->Info, buffer, bufferSize);

  if (written != bufferSize) {
    goto fail_free;
  }

  if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT) {
    CharsetConversionRequest conversionRequest = {0};

    conversionRequest.Flags    |= (P32_CHARSET_CONVERSION_WC_TO_MB);
    conversionRequest.Charset   = request->charset;
    conversionRequest.Input.W   = buffer;
    conversionRequest.Output.A  = request->OutputA;

    if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT_NO_BEST_FIT) {
      conversionRequest.Flags |= P32_CHARSET_CONVERSION_NO_BEST_FIT;
    }

    if (p32_charset_convert (&conversionRequest, heap) == -1) {
      /**
       * We may be allowed to return success if `p32_charset_convert` failed
       * because it is unable to convert retrieved locale information.
       */
      if (conversionRequest.Status != CharsetConversionRequestNoConversion) {
        goto fail_free;
      }

      /**
       * TODO: we should fallback to locale information used for "POSIX" locale.
       */
      if (request->Flags & (P32_LOCALE_INFO_REQUEST_CONVERT_FALLBACK)) {
        if (p32_private_strdup (request->OutputA, "", heap) == -1) {
          goto fail_free;
        }
      } else if ((request->Flags & (P32_LOCALE_INFO_REQUEST_CONVERT_NO_ERROR)) == 0) {
        goto fail_free;
      }
    }
  }

  *request->OutputW = buffer;

  return true;

fail_free:
  HeapFree (heapHandle, 0, buffer);

fail:
  return false;
}

static bool P32GetNumericLocaleInfoW (LocaleInfoRequest *request, uintptr_t heap, Locale *locale) {
  return WinlocaleGetLocaleInfoW (locale, LOCALE_RETURN_NUMBER | request->Info, (LPWSTR) request->Output, 2) == 2;
  UNREFERENCED_PARAMETER (heap);
}

static bool P32GetTextualCalendarInfoW (CalendarInfoRequest *request, uintptr_t heap, Locale *locale) {
  HANDLE heapHandle = (HANDLE) heap;

  Calendar calendar = locale->Calendar;

  if (request->Flags & P32_CALENDAR_INFO_REQUEST_ALTERNATIVE) {
    assert (locale->AlternativeCalendar != 0);
    calendar = locale->AlternativeCalendar;
  }

  LPWSTR buffer     = NULL;
  INT    bufferSize = 0;

  bufferSize = WinlocaleGetCalendarInfoW (locale, calendar, request->Info, buffer, bufferSize, NULL);

  if (bufferSize == 0) {
    goto fail;
  }

  buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (WCHAR));

  if (buffer == NULL) {
    goto fail;
  }

  INT written = WinlocaleGetCalendarInfoW (locale, calendar, request->Info, buffer, bufferSize, NULL);

  if (written != bufferSize) {
    goto fail_free;
  }

  if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT) {
    CharsetConversionRequest conversionRequest = {0};

    conversionRequest.Flags    |= (P32_CHARSET_CONVERSION_WC_TO_MB);
    conversionRequest.Charset   = request->charset;
    conversionRequest.Input.W   = buffer;
    conversionRequest.Output.A  = request->OutputA;

    if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT_NO_BEST_FIT) {
      conversionRequest.Flags |= P32_CHARSET_CONVERSION_NO_BEST_FIT;
    }

    if (p32_charset_convert (&conversionRequest, heap) == -1) {
      /**
       * We may be allowed to return success if `p32_charset_convert` failed
       * because it is unable to convert retrieved locale information.
       */
      if (conversionRequest.Status != CharsetConversionRequestNoConversion) {
        goto fail_free;
      }

      /**
       * TODO: we should fallback to locale information used for "POSIX" locale.
       */
      if (request->Flags & (P32_LOCALE_INFO_REQUEST_CONVERT_FALLBACK)) {
        if (p32_private_strdup (request->OutputA, "", heap) == -1) {
          goto fail_free;
        }
      } else if ((request->Flags & (P32_LOCALE_INFO_REQUEST_CONVERT_NO_ERROR)) == 0) {
        goto fail_free;
      }
    }
  }

  *request->OutputW = buffer;

  return true;

fail_free:
  HeapFree (heapHandle, 0, buffer);

fail:
  return false;
}

static bool P32GetNumericCalendarInfoW (CalendarInfoRequest *request, uintptr_t heap, Locale *locale) {
  Calendar calendar = locale->Calendar;

  if (request->Flags & P32_CALENDAR_INFO_REQUEST_ALTERNATIVE) {
    assert (locale->AlternativeCalendar != 0);
    calendar = locale->AlternativeCalendar;
  }

  return WinlocaleGetCalendarInfoW (locale, calendar, CAL_RETURN_NUMBER | request->Info, NULL, 0, request->Output) == 2;
  UNREFERENCED_PARAMETER (heap);
}

static bool P32GetLanguageNameFromLocale (wchar_t **address, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info    = LOCALE_SENGLANGUAGE;
  infoRequest.OutputW = address;

  return WinlocaleGetTextualLocaleInfo (&infoRequest, heap, locale);
}

static bool P32GetCountryNameFromLocale (wchar_t **address, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info    = LOCALE_SENGCOUNTRY;
  infoRequest.OutputW = address;

  return WinlocaleGetTextualLocaleInfo (&infoRequest, heap, locale);
}

static bool P32GetLanguageCodeFromLocale (wchar_t **address, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info    = LOCALE_SISO639LANGNAME;
  infoRequest.OutputW = address;

  return WinlocaleGetTextualLocaleInfo (&infoRequest, heap, locale);
}

static bool P32GetCountryCodeFromLocale (wchar_t **address, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info    = LOCALE_SISO3166CTRYNAME;
  infoRequest.OutputW = address;

  return WinlocaleGetTextualLocaleInfo (&infoRequest, heap, locale);
}

static bool P32WinlocaleInfo (Locale *locale, uintptr_t heap) {
  uint32_t calendar            = 0;
  uint32_t alternativeCalendar = 0;
  uint32_t ansiCodePage        = 0;
  uint32_t oemCodePage         = 0;

  LocaleInfoRequest infoRequest = {0};

  /**
   * Locale's default calendar.
   */
  infoRequest.Info   = LOCALE_ICALENDARTYPE;
  infoRequest.Output = &calendar;

  if (!WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale)) {
    goto fail;
  }

  /**
   * Locale's alternative calendar.
   *
   * If there is no alternative calendar, then value `(CALID)0` is returned.
   */
  infoRequest.Info   = LOCALE_IOPTIONALCALENDAR;
  infoRequest.Output = &alternativeCalendar;

  if (!WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale)) {
    goto fail;
  }

  /**
   * Locale's default ANSI code page.
   * For an Unicode Locales, ANSI code page is returned as `CP_ACP`.
   */
  infoRequest.Info   = LOCALE_IDEFAULTANSICODEPAGE;
  infoRequest.Output = &ansiCodePage;

  if (!WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale)) {
    goto fail;
  }

  /**
   * Locale's default OEM code page.
   * For an Unicode Locales, OEM code page is returned as `CP_OEMCP`.
   */
  infoRequest.Info   = LOCALE_IDEFAULTCODEPAGE;
  infoRequest.Output = &oemCodePage;

  if (!WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale)) {
    goto fail;
  }

  if (!WinlocaleGeo (locale, heap)) {
    goto fail;
  }

  locale->Calendar            = calendar;
  locale->AlternativeCalendar = alternativeCalendar;
  locale->CodePage.Ansi       = ansiCodePage;
  locale->CodePage.Oem        = oemCodePage;

  return true;

fail:
  return false;
}

/*******************************************************************************
 * Implementation.
 */

#ifdef LIBPOSIX32_TEST
/**
 * Structure used with `p32_winlocale_enum_system_locales` function;
 * all fields of this structure correspond to its arguments.
 */
typedef struct EnumSystemLocaleData {
  EnumSystemLocalesCallback Callback;
  uintptr_t                 Heap;
  void                     *Data;
} EnumSystemLocaleData;
#endif

#if (P32_LOCALE_API & P32_LOCALE_API_LCID)
#include "locale_win32/locale_id.c"
#endif

#if (P32_GEO_API & P32_GEO_API_GEOID)
#include "locale_win32/region_id.c"
#endif

#if (P32_LOCALE_API & P32_LOCALE_API_LN)
#include "locale_win32/locale_name.c"
#endif

#if (P32_GEO_API & P32_GEO_API_RN)
#include "locale_win32/region_name.c"
#endif

/*******************************************************************************
 * External Functions.
 */

bool p32_winlocale_are_file_apis_ansi (void) {
/**
 * The functions `SetFileApisToANSI`, `SetFileApisToOEM` and `AreFileApisANSI`
 * are not available to UWP Applications.
 */
#ifdef LIBPOSIX32_UWP
  return TRUE;
#else
  return AreFileApisANSI ();
#endif
}

#if (P32_LOCALE_API & P32_LOCALE_API_LCID) && !(P32_LOCALE_API & P32_LOCALE_API_LN)
int p32_winlocale_compare_ansi_string (
  Locale     *locale,
  uint32_t    flags,
  const char *str1,
  int         str1Length,
  const char *str2,
  int         str2Length
) {
  return WinlocaleCompareStringA (locale, flags, str1, str1Length, str2, str2Length);
}

int p32_winlocale_map_ansi_string (
  Locale     *locale,
  uint32_t    flags,
  const char *str,
  int         strLength,
  char       *buffer,
  int         bufferSize
) {
  return WinlocaleMapStringA (locale, flags, str, strLength, buffer, bufferSize);
}
#endif /* ANSI APIs */

int p32_winlocale_compare_unicode_string (
  Locale        *locale,
  uint32_t       flags,
  const wchar_t *str1,
  int            str1Length,
  const wchar_t *str2,
  int            str2Length
) {
  return WinlocaleCompareStringW (locale, flags, str1, str1Length, str2, str2Length);
}

int p32_winlocale_map_unicode_string (
  Locale        *locale,
  uint32_t       flags,
  const wchar_t *str,
  int            strLength,
  wchar_t       *buffer,
  int            bufferSize
) {
  return WinlocaleMapStringW (locale, flags, str, strLength, buffer, bufferSize);
}

int p32_winlocale_get_unicode_string_type (
  Locale        *locale,
  uint32_t       flags,
  const wchar_t *str,
  int            strLength,
  uint16_t      *bits
) {
  return GetStringTypeW (flags, str, strLength, bits);
  UNREFERENCED_PARAMETER (locale);
}

bool p32_winlocale_system_default (Locale *locale, uintptr_t heap) {
  return WinlocaleSystemDefault (locale, heap);
}

bool p32_winlocale_user_default (Locale *locale, uintptr_t heap) {
  return WinlocaleUserDefault (locale, heap);
}

bool p32_winlocale_resolve (Locale *locale, uintptr_t heap, LocaleMap *localeMap) {
  return WinlocaleResolve (locale, heap, localeMap);
}

bool p32_winlocale_copy (Locale *destLocale, uintptr_t heap, Locale *srcLocale) {
  return WinlocaleCopy (destLocale, heap, srcLocale);
}

void p32_winlocale_destroy (Locale *locale, uintptr_t heap) {
  WinlocaleDestroy (locale, heap);
}

bool p32_winlocale_equal (Locale *l1, Locale *l2) {
  return WinlocaleEqual (l1, l2);
}

bool p32_winlocale_get_language_name (wchar_t **address, uintptr_t heap, Locale *locale) {
  return WinlocaleGetLanguageName (address, heap, locale);
}

bool p32_winlocale_get_country_name (wchar_t **address, uintptr_t heap, Locale *locale) {
  return WinlocaleGetCountryName (address, heap, locale);
}

bool p32_winlocale_get_language_code (wchar_t **address, uintptr_t heap, Locale *locale) {
  return WinlocaleGetLanguageCode (address, heap, locale);
}

bool p32_winlocale_get_country_code (wchar_t **address, uintptr_t heap, Locale *locale) {
  return WinlocaleGetCountryCode (address, heap, locale);
}

bool p32_winlocale_get_locale_info (LocaleInfoRequest *request, uintptr_t heap, Locale *locale) {
  if (request->Flags & P32_LOCALE_INFO_REQUEST_NUMERIC) {
    return WinlocaleGetNumericLocaleInfo (request, heap, locale);
  }

  return WinlocaleGetTextualLocaleInfo (request, heap, locale);
}

bool p32_winlocale_get_calendar_info (CalendarInfoRequest *request, uintptr_t heap, Locale *locale) {
  if (request->Flags & P32_LOCALE_INFO_REQUEST_NUMERIC) {
    return WinlocaleGetNumericCalendarInfo (request, heap, locale);
  }

  return WinlocaleGetTextualCalendarInfo (request, heap, locale);
}

#ifdef LIBPOSIX32_TEST
void p32_winlocale_enum_system_locales (EnumSystemLocalesCallback callback, uintptr_t heap, void *data) {
  WinlocaleEnumSystemLocales (callback, heap, data);
}

bool p32_winlocale_get_ansi_code_page (uint32_t *codePage, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info   = LOCALE_IDEFAULTANSICODEPAGE;
  infoRequest.Output = codePage;

  return WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale);
}

bool p32_winlocale_get_oem_code_page (uint32_t *codePage, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info   = LOCALE_IDEFAULTCODEPAGE;
  infoRequest.Output = codePage;

  return WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale);
}

bool p32_winlocale_get_mac_code_page (uint32_t *codePage, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info   = LOCALE_IDEFAULTMACCODEPAGE;
  infoRequest.Output = codePage;

  return WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale);
}

bool p32_winlocale_get_ebcdic_code_page (uint32_t *codePage, uintptr_t heap, Locale *locale) {
  LocaleInfoRequest infoRequest = {0};

  infoRequest.Info   = LOCALE_IDEFAULTEBCDICCODEPAGE;
  infoRequest.Output = codePage;

  return WinlocaleGetNumericLocaleInfo (&infoRequest, heap, locale);
}
#endif
