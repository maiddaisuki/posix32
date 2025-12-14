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
#include <stdio.h>  /* EOF */
#include <stdlib.h> /* _countof */
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "ctype-internal.h"
#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to gather locale information dependant on
 * LC_CTYPE locale category.
 */

/**
 * Supported character classes.
 */
static const CharTypeW CharTypes[16] = {
  {L"alnum",  P32_CHARTYPE_ALNUM  },
  {L"alpha",  P32_CHARTYPE_ALPHA  },
  {L"blank",  P32_CHARTYPE_BLANK  },
  {L"cntrl",  P32_CHARTYPE_CONTROL},
  {L"digit",  P32_CHARTYPE_DIGIT  },
  {L"graph",  P32_CHARTYPE_GRAPH  },
  {L"lower",  P32_CHARTYPE_LOWER  },
  {L"print",  P32_CHARTYPE_PRINT  },
  {L"punct",  P32_CHARTYPE_PUNCT  },
  {L"space",  P32_CHARTYPE_SPACE  },
  {L"upper",  P32_CHARTYPE_UPPER  },
  {L"xdigit", P32_CHARTYPE_XDIGIT },
  {L"",       0                   },
  {L"",       0                   },
  {L"",       0                   },
  {L"",       0                   },
};

/**
 * Supported character mappings.
 */
static const CharMappingW CharMappings[16] = {
  {L"tolower",           P32_WCTRANS_POSIX_LOWER   },
  {L"toupper",           P32_WCTRANS_POSIX_UPPER   },
  {L"to-ja-katakana",    P32_WCTRANS_JA_KATAKANA   },
  {L"to-ja-hiragana",    P32_WCTRANS_JA_HIRAGANA   },
  {L"to-zh-simplified",  P32_WCTRANS_ZH_SIMPLIFIED },
  {L"to-zh-traditional", P32_WCTRANS_ZH_TRADITIONAL},
  {L"",                  0                         },
  {L"",                  0                         },
  {L"",                  0                         },
  {L"",                  0                         },
  {L"",                  0                         },
  {L"",                  0                         },
  {L"",                  0                         },
  {L"",                  0                         },
  {L"",                  0                         },
  {L"",                  0                         },
};

/**
 * Get locale information dependant on `LC_CTYPE` locale category.
 */
