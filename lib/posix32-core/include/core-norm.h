/**
 * Copyright 2026 Kirill Makurin
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

#ifndef LIBPOSIX32_CORE_NORM_H_INCLUDED
#define LIBPOSIX32_CORE_NORM_H_INCLUDED

#include <stdint.h>
#include <wchar.h>

#include "posix32.h"

/**
 * Normalization forms.
 */
typedef enum NormForm {
  /**
   * Canonical composition.
   */
  NormForm_C,
  /**
   * Canonical decomposition.
   */
  NormForm_D,
  /**
   * Compatibility composition.
   */
  NormForm_KC,
  /**
   * Compatibility decomposition.
   */
  NormForm_KD,
} NormForm;

/**
 * Use `malloc` for memory allocations.
 *
 * Otherwise, `p32_normalize_unicode_string` will use `HeapAlloc` for
 * memory allocations. This is the default behavior.
 */
#define P32_NORMALIZATION_REQUEST_MALLOC (1)

/**
 * Status codes reported by `p32_normalize_unicode_string`;
 * they are stored in `NormalizationRequest::Status`.
 */
typedef enum NormalizationRequestStatus {
  /**
   * Set on success.
   */
  NormalizationRequestSuccess,
  /**
   * Set on failure.
   */
  NormalizationRequestFailure,
  /**
   * Set on failure.
   *
   * This status code indicates that string contains invalid Unicode.
   */
  NormalizationRequestInvalidUnicode,
  /**
   * This status code indicates that no normalization was performed because
   * string was already normalized.
   *
   * This status code is set when `p32_normalize_unicode_string` returns `-1`.
   * This status code must always be checked for and handled appropriately.
   */
  NormalizationRequestNormalized,
  /**
   * Set on failure.
   *
   * This status code indicates that requested normalization is not available.
   * This status code must always be checked for and handled appropriately.
   *
   * For example, this status code is always returned on Windows 9x systems
   * which lack Unicode support.
   *
   * This status code may also be returned on pre-Vista systems.
   */
  NormalizationRequestNotSupported,
  /**
   * Set on failure.
   *
   * This status code indicates failure to allocate buffer to hold
   * the normalized string.
   */
  NormalizationRequestOutOfMemory,
} NormalizationRequestStatus;

/**
 * Structure used with `p32_normalize_unicode_string`.
 */
typedef struct NormalizationRequest {
  /**
   * Normalization form.
   */
  NormForm Form;
  /**
   * Combination of `P32_NORMALIZATION_REQUEST_*` flags.
   */
  uint32_t Flags;
  /**
   * Status code returned by `p32_normalize_unicode_string`.
   */
  NormalizationRequestStatus Status;
  /**
   * Input field.
   *
   * String to normalize.
   */
  const wchar_t *Input;
  /**
   * Output field.
   *
   * On success, this field will recieve pointer to the buffer allocated to
   * hold the normalized string.
   */
  wchar_t **Output;
} NormalizationRequest;

/**
 * Normalize Unicode string according to `request`.
 *
 * The `request` must point to a properly initialized `NormalizationRequest`
 * structure.
 *
 * On success, returns length, excluding terminating NUL, of the normalized
 * string the address of which is stored in variable pointed by
 * `request->Output`.
 *
 * On failure, returns `-1` and sets `request->Status` to provide additional
 * information about the cause.
 */
P32_CORE_DECL int p32_normalize_unicode_string (NormalizationRequest *request, uintptr_t heap);

#endif /* LIBPOSIX32_CORE_NORM_H_INCLUDED */
