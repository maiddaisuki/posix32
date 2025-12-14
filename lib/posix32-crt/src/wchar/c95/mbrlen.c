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

size_t p32_private_mbrlen_l (
  const char *P32_RESTRICT mbs,
  size_t                   count,
  mbstate_t *P32_RESTRICT  state,
  locale_t                 locale
) {
  assert (state != NULL);
  return p32_private_mbrtowc_l (NULL, mbs, count, state, locale);
}

static void P32LocaleFunction_mbrlen (LocaleFunctions *functions, Charset *charset) {
  functions->F_mbrlen = p32_private_mbrlen_l;
  UNREFERENCED_PARAMETER (charset);
}

/**
 * Private `mbstate_t` to use if caller did not supply one.
 */
static mbstate_t P32ConversionState_mbrlen = {0};

size_t p32_mbrlen_l (const char *mbs, size_t count, mbstate_t *state, locale_t locale) {
  if (state == NULL) {
    state = &P32ConversionState_mbrlen;
  }

  return locale->Functions.F_mbrlen (mbs, count, state, locale);
}

size_t p32_mbrlen (const char *mbs, size_t count, mbstate_t *state) {
  locale_t locale = p32_active_locale ();

  if (locale == NULL) {
    locale = p32_default_locale ();
  }

  return p32_mbrlen_l (mbs, count, state, locale);
}