static bool P32LcCtypeInfo (LcCtypeInfo *lcCtypeInfo, uintptr_t heap, Charset *charset) {
  if (!p32_charset_name (&lcCtypeInfo->CharsetName.W, heap, charset->CodePage)) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcCtypeInfoW (LcCtypeInfo *dest, uintptr_t heap, LcCtypeInfo *src) {
  if (p32_private_wcsdup (&dest->CharsetName.W, src->CharsetName.W, heap) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Free locale information stored in `lcCtypeInfo`.
 */
static void P32FreeLcCtypeInfoW (LcCtypeInfo *lcCtypeInfo, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (lcCtypeInfo->CharsetName.W != NULL) {
    HeapFree (heapHandle, 0, lcCtypeInfo->CharsetName.W);
    lcCtypeInfo->CharsetName.W = NULL;
  }
}

/**
 * Convert locale information in `lcCtypeInfo` to `codePage`.
 */
static bool P32ConvertLcCtypeInfo (LcCtypeInfo *lcCtypeInfo, uintptr_t heap, Charset *charset) {
  /**
   * TODO: should we use `codePage` or ASCII?
   */
  uint32_t codePage = charset->CodePage;

  if (p32_private_wcstombs (&lcCtypeInfo->CharsetName.A, lcCtypeInfo->CharsetName.W, heap, codePage, false) == -1) {
    goto fail;
  }

  for (size_t i = 0; i < _countof (CharTypes); ++i) {
    if (p32_private_wcstombs (&lcCtypeInfo->CharTypes[i].Name, CharTypes[i].Name, heap, codePage, false) == -1) {
      goto fail;
    }

    lcCtypeInfo->CharTypes[i].CharType = CharTypes[i].CharType;
  }

  for (size_t i = 0; i < _countof (CharMappings); ++i) {
    if (p32_private_wcstombs (&lcCtypeInfo->CharMappings[i].Name, CharMappings[i].Name, heap, codePage, false) == -1) {
      goto fail;
    }

    lcCtypeInfo->CharMappings[i].CharMapping = CharMappings[i].CharMapping;
  }

  return true;

fail:
  return false;
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcCtypeInfoA (LcCtypeInfo *dest, uintptr_t heap, LcCtypeInfo *src) {
  if (p32_private_strdup (&dest->CharsetName.A, src->CharsetName.A, heap) == -1) {
    goto fail;
  }

  for (size_t i = 0; i < _countof (src->CharTypes); ++i) {
    if (p32_private_strdup (&dest->CharTypes[i].Name, src->CharTypes[i].Name, heap) == -1) {
      goto fail;
    }

    dest->CharTypes[i].CharType = src->CharTypes[i].CharType;
  }

  for (size_t i = 0; i < _countof (src->CharMappings); ++i) {
    if (p32_private_strdup (&dest->CharMappings[i].Name, src->CharMappings[i].Name, heap) == -1) {
      goto fail;
    }

    dest->CharMappings[i].CharMapping = src->CharMappings[i].CharMapping;
  }

  return true;

fail:
  return false;
}

/**
 * Free locale information stored in `lcCtypeInfo`.
 */
static void P32FreeLcCtypeInfoA (LcCtypeInfo *lcCtypeInfo, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (lcCtypeInfo->CharsetName.A != NULL) {
    HeapFree (heapHandle, 0, lcCtypeInfo->CharsetName.A);
    lcCtypeInfo->CharsetName.A = NULL;
  }

  for (size_t i = 0; i < _countof (lcCtypeInfo->CharTypes); ++i) {
    if (lcCtypeInfo->CharTypes[i].Name != NULL) {
      HeapFree (heapHandle, 0, lcCtypeInfo->CharTypes[i].Name);
      lcCtypeInfo->CharTypes[i].Name = NULL;
    }

    lcCtypeInfo->CharTypes[i].CharType = 0;
  }

  for (size_t i = 0; i < _countof (lcCtypeInfo->CharMappings); ++i) {
    if (lcCtypeInfo->CharMappings[i].Name != NULL) {
      HeapFree (heapHandle, 0, lcCtypeInfo->CharMappings[i].Name);
      lcCtypeInfo->CharMappings[i].Name = NULL;
    }

    lcCtypeInfo->CharMappings[i].CharMapping = 0;
  }
}

/**
 * Fill in `asciiMap`.
 */
static bool P32AsciiMap (AsciiMap *asciiMap, locale_t locale) {
  for (wint_t wc = 0; wc < 0x80; ++wc) {
    int c = locale->Functions.F_wctob (wc, &locale->Charset);

    /**
     * Code pages 10021 and 20269 cannot represent ASCII character 0x7F (DEL).
     */
    assert (c != EOF || wc == 0x7F);

    asciiMap->Map[wc] = (char) c;
  }

  return true;
}

/**
 * Fill in `charTypeMap`.
 */
static bool P32CharTypeMap (CharTypeMap *charTypeMap, locale_t locale) {
  for (int i = 0; i < 256; ++i) {
    wchar_t wc = locale->Functions.F_btowc (i, &locale->Charset);

    if (wc == WEOF) {
      charTypeMap->Map[i] = 0;
      continue;
    }

    charTypeMap->Map[i] = (uint16_t) p32_iswctype_l (wc, P32_CHARTYPE_ALL, locale);
  }

  return true;
}

/**
 * Fill in `lowerMap`.
 */
static bool P32ToLowerMap (LowerMap *lowerMap, locale_t locale) {
  for (int i = 0; i < 256; ++i) {
    wchar_t wc = locale->Functions.F_btowc (i, &locale->Charset);

    if (wc == WEOF) {
      lowerMap->Map[i] = (uint8_t) i;
      continue;
    }

    wchar_t wcLower = p32_towlower_l (wc, locale);

    if (wc == wcLower) {
      lowerMap->Map[i] = (uint8_t) i;
      continue;
    }

    int cLower = locale->Functions.F_wctob (wcLower, &locale->Charset);

    if (cLower == EOF) {
      lowerMap->Map[i] = (uint8_t) i;
      continue;
    }

    lowerMap->Map[i] = (uint8_t) cLower;
  }

  return true;
}

/**
 * Fill in `upperMap`.
 */
static bool P32ToUpperMap (UpperMap *upperMap, locale_t locale) {
  for (int i = 0; i < 256; ++i) {
    wchar_t wc = locale->Functions.F_btowc (i, &locale->Charset);

    if (wc == WEOF) {
      upperMap->Map[i] = (uint8_t) i;
      continue;
    }

    wchar_t wcUpper = p32_towupper_l (wc, locale);

    if (wc == wcUpper) {
      upperMap->Map[i] = (uint8_t) i;
      continue;
    }

    int cUpper = locale->Functions.F_wctob (wcUpper, &locale->Charset);

    if (cUpper == EOF) {
      upperMap->Map[i] = (uint8_t) i;
      continue;
    }

    upperMap->Map[i] = (uint8_t) cUpper;
  }

  return true;
}

bool p32_localeinfo_ctype (locale_t locale, uintptr_t heap) {
  Locale      *lcCtype     = &locale->WinLocale.LcCtype;
  LcCtypeInfo *lcCtypeInfo = &locale->LocaleInfo.LcCtype;
  Charset     *charset     = &locale->Charset;

  /**
   * Use filesystem casing rules with "POSIX" ("C") locale.
   */
  if (lcCtype->Type == LOCALE_TYPE_POSIX) {
    lcCtypeInfo->WcTransFlags = 0;
    lcCtypeInfo->CaseCmpFlags = 0;
  } else {
    lcCtypeInfo->WcTransFlags = LCMAP_LINGUISTIC_CASING;
    lcCtypeInfo->CaseCmpFlags = LINGUISTIC_IGNORECASE | NORM_LINGUISTIC_CASING | SORT_STRINGSORT;
  }

  if (!P32LcCtypeInfo (lcCtypeInfo, heap, charset)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"%s(LC_CTYPE): failed to obtain locale information\n", locale->WindowsLocaleStrings.W.LcCtype);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail;
  }

  if (!P32ConvertLcCtypeInfo (lcCtypeInfo, heap, charset)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (
      _CRT_ERROR, L"%s(LC_CTYPE): failed to convert locale information\n", locale->WindowsLocaleStrings.W.LcCtype
    );

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_convert;
  }

  if (!P32AsciiMap (&lcCtypeInfo->AsciiMap, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"%s(LC_CTYPE): failed to create AsciiMap\n", locale->WindowsLocaleStrings.W.LcCtype);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_ascii_map;
  }

  if (!P32CharTypeMap (&lcCtypeInfo->CharType, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"%s(LC_CTYPE): failed to create CharTypeMap\n", locale->WindowsLocaleStrings.W.LcCtype);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_chartype_map;
  }

  if (!P32ToLowerMap (&lcCtypeInfo->ToLower, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"%s(LC_CTYPE): failed to create LowerMap\n", locale->WindowsLocaleStrings.W.LcCtype);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_lower_map;
  }

  if (!P32ToUpperMap (&lcCtypeInfo->ToUpper, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"%s(LC_CTYPE): failed to create UpperMap\n", locale->WindowsLocaleStrings.W.LcCtype);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_upper_map;
  }

  return true;

fail_upper_map:
  memset (lcCtypeInfo->ToUpper.Map, 0, sizeof (UpperMap));
fail_lower_map:
  memset (lcCtypeInfo->ToLower.Map, 0, sizeof (LowerMap));
fail_chartype_map:
  memset (lcCtypeInfo->CharType.Map, 0, sizeof (CharTypeMap));
fail_ascii_map:
  memset (lcCtypeInfo->AsciiMap.Map, 0, sizeof (AsciiMap));
fail_convert:
  P32FreeLcCtypeInfoA (lcCtypeInfo, heap);
fail:
  P32FreeLcCtypeInfoW (lcCtypeInfo, heap);

  lcCtypeInfo->WcTransFlags = 0;
  lcCtypeInfo->CaseCmpFlags = 0;

  return false;
}

bool p32_localeinfo_ctype_copy (LcCtypeInfo *dest, uintptr_t heap, LcCtypeInfo *src) {
  if (!P32CopyLcCtypeInfoW (dest, heap, src)) {
    goto fail_w;
  }

  if (!P32CopyLcCtypeInfoA (dest, heap, src)) {
    goto fail_a;
  }

  memcpy (dest->AsciiMap.Map, src->AsciiMap.Map, sizeof (AsciiMap));
  memcpy (dest->CharType.Map, src->CharType.Map, sizeof (CharTypeMap));
  memcpy (dest->ToLower.Map, src->ToLower.Map, sizeof (LowerMap));
  memcpy (dest->ToUpper.Map, src->ToUpper.Map, sizeof (UpperMap));

  dest->WcTransFlags = src->WcTransFlags;
  dest->CaseCmpFlags = src->CaseCmpFlags;

  return true;

fail_a:
  P32FreeLcCtypeInfoA (dest, heap);
fail_w:
  P32FreeLcCtypeInfoW (dest, heap);

  return false;
}

void p32_localeinfo_ctype_free (LcCtypeInfo *lcCtypeInfo, uintptr_t heap) {
  P32FreeLcCtypeInfoA (lcCtypeInfo, heap);
  P32FreeLcCtypeInfoW (lcCtypeInfo, heap);
  memset (lcCtypeInfo->AsciiMap.Map, 0, sizeof (lcCtypeInfo->AsciiMap.Map));
  memset (lcCtypeInfo->CharType.Map, 0, sizeof (lcCtypeInfo->CharType.Map));
  memset (lcCtypeInfo->ToLower.Map, 0, sizeof (lcCtypeInfo->ToLower.Map));
  memset (lcCtypeInfo->ToUpper.Map, 0, sizeof (lcCtypeInfo->ToUpper.Map));
  lcCtypeInfo->WcTransFlags = 0;
  lcCtypeInfo->CaseCmpFlags = 0;
}
