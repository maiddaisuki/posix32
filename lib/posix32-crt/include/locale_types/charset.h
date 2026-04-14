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
#define P32_CHARSET_EBCDIC (1 << 2)

/**
 * A SBCS code page where all 256 bytes are valid characters.
 */
#define P32_CHARSET_FULL (1 << 4)

/**
 * Do not allow best-fit conversion.
 *
 * This flag causes `p32_charset_convert` to behave as if
 * `P32_CHARSET_CONVERSION_NO_BEST_FIT` flag is always set.
 */
#define P32_CHARSET_CONV_NO_BEST_FIT (1 << 8)

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

/**
 * Use code page specified in `CodePage` field of `CharsetConversionRequest`
 * structure to perform conversion.
 *
 * If this flag is not set, then information pointed by `Charset` field is
 * used instead.
 *
 * This flag should be used with caution since it adds an extra call to
 * `p32_charset_info` to fill in `Charset` structure for `CodePage`.
 */
#define P32_CHARSET_CONVERSION_CP     (1)
/**
 * Use `malloc` for memory allocations.
 *
 * If this flag is not set, then `p32_charset_convert` and functions it calls
 * internally will use `HeapAlloc` for memory allocations.
 */
#define P32_CHARSET_CONVERSION_MALLOC (1 << 1)

/**
 * Convert multibyte character string pointed by `Input.A` to wide character
 * string.
 */
#define P32_CHARSET_CONVERSION_MB_TO_WC (1 << 8)

/**
 * Convert wide character string pointed by `Input.W` to multibyte character
 * string.
 */
#define P32_CHARSET_CONVERSION_WC_TO_MB    (1 << 16)
/**
 * Do not allow best-fit conversion of `Input.W`.
 *
 * This flag only has effect when used with `P32_CHARSET_CONVERSION_WC_TO_MB`.
 * Otherwise, this flag is ignored.
 */
#define P32_CHARSET_CONVERSION_NO_BEST_FIT (1 << 17)

/**
 * These values are stored in `Status` field of `CharsetConversionRequest`
 * structure to provide additional information about call to
 * `p32_charset_convert` function.
 */
typedef enum CharsetConversionRequestStatus {
  /**
   * Set on success.
   */
  CharsetConversionRequestSuccess,
  /**
   * Set on failure.
   *
   * This condition may occur if `CodePage` is not a valid code page number.
   */
  CharsetConversionRequestFailure,
  /**
   * Set on failure.
   *
   * With `P32_CHARSET_CONVERSION_MB_TO_WC` this may happen if `Input.A`
   * containsan invalid multibyte sequence or unassigned code points.
   *
   * With `P32_CHARSET_CONVERSION_WC_TO_MB` this may happen if `Input.W`
   * contains invalid UTF-16 or cannot be converted to target code page.
   */
  CharsetConversionRequestNoConversion,
  /**
   * Set on failure.
   *
   * This condition indicates failure to allocate memory required to
   * perform conversion.
   */
  CharsetConversionRequestOutOfMemory,
  /**
   * Set on failure.
   *
   * This condition occurs when buffer required to hold converted string
   * exceeds `INT_MAX` code units.
   */
  CharsetConversionRequestTooBig,
} CharsetConversionRequestStatus;

/**
 * Structure used with `p32_charset_convert` function.
 */
typedef struct CharsetConversionRequest {
  /**
   * Combination of `P32_CHARSET_CONVERSION_*` flags.
   *
   * One and only one of `P32_CHARSET_CONVERSION_MB_TO_WC` and
   * `P32_CHARSET_CONVERSION_WC_TO_MB` must be set.
   */
  uint32_t Flags;
  /**
   * When `p32_charset_convert` succeeds, this field is set to
   * `CharsetConversionRequestSuccess`.
   *
   * Otherwise, this filed is set to any other value to provide additional
   * details why call to `p32_charset_convert` has failed.
   */
  CharsetConversionRequestStatus Status;
  /**
   * Code page to use during conversion.
   *
   * If `P32_CHARSET_CONVERSION_CP` flag is set, then `CodePage` must be set
   * to code page number.
   *
   * Otherwise, `Charset` must point to a valid `Charset` structure which
   * contains information filled in by `p32_charset_info` function.
   */
  union {
    uint32_t CodePage;
    Charset *Charset;
  };
  /**
   * Input field.
   *
   * This field must point to string to convert.
   */
  union {
    const char    *A;
    const wchar_t *W;
  } Input;
  /**
   * Output field.
   *
   * This field will recieve pointer to the buffer allocated to hold
   * the converted string.
   */
  union {
    char    **A;
    wchar_t **W;
  } Output;
} CharsetConversionRequest;

/**
 * Perform conversion according to `request`.
 *
 * The `request` argument must point to a properly initialized
 * `CharsetConversionRequest` structure.
 *
 * On success, returns length, excluding terminating NUL, of the converted
 * string the address of which is stored in variable pointed by
 * `request->Output`.
 *
 * On failure, returns `-1` and sets `request->Status` to provide additional
 * information about the cause.
 */
P32_TEST_DECL int p32_charset_convert (CharsetConversionRequest *request, uintptr_t heap);

#ifdef LIBPOSIX32_TEST
/**
 * Callback used with `p32_charset_enum_system_code_pages`.
 */
typedef bool (*EnumSystemCodePagesCallback) (uint32_t, void *);

/**
 * Call `callback` once for each code page supported by the operating system,
 * as long as it returns `true`.
 *
 * The `data` argument is passed as second argument to `callback`.
 *
 * This function is a portability wrapper for `EnumSystemCodePages`.
 */
P32_TEST_DECL void p32_charset_enum_system_code_pages (EnumSystemCodePagesCallback callback, void *data);
#endif

#endif /* LIBPOSIX32_CHARSET_H_INCLUDED */
