/**
 * Copyright 2025 Kirill Makurin
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
#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to manipulate `LocaleStrings` structure.
 */

#if P32_CRT <= P32_MSVCRT41 || (P32_MSVCRT && _WIN32_WINNT <= _WIN32_WINNT_NT4)
#define P32_USE_LEGACY_LOCALE_STRINGS
#endif

/**
 * Copy locale string from `src` to `dest`.
 */
static bool P32CopyLocaleStringsA (LocaleStringsA *dest, uintptr_t heap, LocaleStringsA *src) {
  if (p32_private_strdup (&dest->LcAll, src->LcAll, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->LcCollate, src->LcCollate, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->LcCtype, src->LcCtype, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->LcMessages, src->LcMessages, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->LcMonetary, src->LcMonetary, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->LcNumeric, src->LcNumeric, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->LcTime, src->LcTime, heap) == -1) {
    goto fail;
  }

  dest->CodePage = src->CodePage;

  return true;

fail:
  return false;
}

/**
 * Free `LocaleStringsA` structure.
 */
static void P32FreeLocaleStringsA (LocaleStringsA *localeStrings, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  localeStrings->CodePage = 0;

  if (localeStrings->LcAll != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcAll);
    localeStrings->LcAll = NULL;
  }

  if (localeStrings->LcCollate != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcCollate);
    localeStrings->LcCollate = NULL;
  }

  if (localeStrings->LcCtype != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcCtype);
    localeStrings->LcCtype = NULL;
  }

  if (localeStrings->LcMessages != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcMessages);
    localeStrings->LcMessages = NULL;
  }

  if (localeStrings->LcMonetary != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcMonetary);
    localeStrings->LcMonetary = NULL;
  }

  if (localeStrings->LcNumeric != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcNumeric);
    localeStrings->LcNumeric = NULL;
  }

  if (localeStrings->LcTime != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcTime);
    localeStrings->LcTime = NULL;
  }
}

/**
 * Copy locale string from `src` to `dest`.
 */
static bool P32CopyLocaleStringsW (LocaleStringsW *dest, uintptr_t heap, LocaleStringsW *src) {
  if (p32_private_wcsdup (&dest->LcAll, src->LcAll, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->LcCollate, src->LcCollate, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->LcCtype, src->LcCtype, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->LcMessages, src->LcMessages, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->LcMonetary, src->LcMonetary, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->LcNumeric, src->LcNumeric, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->LcTime, src->LcTime, heap) == -1) {
    goto fail;
  }

  dest->CodePage = src->CodePage;

  return true;

fail:
  return false;
}

/**
 * Free `LocaleStringsW` structure.
 */
static void P32FreeLocaleStringsW (LocaleStringsW *localeStrings, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  localeStrings->CodePage = 0;

  if (localeStrings->LcAll != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcAll);
    localeStrings->LcAll = NULL;
  }

  if (localeStrings->LcCollate != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcCollate);
    localeStrings->LcCollate = NULL;
  }

  if (localeStrings->LcCtype != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcCtype);
    localeStrings->LcCtype = NULL;
  }

  if (localeStrings->LcMessages != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcMessages);
    localeStrings->LcMessages = NULL;
  }

  if (localeStrings->LcMonetary != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcMonetary);
    localeStrings->LcMonetary = NULL;
  }

  if (localeStrings->LcNumeric != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcNumeric);
    localeStrings->LcNumeric = NULL;
  }

  if (localeStrings->LcTime != NULL) {
    HeapFree (heapHandle, 0, localeStrings->LcTime);
    localeStrings->LcTime = NULL;
  }
}

bool p32_localestr_copy (LocaleStrings *dest, uintptr_t heap, LocaleStrings *src) {
  if (!P32CopyLocaleStringsW (&dest->W, heap, &src->W)) {
    goto free_w;
  }

  if (!P32CopyLocaleStringsA (&dest->A, heap, &src->A)) {
    goto free_a;
  }

  dest->Format = src->Format;
  dest->Mask   = src->Mask;

  return true;

free_a:
  P32FreeLocaleStringsA (&dest->A, heap);
free_w:
  P32FreeLocaleStringsW (&dest->W, heap);

  return false;
}

