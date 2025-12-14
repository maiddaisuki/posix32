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

#include "wchar-internal.h"

size_t p32_private_mbsrtowcs_l (
  wchar_t *P32_RESTRICT     wcs,
  const char **P32_RESTRICT mbs,
  size_t                    size,
  mbstate_t *P32_RESTRICT   state,
  locale_t                  locale
) {
  assert (state != NULL);

  /**
   * Internal conversion state.
   */
  mbstate_t conversionState = *state;

  /**
   * Total number of wide characters written to `wcs`.
   */
  size_t wcConverted = 0;

  /**
   * Length of converted wide character.
   */
  size_t wcLength = 0;

  /**
   * Converted wide character.
   */
  wchar_t wc[2] = {WEOF, WEOF};

  /**
   * Next character to convert.
   */
  const char *mbc = *mbs;

  while (1) {
    /**
     * See mbsnrtowcs.c for details.
     */
    mbstate_t newState = conversionState;

    size_t length = locale->Functions.F_mbrtoc16 (&wc[0], mbc, locale->Charset.MaxLength, &newState, &locale->Charset);
    assert (length != (size_t) -2);

    /**
     * `state` contains conversion state from mbrtoc8, mbrtoc16 or mbrtoc32.
     */
    if (length == (size_t) -3) {
      goto einval;
    }

    /**
     * Conversion failed or `state` describes invalid conversion state.
     */
    if (length == (size_t) -1) {
      if (wcs != NULL) {
        *mbs = mbc;
      }

      return (size_t) -1;
    }

    /**
     * Single wide character has been stored in `u16[0]`.
     */
    if (p32_mbsinit (&newState)) {
      wcLength = 1;

      /**
       * UTF-16 High surrogate has been stored in `u16[0]`.
       */
    } else {
      size_t ret = locale->Functions.F_mbrtoc16 (&wc[1], "", 0, &newState, &locale->Charset);

      assert (ret == (size_t) -3);
      assert (p32_mbsinit (&newState));

      if (ret != (size_t) -3) {
        goto eilseq;
      }

      assert (IS_SURROGATE_PAIR (wc[0], wc[1]));
      wcLength = 2;
    }

    /**
     * Storing `wcLength` wide characters in `wcs` would exceed `size`.
     */
    if (wcs != NULL && wcConverted + wcLength > size) {
      *mbs = mbc;
      break;
    }

    if (wcs != NULL) {
      wmemcpy (wcs, wc, wcLength);
      wcs += wcLength;
    }

    /**
     * Termintaing '\0' has been converted.
     */
    if (length == 0) {
      assert (wc[0] == u'\0');

      if (wcs != NULL) {
        *mbs = NULL;
      }

      assert (p32_mbsinit (&conversionState));
      break;
    }

    conversionState  = newState;
    wcConverted     += wcLength;
    mbc             += length;

    /**
     * `size` wide characters have been written to `wcs`.
     */
    if (mbs != NULL && wcConverted == size) {
      *mbs = mbc;
      break;
    }
  }

  /**
   * Update user-supplied conversion state.
   */
  if (wcs != NULL) {
    *state = conversionState;
  }

  return wcConverted;

einval:
  _set_errno (EINVAL);
  return (size_t) -1;

eilseq:
  _set_errno (EILSEQ);
  return (size_t) -1;
}

static void P32LocaleFunction_mbsrtowcs (LocaleFunctions *functions, Charset *charset) {
  functions->F_mbsrtowcs = p32_private_mbsrtowcs_l;
  UNREFERENCED_PARAMETER (charset);
}

/**
 * Private `mbstate_t` to use if caller did not supply one.
 */
static mbstate_t P32ConversionState_mbsrtowcs = {0};

size_t p32_mbsrtowcs_l (wchar_t *wcs, const char **mbs, size_t count, mbstate_t *state, locale_t locale) {
  if (state == NULL) {
    state = &P32ConversionState_mbsrtowcs;
  }

  return locale->Functions.F_mbsrtowcs (wcs, mbs, count, state, locale);
}

size_t p32_mbsrtowcs (wchar_t *wcs, const char **mbs, size_t count, mbstate_t *state) {
  locale_t locale = p32_active_locale ();

  if (locale == NULL) {
    locale = p32_default_locale ();
  }

  return p32_mbsrtowcs_l (wcs, mbs, count, state, locale);
}
