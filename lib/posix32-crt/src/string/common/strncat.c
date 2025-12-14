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

#include "string-internal.h"

static char *p32_strncat_sbcs (char *dest, const char *src, size_t count, locale_t locale) {
  return strncat (dest, src, count);
  UNREFERENCED_PARAMETER (locale);
}

char *p32_private_strncat_l (char *dest, const char *src, size_t count, locale_t locale) {
  size_t destLength = p32_private_strlen_l (dest, locale);

  if (destLength == (size_t) -1) {
    return NULL;
  }

  size_t srcLength = p32_private_strnlen_l (src, count, locale);

  if (srcLength == (size_t) -1) {
    return NULL;
  }

  /**
   * Pointer to `dest`'s terminating '\0'.
   */
  char *destTail = dest + destLength;

  memcpy (destTail, src, srcLength);
  destTail[srcLength] = '\0';

  return dest;
}

static void P32LocaleFunction_strncat (LocaleFunctions *functions, Charset *charset, Locale *locale) {
  functions->F_strncat = p32_strncat_sbcs;
  return;
  UNREFERENCED_PARAMETER (charset);
  UNREFERENCED_PARAMETER (locale);
}

char *p32_strncat (char *dest, const char *src, size_t count) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return activeLocale->Functions.F_strncat (dest, src, count, activeLocale);
}