void p32_localestr_free (LocaleStrings *localeStrings, uintptr_t heap) {
  localeStrings->Mask   = 0;
  localeStrings->Format = 0;
  P32FreeLocaleStringsA (&localeStrings->A, heap);
  P32FreeLocaleStringsW (&localeStrings->W, heap);
}

bool p32_localestr_split (LocaleStrings *localeStrings, uintptr_t heap, const wchar_t *string) {
  HANDLE heapHandle = (HANDLE) heap;

  wchar_t *separator = wcschr (string, L';');

  /**
   * No separator, which means that `string` contains single locale string.
   */
  if (separator == NULL) {
    if (p32_private_wcsdup (&localeStrings->W.LcAll, string, heap) == -1) {
      return false;
    }

    localeStrings->Mask = LC_ALL;

    return true;
  }

  const wchar_t *part = string;

  while (1) {
    wchar_t **buffer = NULL;

    /**
     * Offset to locale string from `part`.
     */
    ptrdiff_t offset = 0;

    /**
     * Length of locale string.
     */
    ptrdiff_t length = 0;

    /**
     * Locale category whose locale string is pointed by `part + offset`.
     */
    int category = 0;

    if (wcsncmp (part, L"LC_COLLATE=", 11) == 0) {
      category = LC_COLLATE_MASK;
      offset   = 11;
      length   = separator - part - offset;
      buffer   = &localeStrings->W.LcCollate;
    } else if (wcsncmp (part, L"LC_CTYPE=", 9) == 0) {
      category = LC_CTYPE_MASK;
      offset   = 9;
      length   = separator - part - offset;
      buffer   = &localeStrings->W.LcCtype;
    } else if (wcsncmp (part, L"LC_MESSAGES=", 12) == 0) {
      category = LC_MESSAGES_MASK;
      offset   = 12;
      length   = separator - part - offset;
      buffer   = &localeStrings->W.LcMessages;
    } else if (wcsncmp (part, L"LC_MONETARY=", 12) == 0) {
      category = LC_MONETARY_MASK;
      offset   = 12;
      length   = separator - part - offset;
      buffer   = &localeStrings->W.LcMonetary;
    } else if (wcsncmp (part, L"LC_NUMERIC=", 11) == 0) {
      category = LC_NUMERIC_MASK;
      offset   = 11;
      length   = separator - part - offset;
      buffer   = &localeStrings->W.LcNumeric;
    } else if (wcsncmp (part, L"LC_TIME=", 8) == 0) {
      category = LC_TIME_MASK;
      offset   = 8;
      length   = separator - part - offset;
      buffer   = &localeStrings->W.LcTime;
    } else {
      goto fail;
    }

    /**
     * Empty locale string.
     */
    if (length == 0) {
      goto fail;
    }

    /**
     * Locale string for the same category has been specified twice.
     *
     * Note that CRT's `setlocale` crashes in such case.
     */
    if (localeStrings->Mask & category) {
      goto fail;
    }

    *buffer = HeapAlloc (heapHandle, 0, (length + 1) * sizeof (wchar_t));

    if (*buffer == NULL) {
      goto fail;
    }

    wmemcpy (*buffer, part + offset, length);
    /* (part + offset)[length] is L';' */
    (*buffer)[length] = L'\0';

    localeStrings->Mask |= category;

    /**
     * We reached the end of the `string`.
     */
    if (*separator == L'\0') {
      break;
    }

    part = separator + 1;

    /**
     * Find next separator.
     */
    separator = wcschr (part, L';');

    /**
     * If `separator == NULL`, then there is still one more element in
     * the list, use L'\0' as separator.
     */
    if (separator == NULL) {
      separator = wcschr (part, L'\0');
    }
  }

  return true;

fail:
  p32_localestr_free (localeStrings, heap);

  return false;
}

