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

#ifndef LIBPOSIX32_CHARSET_H_INCLUDED
#define LIBPOSIX32_CHARSET_H_INCLUDED

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * A SBCS code page.
 */
#define P32_CHARSET_SBCS (1)

/**
 * A DBCS code page.
 */
#define P32_CHARSET_DBCS (1 << 1)

/**
 * An EBCDIC code page.
 */
#define P32_CHARSET_EBCDIC (1 << 8)

/**
 * A SBCS code page where all 256 bytes are valid characters.
 */
#define P32_CHARSET_FULL (1 << 9)

/**
 * Code page cannot represent all ASCII characters.
 */
#define P32_CHARSET_REJECT_ASCII (1 << 16)

/**
 * Code page can be set with `[_w]setlocale`, however conversion functions
 * fail to convert even simple ASCII strings.
 */
#define P32_CHARSET_REJECT_BROKEN (1 << 17)

/**
 * Code page cannot be set with `[_w]setlocale`, but otherwise completely
 * usable and supported.
 */
#define P32_CHARSET_REJECT_CRT (1 << 18)

/**
 * Code page cannot be set for Global Locale.
 */
#define P32_CHARSET_REJECT_GLOBAL (1 << 19)

/**
 * Code page is documented, however:
 *
 * 1. `IsValidCodePage` returns `FALSE`.
 * 2. `EnumSystemCodePages` does not enumerate them.
 */
#define P32_CHARSET_REJECT_LEGACY (1 << 20)

/**
 * Code page is available only to managed applications.
 */
#define P32_CHARSET_REJECT_MANAGED (1 << 21)

/**
 * Code page is not documented, but enumurated by `EnumSystemCodePages`.
 *
 * `GetCPInfo` may hang when called with one of these code pages.
 */
#define P32_CHARSET_REJECT_UNDOCUMENTED (1 << 22)

/**
 * Code page cannot be set with `[_w]setlocale`.
 */
#define P32_CHARSET_REJECT_UNSUPPORTED (1 << 23)

/**
 * Information about code page.
 */
typedef struct Charset {
  uint32_t CodePage;
  uint32_t Flags;
  uint32_t MaxLength;
  /**
   * DBCS: lead bytes.
   *
   * Contains up to five pairs of bytes, terminated by a pair of zero bytes.
   * Each pair is a range specifying lead bytes used by code page.
   */
  uint8_t Map[MAX_LEADBYTES];
  /**
   * Flags to use with `MultiByteToWideChar`.
   */
  uint32_t ToWideChar;
  /**
   * Flags to use with `WideCharToMultiByte`.
   */
  uint32_t ToMultiByte;
  /**
   * For use with `wcrtomb` and `wctomb`.
   */
  struct {
    /**
     * Length of `Char`.
     */
    uint8_t Length;
    char    Char[MB_LEN_MAX];
  } ReplacementChar;
} Charset;

/**
 * Return `true` if `codePage` is supported and `false` otherwise.
 *
 * `rejectMask` is combination of `P32_CHARSET_REJECT_*` flags which must be
 * rejected in addition to ones rejected by default.
 *
 * `allowMask` is combination of `P32_CHARSET_REJECT_*` flags which must be
 * allowed even if they are rejected by default.
 *
 * Note that `rejectMask` takes precedance over `allowMask`.
 */
P32_TEST_DECL bool p32_charset_usable (uint32_t codePage, int rejectMask, int allowMask);

/**
 * Get conversion flags to use with `charset->CodePage`.
 *
 * If `bestFit` is `true`, store conversion flags which will allow best-fit
 * conversion.
 */
P32_TEST_DECL void p32_charset_conversion_flags (Charset *charset, bool bestFit);

/**
 * Fill in `charset` with information about `charset->CodePage`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_charset_info (Charset *charset);

/**
 * Get character set name for `codePage`.
 *
 * With exception of a few code pages, returned string can be passed to
 * GNU libiconv's `iconv(3)`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
P32_TEST_DECL bool p32_charset_name (wchar_t **address, uintptr_t heap, uint32_t codePage);

#endif /* LIBPOSIX32_CHARSET_H_INCLUDED */
