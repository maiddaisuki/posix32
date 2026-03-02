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
 * This file contains generic implementation of `wcrtomb` function.
 */

size_t p32_private_wcrtomb_l (char *P32_RESTRICT mbc, wchar_t wc, mbstate_t *P32_RESTRICT state, locale_t locale) {
  assert (state != NULL);

  size_t length = locale->Functions.F_c16rtomb (mbc, wc, state, &locale->Charset);

  /**
   * `wc` is invalid wide character or `state` describes invalid
   * conversion state.
   */
  if (length == (size_t) -1) {
    return (size_t) -1;
  }

  /**
   * We have consumed high surrogate of an UTF-16 Code Unit Sequence.
   *
   * Unlike `c16rtomb`, `wcrtomb` provides no mechanism to handle UTF-16
   * surrogate pairs. CRT's `wcrtomb` fails in this case.
   *
   * Instead of failing right away, we consume high surrogate and produce
   * multibyte sequence stored in `locale->Charset.ReplacementChar`.
   *
   * For anything other than UTF-8 this is '?' character.
   * For UTF-8 this is UTF-8 Code Unit Sequence for U+FFFD.
   *
   * For anything other than UTF-8, the following call that consumes
   * low surrogate will fail, since there is no SBCS/DBCS code page which is
   * able to represent characters encoded with UTF-16 Surrogate Pairs.
   * For UTF-8, this call will produce valid UTF-8 Code Unit Sequence.
   */
  if (length == 0) {
    assert (!p32_mbsinit (state));
    assert (mbc != NULL);

    memcpy (mbc, locale->Charset.ReplacementChar.Char, locale->Charset.ReplacementChar.Length);
    return locale->Charset.ReplacementChar.Length;
  }

  return length;
}
