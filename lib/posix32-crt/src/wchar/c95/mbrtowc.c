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

size_t p32_private_mbrtowc_l (
  wchar_t *P32_RESTRICT    wc,
  const char *P32_RESTRICT mbc,
  size_t                   count,
  mbstate_t *P32_RESTRICT  state,
  locale_t                 locale
) {
  assert (state != NULL);

  /**
   * Converted wide character.
   *
   * Unlike `mbrtoc16`, `mbrtowc` cannot handle UTF-16 Surrogate Pairs.
   *
   * If we consumed multibyte sequence which would produce UTF-16
   * Surrogate Pair, we must ensure that we do not produce invalid UTF-16
   * by writing only the low or high surrogate to `wc`.
   *
   * CRT's `mbrtowc` stores U+FFFD in such case and we follow this behavior.
   */
  wchar_t u16[2] = {WEOF, WEOF};

  const size_t length = locale->Functions.F_mbrtoc16 (&u16[0], mbc, count, state, &locale->Charset);

  /**
   * `state` is conversion state from `mbrtoc8`, `mbrtoc16` or `mbrtoc32`.
   */
  if (length == (size_t) -3) {
    _set_errno (EINVAL);
    return (size_t) -1;
  }

  /**
   * `mbc` points to invalid or incomplete character.
   */
  if (length == (size_t) -1 || length == (size_t) -2) {
    return length;
  }

  /**
   * Conversion produced single non-surrogate UTF-16 Code Unit.
   */
  if (p32_mbsinit (state)) {
    assert (!IS_HIGH_SURROGATE (u16[0]) && !IS_LOW_SURROGATE (u16[0]));

    /**
     * If `mbc` is `NULL`, then `wc` is ignored.
     */
    if (wc != NULL && mbc != NULL) {
      *wc = u16[0];
    }

    return length;
  }

  assert (mbc != NULL);

  /**
   * We have consumed multibyte sequence which would produce
   * UTF-16 Surrogate pair.
   *
   * Call `mbrtoc16` again to update `state`.
   * After this call `u16` must contain UTF-16 Surrogate Pair.
   *
   * If `wc` is not null, store U+FFFD.
   */
  size_t ret = locale->Functions.F_mbrtoc16 (&u16[1], "", 0, state, &locale->Charset);

  assert (ret == (size_t) -3);
  assert (p32_mbsinit (state));
  assert (IS_SURROGATE_PAIR (u16[0], u16[1]));

  if (wc != NULL) {
    *wc = 0xFFFD;
  }

  return length;
  UNREFERENCED_PARAMETER (ret);
}

static void P32LocaleFunction_mbrtowc (LocaleFunctions *functions, Charset *charset) {
  functions->F_mbrtowc = p32_private_mbrtowc_l;
  UNREFERENCED_PARAMETER (charset);
}

/**
 * Private `mbstate_t` to use if caller did not supply one.
 */
static mbstate_t P32ConversionState_mbrtowc = {0};

size_t p32_mbrtowc_l (wchar_t *wc, const char *mbc, size_t count, mbstate_t *state, locale_t locale) {
  if (state == NULL) {
    state = &P32ConversionState_mbrtowc;
  }

  return locale->Functions.F_mbrtowc (wc, mbc, count, state, locale);
}

size_t p32_mbrtowc (wchar_t *wc, const char *mbc, size_t count, mbstate_t *state) {
  locale_t locale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (locale == NULL) {
    locale = p32_posix_locale ();
  }
#endif

  return p32_mbrtowc_l (wc, mbc, count, state, locale);
}