/*******************************************************************************
 * Obtaining Locale Infromation from Environment
 */

/**
 * Obtain value of environment variable `varname`.
 *
 * If environment variable exists and its value is not an empty string,
 * store newly allocated buffer in `*address`. Buffer is allocated from `heap`.
 *
 * If `varname` is not found in environment or its value is an empty string,
 * and `fallbackVarname` is non-NULL, this function attemps to obtain value
 * of that environment variable instead. (It is expected to be L"LANG").
 *
 * Return values:
 *  `-1`: an error has occured
 *   `0`: non-empty string has been stored in `*address`
 *   `1`: environment variable does not exists, or its value is an empty string
 */
static int P32GetEnvVar (wchar_t **address, uintptr_t heap, const wchar_t *varname, const wchar_t *fallbackVarname) {
  HANDLE heapHandle = (HANDLE) heap;

  LPWSTR buffer     = NULL;
  DWORD  bufferSize = 0;

  bufferSize = GetEnvironmentVariableW (varname, buffer, bufferSize);

  if (bufferSize == 0) {
    assert (GetLastError () == ERROR_ENVVAR_NOT_FOUND);
    goto fallback;
  } else if (bufferSize == 1) {
    goto fallback;
  }

  buffer = (LPWSTR) HeapAlloc (heapHandle, 0, bufferSize * sizeof (wchar_t));

  if (buffer == NULL) {
    goto fail;
  }

  DWORD written = GetEnvironmentVariableW (varname, buffer, bufferSize);

  if (written == 0 || written >= bufferSize) {
    goto fail_free;
  }

  *address = buffer;

  return 0;

fallback:
  if (fallbackVarname != NULL) {
    return P32GetEnvVar (address, heap, fallbackVarname, NULL);
  }

  return 1;

fail_free:
  HeapFree (heapHandle, 0, buffer);

fail:
  return -1;
}

/**
 * Stores locale string corresponding to user default locale in `localeStrings`.
 *
 * If `localeStrings->Mask` is 0, store single string in `localeStrings->LcAll`.
 *
 * Otherwise, store a string for each locale category specified in `mask`
 * that is missing from `localeStrings->Mask` in the corresponding field.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32UserDefaultLocale (LocaleStrings *localeStrings, uintptr_t heap, int mask) {
  /**
   * User's default locale.
   */
  Locale defaultLocale = {0};

  if (!p32_winlocale_default (&defaultLocale, heap)) {
    return false;
  }

  assert (defaultLocale.LocaleName != NULL);

  bool success = false;

  /**
   * If no locale strings are stored in `localeStrigns`, store single string
   * in `localeStrings->LcAll`.
   */
  if (localeStrings->Mask == 0) {
    if (p32_private_wcsdup (&localeStrings->W.LcAll, defaultLocale.LocaleName, heap) == -1) {
      goto fail;
    }

    localeStrings->Mask = LC_ALL;

    /**
     * Store a string for each locale category in `mask` that is missing
     * from `localeStrings->Mask`.
     */
  } else {
    if ((mask & LC_COLLATE_MASK) && (localeStrings->Mask & LC_COLLATE_MASK) == 0) {
      if (p32_private_wcsdup (&localeStrings->W.LcCollate, defaultLocale.LocaleName, heap) == -1) {
        goto fail;
      }
      localeStrings->Mask |= LC_COLLATE_MASK;
    }

    if ((mask & LC_CTYPE_MASK) && (localeStrings->Mask & LC_CTYPE_MASK) == 0) {
      if (p32_private_wcsdup (&localeStrings->W.LcCtype, defaultLocale.LocaleName, heap) == -1) {
        goto fail;
      }
      localeStrings->Mask |= LC_CTYPE_MASK;
    }

    if ((mask & LC_MESSAGES_MASK) && (localeStrings->Mask & LC_MESSAGES_MASK) == 0) {
      if (p32_private_wcsdup (&localeStrings->W.LcMessages, defaultLocale.LocaleName, heap) == -1) {
        goto fail;
      }
      localeStrings->Mask |= LC_MESSAGES_MASK;
    }

    if ((mask & LC_MONETARY_MASK) && (localeStrings->Mask & LC_MONETARY_MASK) == 0) {
      if (p32_private_wcsdup (&localeStrings->W.LcMonetary, defaultLocale.LocaleName, heap) == -1) {
        goto fail;
      }
      localeStrings->Mask |= LC_MONETARY_MASK;
    }

    if ((mask & LC_NUMERIC_MASK) && (localeStrings->Mask & LC_NUMERIC_MASK) == 0) {
      if (p32_private_wcsdup (&localeStrings->W.LcNumeric, defaultLocale.LocaleName, heap) == -1) {
        goto fail;
      }
      localeStrings->Mask |= LC_NUMERIC_MASK;
    }

    if ((mask & LC_TIME_MASK) && (localeStrings->Mask & LC_TIME_MASK) == 0) {
      if (p32_private_wcsdup (&localeStrings->W.LcTime, defaultLocale.LocaleName, heap) == -1) {
        goto fail;
      }
      localeStrings->Mask |= LC_TIME_MASK;
    }

    assert ((localeStrings->Mask & mask) == mask);
  }

  success = true;

