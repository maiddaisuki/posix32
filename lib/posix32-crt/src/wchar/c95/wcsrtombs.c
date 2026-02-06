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

#include "wchar-internal.h"

size_t p32_private_wcsrtombs_l (
  char *P32_RESTRICT           mbs,
  const wchar_t **P32_RESTRICT wcs,
  size_t                       size,
  mbstate_t *P32_RESTRICT      state,
  locale_t                     locale
) {
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
    const size_t length = locale->Functions.F_c16rtomb (buffer, wc[0], &newState, &locale->Charset);

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

static void P32LocaleFunction_wcsrtombs (LocaleFunctions *functions, Charset *charset) {
  functions->F_wcsrtombs = p32_private_wcsrtombs_l;
  UNREFERENCED_PARAMETER (charset);
}

/**
 * Private `mbstate_t` to use if caller did not supply one.
 */
static mbstate_t P32ConversionState_wcsrtombs = {0};

size_t p32_wcsrtombs_l (char *mbs, const wchar_t **wcs, size_t count, mbstate_t *state, locale_t locale) {
  if (state == NULL) {
    state = &P32ConversionState_wcsrtombs;
  }

  return locale->Functions.F_wcsrtombs (mbs, wcs, count, state, locale);
}

size_t p32_wcsrtombs (char *mbs, const wchar_t **wcs, size_t count, mbstate_t *state) {
  locale_t locale = p32_active_locale ();

  if (locale == NULL) {
    locale = p32_default_locale ();
  }

  return p32_wcsrtombs_l (mbs, wcs, count, state, locale);
}
