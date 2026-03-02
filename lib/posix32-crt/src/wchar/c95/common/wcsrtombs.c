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

/**
 * File Summary:
 *
 * This file contains generic implementation of `wcsrtombs` function.
 */

size_t wcsrtombs (
  char *P32_RESTRICT           mbs,
  const wchar_t **P32_RESTRICT wcs,
  size_t                       size,
  mbstate_t *P32_RESTRICT      state,
  Charset *P32_RESTRICT        charset
) {
  assert (state != NULL);

  /**
   * Internal conversion state.
   */
  mbstate_t conversionState = *state;

  /**
   * Total number of bytes written to `mbs`.
   */
  size_t mbConverted = 0;

  /**
   * Buffer to hold converted multibyte character.
   */
  char buffer[MB_LEN_MAX];

  /**
   * Next wide character to convert.
   */
  const wchar_t *wc = *wcs;

  while (1) {
    /**
     * See wcsnrtombs.c for details.
     */
    mbstate_t newState = conversionState;

    /**
     * `length` may be 0 if `wc[0]` is UTF-16 High Surrogate.
     */
    const size_t length = c16rtomb (buffer, wc[0], &newState, charset);

    /**
     * `wc` points to invalid wide character or `state` describes invalid
     * conversion state.
     */
    if (length == (size_t) -1) {
      if (mbs != NULL) {
        *wcs = wc;
      }

      return (size_t) -1;
    }

    /**
     * We have consumed UTF-16 High Surrogate from `wcs`.
     */
    if (length == 0) {
      assert (!p32_mbsinit (&newState));

      conversionState  = newState;
      wc              += 1;

      continue;
    }

    /**
     * Storing `length` bytes in `mbs` would exceed `size`.
     */
    if (mbs != NULL && mbConverted + length > size) {
      *wcs = wc;
      break;
    }

    /**
     * Copy converted multibyte character to `mbs`.
     */
    if (mbs != NULL) {
      memcpy (mbs, buffer, length);
      mbs += length;
    }

    /**
     * Terminating '\0' has been converted.
     */
    if (buffer[0] == '\0') {
      assert (length == 1);

      if (mbs != NULL) {
        *wcs = NULL;
      }

      assert (p32_mbsinit (&conversionState));
      break;
    }

    conversionState  = newState;
    mbConverted     += length;
    wc              += 1;

    /**
     * `size` bytes have been written to `mbs`.
     */
    if (mbs != NULL && mbConverted == size) {
      *wcs = wc;
      break;
    }
  }

  /**
   * Update user-supplied conversion state.
   */
  if (mbs != NULL) {
    *state = conversionState;
  }

  return mbConverted;
}