fail:
  p32_winlocale_destroy (&defaultLocale, heap);

  return success;
}

bool p32_localestr_user_default (LocaleStrings *localeStrings, uintptr_t heap) {
  assert (localeStrings->Mask == LC_ALL);
  return P32UserDefaultLocale (localeStrings, heap, LC_ALL_MASK);
}

bool p32_localestr_from_env (LocaleStrings *localeStrings, uintptr_t heap, int mask) {
  /**
   * If `LC_ALL` lookup is successful, use it for all categories.
   */
  switch (P32GetEnvVar (&localeStrings->W.LcAll, heap, L"LC_ALL", NULL)) {
    case -1:
      return false;
    case 0: /* Found */
      localeStrings->Mask = LC_ALL;
      return true;
    default: /* Not found */
      break;
  }

  /**
   * Try to lookup category-specific variables and fallback to `LANG`
   * if category-specific variable does not exist or contains an empty string.
   *
   * If `mask` specifies single locale category and it has been found in the
   * environment, return early.
   */

  if (mask & LC_COLLATE_MASK) {
    switch (P32GetEnvVar (&localeStrings->W.LcCollate, heap, L"LC_COLLATE", L"LANG")) {
      case -1:
        goto fail;
      case 0: /* Found */
        localeStrings->Mask |= LC_COLLATE_MASK;
        if (mask == LC_COLLATE_MASK) {
          return true;
        }
      case 1: /* Not found */
        break;
    }
  }

  if (mask & LC_CTYPE_MASK) {
    switch (P32GetEnvVar (&localeStrings->W.LcCtype, heap, L"LC_CTYPE", L"LANG")) {
      case -1:
        goto fail;
      case 0: /* Found */
        localeStrings->Mask |= LC_CTYPE_MASK;
        if (mask == LC_CTYPE_MASK) {
          return true;
        }
      case 1: /* Not found */
        break;
    }
  }

  if (mask & LC_MESSAGES_MASK) {
    switch (P32GetEnvVar (&localeStrings->W.LcMessages, heap, L"LC_MESSAGES", L"LANG")) {
      case -1:
        goto fail;
      case 0: /* Found */
        localeStrings->Mask |= LC_MESSAGES_MASK;
        if (mask == LC_MESSAGES_MASK) {
          return true;
        }
      case 1: /* Not found */
        break;
    }
  }

  if (mask & LC_MONETARY_MASK) {
    switch (P32GetEnvVar (&localeStrings->W.LcMonetary, heap, L"LC_MONETARY", L"LANG")) {
      case -1:
        goto fail;
      case 0: /* Found */
        localeStrings->Mask |= LC_MONETARY_MASK;
        if (mask == LC_MONETARY_MASK) {
          return true;
        }
      case 1: /* Not found */
        break;
    }
  }

  if (mask & LC_NUMERIC_MASK) {
    switch (P32GetEnvVar (&localeStrings->W.LcNumeric, heap, L"LC_NUMERIC", L"LANG")) {
      case -1:
        goto fail;
      case 0: /* Found */
        localeStrings->Mask |= LC_NUMERIC_MASK;
        if (mask == LC_NUMERIC_MASK) {
          return true;
        }
      case 1: /* Not found */
        break;
    }
  }

  if (mask & LC_TIME_MASK) {
    switch (P32GetEnvVar (&localeStrings->W.LcTime, heap, L"LC_TIME", L"LANG")) {
      case -1:
        goto fail;
      case 0: /* Found */
        localeStrings->Mask |= LC_TIME_MASK;
        if (mask == LC_TIME_MASK) {
          return true;
        }
      case 1: /* Not found */
        break;
    }
  }

  /**
   * Return if all requested categories have been found in the environment.
   */
  if ((localeStrings->Mask & mask) == mask) {
    return true;
  }

  /**
   * Fill the rest using user's default locale.
   */
  if (!P32UserDefaultLocale (localeStrings, heap, mask)) {
    goto fail;
  }

  assert (localeStrings->Mask == LC_ALL || (localeStrings->Mask & mask) == mask);

  return true;

fail:
  p32_localestr_free (localeStrings, heap);

  return false;
}

