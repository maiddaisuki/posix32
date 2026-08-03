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

#include "core-charset.h"

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
 * Normalize string to form C before converting it.
 */
#define P32_CHARSET_CONVERSION_NORM_C  (1 << 24)
/**
 * Normalize string to form D before converting it.
 */
#define P32_CHARSET_CONVERSION_NORM_D  (1 << 25)
/**
 * Normalize string to form KC before converting it.
 */
#define P32_CHARSET_CONVERSION_NORM_KC (1 << 26)
/**
 * Normalize string to form KD before converting it.
 */
#define P32_CHARSET_CONVERSION_NORM_KD (1 << 27)

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

#endif /* LIBPOSIX32_CHARSET_H_INCLUDED */
