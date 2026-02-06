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

#include "p32_stdlib.h"

static int p32_mblen_generic (const char *mbs, size_t count, locale_t locale) {
  /**
   * We do not support stateful encodings which allows us to use `mbstate_t`
   * object allocated on the stack. This effectively makes `mblen` thread safe.
   *
   * Note that unlike `mbtowc`, `mblen` does not accumulate incompelte
   * multibyte sequence in its private `mbstate_t` object.
   */
  mbstate_t state = {0};

  size_t length = p32_private_mbrlen_l (mbs, count, &state, locale);

  /**
   * `mbs` point to invalid or incomplete multibyte character.
   */
  if (length == (size_t) -1 || length == (size_t) -2) {
    return -1;
  }

  assert (length <= locale->Charset.MaxLength);
  return (int) length;
}

static void P32LocaleFunction_mblen (LocaleFunctions *functions, Charset *charset) {
  functions->F_mblen = p32_mblen_generic;
  return;
  UNREFERENCED_PARAMETER (charset);
}

int p32_mblen_l (const char *mbs, size_t count, locale_t locale) {
  return locale->Functions.F_mblen (mbs, count, locale);
}

int p32_mblen (const char *mbs, size_t count) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    return mblen (mbs, count);
  }
#endif

  return p32_mblen_l (mbs, count, activeLocale);
}