/*******************************************************************************
 * Formatting Locale Strings
 */

#ifdef P32_USE_LEGACY_LOCALE_STRINGS
#include "locale_string/legacy.c"
#endif

/**
 * Retieve language name associated with `locale` and store it in `*address`
 */
static bool P32GetLanguageName (wchar_t **address, uintptr_t heap, Locale *locale) {
  if (P32_LOCALE_NAMES && locale->Type == LOCALE_TYPE_PSEUDO_LOCALE) {
    Language language = {0};

    p32_language (locale->Map.Language, &language);

    return p32_private_wcsdup (address, language.Name, heap) != -1;
  }

  return p32_winlocale_getinfo (address, heap, locale, LOCALE_SENGLANGUAGE);
}

/**
 * Retieve language code associated with `locale` and store it in `*address`
 */
static bool P32GetLanguageCode (wchar_t **address, uintptr_t heap, Locale *locale) {
  if (P32_LOCALE_NAMES && locale->Type == LOCALE_TYPE_PSEUDO_LOCALE) {
    Language language = {0};

    p32_language (locale->Map.Language, &language);

    return p32_private_wcsdup (address, language.Code, heap) != -1;
  }

  return p32_winlocale_getinfo (address, heap, locale, LOCALE_SISO639LANGNAME);
}

/**
 * Retieve country name associated with `locale` and store it in `*address`
 */
static bool P32GetCountryName (wchar_t **address, uintptr_t heap, Locale *locale) {
  if (P32_LOCALE_NAMES && locale->Type == LOCALE_TYPE_PSEUDO_LOCALE) {
    Country country = {0};

    p32_country (locale->Map.Country, &country);

    return p32_private_wcsdup (address, country.Name, heap) != -1;
  }

  return p32_winlocale_getinfo (address, heap, locale, LOCALE_SENGCOUNTRY);
}

/**
 * Retieve country code associated with `locale` and store it in `*address`
 */
static bool P32GetCountryCode (wchar_t **address, uintptr_t heap, Locale *locale) {
  if (P32_LOCALE_NAMES && locale->Type == LOCALE_TYPE_PSEUDO_LOCALE) {
    Country country = {0};

    p32_country (locale->Map.Country, &country);

    return p32_private_wcsdup (address, country.Code, heap) != -1;
  }

  return p32_winlocale_getinfo (address, heap, locale, LOCALE_SISO3166CTRYNAME);
}

/**
 * Store "C" string in `*address`.
 *
 * This is used for "C" and "POSIX" locales.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32FormatCLocaleString (wchar_t **address, uintptr_t heap) {
  return p32_private_wcsdup (address, L"C", heap) != -1;
}

/**
 * Copy `locale->LocaleName` to `*address`
 *
 * This is used for Windows pseudo locales.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32FormatPseudoLocaleString (wchar_t **address, uintptr_t heap, Locale *locale) {
  return p32_private_wcsdup (address, locale->LocaleName, heap) != -1;
}

/**
 * Format single locale string using Windows Format.
 *
 * This string is returned by `setlocale` and `getlocalename_l (LC_ALL, ...)`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32FormatWindowsLocaleString (wchar_t **address, uintptr_t heap, Locale *locale, uint32_t codePage) {
  if (locale->Type == LOCALE_TYPE_POSIX) {
    return P32FormatCLocaleString (address, heap);
  }

  if (locale->Type == LOCALE_TYPE_PSEUDO_LOCALE) {
    return P32FormatPseudoLocaleString (address, heap, locale);
  }

  return p32_private_aswprintf (address, heap, L"%s.%u", locale->LocaleName, codePage) != -1;
}

/**
 * Format single locale string using CRT Format.
 *
 * This string is used with CRT's `[_w]setlocale` and `_[w]create_locale`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32FormatCrtLocaleString (wchar_t **address, uintptr_t heap, Locale *locale, uint32_t codePage) {
  HANDLE heapHandle = (HANDLE) heap;

  if (locale->Type == LOCALE_TYPE_POSIX) {
    return P32FormatCLocaleString (address, heap);
  }

#if P32_LOCALE_NAMES
  /**
   * When Windows locale name is passed to CRT's `[_w]setlocale` or
   * `_[w]create_locale`, it uses locale's default ANSI code page.
   *
   * Windows pseudo locales must be used with their default ANSI code pages.
   *
   * For Windows pseudo locales, there is no way to set specific code page when
   * using locale names, and `GetLocaleInfoEx` returns string which are not
   * accepted by CRT's `[_w]setlocale` and `_[w]create_locale`.
   *
   * If Windows pseudo locale is used only for specific locale categories and
   * `codePage` is not that pseude locale's default ANSI code page, we will
   * use information from `locale->Map` to construct reasonable locale string.
   */
  if (locale->CodePage.Ansi == codePage) {
    return p32_private_wcsdup (address, locale->LocaleName, heap) != -1;
  }
#endif
#ifdef P32_USE_LEGACY_LOCALE_STRINGS
  const CrtLocaleString *entry = NULL;

  if (codePage == P32_LOCALE_CODEPAGE (*locale)) {
    for (size_t i = 0; i < _countof (CrtLocaleStrings); ++i) {
      if (CrtLocaleStrings[i].Language == locale->Map.Language && CrtLocaleStrings[i].Country == locale->Map.Country) {
        entry = &CrtLocaleStrings[i];
        break;
      }
    }

    if (entry != NULL) {
      assert (entry->LocaleString != NULL);
      return p32_private_wcsdup (address, entry->LocaleString, heap) != -1;
    }
  }
#endif

  bool success = false;

  LPWSTR ll = NULL;
  LPWSTR cc = NULL;

  /**
   * UCRT supports "ll_CC.UTF-8" syntax for UTF-8 locales.
   *
   * This allows us to partially work around issue when CRT's `[_w]setlocale`
   * rejects locale string constructed from strings returned by
   * `GetLocaleInfo[Ex]`.
   *
   * However, we use this format only if no script was applied during locale
   * resolution and only for UTF-8 (this format is not accepted with other
   * character sets).
   */
  if (P32_CRT >= P32_UCRT && codePage == CP_UTF8 && locale->Map.Script == ScriptIndex_invalid) {
    if (!P32GetLanguageCode (&ll, heap, locale)) {
      goto fail;
    }

    if (!P32GetCountryCode (&cc, heap, locale)) {
      goto fail;
    }

    if (p32_private_aswprintf (address, heap, L"%s_%s.UTF-8", ll, cc) == -1) {
      goto fail;
    }
  } else {
    if (!P32GetLanguageName (&ll, heap, locale)) {
      goto fail;
    }

    if (!P32GetCountryName (&cc, heap, locale)) {
      goto fail;
    }

    if (p32_private_aswprintf (address, heap, L"%s_%s.%u", ll, cc, codePage) == -1) {
      goto fail;
    }
  }

  success = true;

fail:
  if (ll != NULL) {
    HeapFree (heapHandle, 0, ll);
  }
  if (cc != NULL) {
    HeapFree (heapHandle, 0, cc);
  }

  return success;
}

/**
 * Format single locale string using `ISO Format`.
 *
 * This string is returned by `getlocalename_l` when called for a single locale
 * category.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32FormatIsoLocaleString (wchar_t **address, uintptr_t heap, Locale *locale, uint32_t codePage) {
  HANDLE heapHandle = (HANDLE) heap;

  if (locale->Type == LOCALE_TYPE_POSIX) {
    return P32FormatCLocaleString (address, heap);
  }

  bool success = false;

  LPWSTR  ll          = NULL;
  LPWSTR  cc          = NULL;
  LPCWSTR xx          = NULL;
  LPWSTR  charsetName = NULL;

  if (!P32GetLanguageCode (&ll, heap, locale)) {
    goto fail;
  }

  if (!P32GetCountryCode (&cc, heap, locale)) {
    goto fail;
  }

  if (!p32_charset_name (&charsetName, heap, codePage)) {
    goto fail;
  }

  if (locale->Map.Modifier != ModifierIndex_invalid) {
    Modifier modifier = {0};

    p32_modifier (locale->Map.Modifier, &modifier);

    xx = modifier.Name;
  } else if (locale->Map.Script != ScriptIndex_invalid) {
    Script script = {0};

    p32_script (locale->Map.Script, &script);

    xx = script.Name;
  }

  if (xx != NULL) {
    if (p32_private_aswprintf (address, heap, L"%s_%s.%s@%s", ll, cc, charsetName, xx) == -1) {
      goto fail;
    }
  } else {
    if (p32_private_aswprintf (address, heap, L"%s_%s.%s", ll, cc, charsetName) == -1) {
      goto fail;
    }
  }

  success = true;

fail:
  if (charsetName != NULL) {
    HeapFree (heapHandle, 0, charsetName);
  }
  if (cc != NULL) {
    HeapFree (heapHandle, 0, cc);
  }
  if (ll != NULL) {
    HeapFree (heapHandle, 0, ll);
  }

  return success;
}

/**
 * Format locale string corresponding to `locale` and store it in `*address`.
 * The string is formatted using specified `format`.
 *
 * This buffer to store the formatted string will be allocated from `heap`.
 * The `codePage` specifies code page to be included to the formatted string.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32FormatLocaleString (
  wchar_t          **address,
  uintptr_t          heap,
  Locale            *locale,
  uint32_t           codePage,
  LocaleStringFormat format
) {
  switch (format) {
    case LOCALE_STRING_FORMAT_C:
      return P32FormatCLocaleString (address, heap);
    case LOCALE_STRING_FORMAT_WINDOWS:
      return P32FormatWindowsLocaleString (address, heap, locale, codePage);
    case LOCALE_STRING_FORMAT_CRT:
      return P32FormatCrtLocaleString (address, heap, locale, codePage);
    case LOCALE_STRING_FORMAT_ISO:
      return P32FormatIsoLocaleString (address, heap, locale, codePage);
    default:
      return false;
  }
}

/**
 * Store single string in `localeStrings->LcAll` which contains list of locale
 * categories constructed from all locale string in `localeStrings`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32FormatLcAllLocaleString (LocaleStringsW *localeStrings, uintptr_t heap) {
  /**
   * NOTE: LC_MESSAGES is in the very end.
   */
  const wchar_t format[] = L"LC_COLLATE=%s;LC_CTYPE=%s;LC_MONETARY=%s;LC_NUMERIC=%s;LC_TIME=%s;LC_MESSAGES=%s";

  int length = p32_private_aswprintf (
    &localeStrings->LcAll, heap, format, localeStrings->LcCollate, localeStrings->LcCtype, localeStrings->LcMonetary,
    localeStrings->LcNumeric, localeStrings->LcTime, localeStrings->LcMessages
  );

  return length != -1;
}

/**
 * Convert locale strings in `localeStrings->W` and store them in
 * `localeStrings->A`.
 *
 * Conversion is performed using `localeStrings->A.CodePage`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32ConverLocaleStrings (LocaleStrings *localeStrings, uintptr_t heap) {
  /**
   * Code page to use during conversion.
   */
  uint32_t codePage = localeStrings->A.CodePage;

  /**
   * When formattig locale string in CRT format, we want to allow best-fit
   * conversion.
   *
   * Locale strings in Windows and ISO formats must contain only ASCII
   * characters, and as such, best-fit conversion is not allowed.
   */
  bool bestFit = (localeStrings->Format == LOCALE_STRING_FORMAT_CRT);

  if (p32_private_wcstombs (&localeStrings->A.LcAll, localeStrings->W.LcAll, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&localeStrings->A.LcCollate, localeStrings->W.LcCollate, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&localeStrings->A.LcCtype, localeStrings->W.LcCtype, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&localeStrings->A.LcMessages, localeStrings->W.LcMessages, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&localeStrings->A.LcMonetary, localeStrings->W.LcMonetary, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&localeStrings->A.LcNumeric, localeStrings->W.LcNumeric, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&localeStrings->A.LcTime, localeStrings->W.LcTime, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

bool p32_localestr_format (LocaleStrings *localeStrings, uintptr_t heap, WindowsLocale *winLocale) {
  /**
   * Locale string format to use.
   */
  LocaleStringFormat format = localeStrings->Format;

  /**
   * Code page to use in formatted locale string.
   */
  uint32_t codePage = localeStrings->W.CodePage;

  if (!P32FormatLocaleString (&localeStrings->W.LcCollate, heap, &winLocale->LcCollate, codePage, format)) {
    goto fail;
  }

  if (!P32FormatLocaleString (&localeStrings->W.LcCtype, heap, &winLocale->LcCtype, codePage, format)) {
    goto fail;
  }

  if (!P32FormatLocaleString (&localeStrings->W.LcMessages, heap, &winLocale->LcMessages, codePage, format)) {
    goto fail;
  }

  if (!P32FormatLocaleString (&localeStrings->W.LcMonetary, heap, &winLocale->LcMonetary, codePage, format)) {
    goto fail;
  }

  if (!P32FormatLocaleString (&localeStrings->W.LcNumeric, heap, &winLocale->LcNumeric, codePage, format)) {
    goto fail;
  }

  if (!P32FormatLocaleString (&localeStrings->W.LcTime, heap, &winLocale->LcTime, codePage, format)) {
    goto fail;
  }

  /**
   * If `localeStrings->Mask == LC_ALL`, then all locale categories use the
   * same locale.
   *
   * In this case we can store single locale string in `localeStrings->W.LcAll`.
   *
   * However, we do not want to do it if we're formatting locale string for use
   * with CRT's `[_w]setlocale`.
   */
  if (localeStrings->Mask == LC_ALL && format != LOCALE_STRING_FORMAT_CRT) {
    if (!P32FormatLocaleString (&localeStrings->W.LcAll, heap, &winLocale->LcCtype, codePage, format)) {
      goto fail;
    }
  } else {
    if (!P32FormatLcAllLocaleString (&localeStrings->W, heap)) {
      goto fail;
    }
  }

  if (!P32ConverLocaleStrings (localeStrings, heap)) {
    goto fail;
  }

  return true;

fail:
  p32_localestr_free (localeStrings, heap);

  return false;
}
