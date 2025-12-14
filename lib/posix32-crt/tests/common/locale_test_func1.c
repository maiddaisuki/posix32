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

#if !P32_LOCALE_NAMES
static LocaleCallback1 callback1 = NULL;
#endif

#if P32_LOCALE_NAMES
static BOOL CALLBACK CallbackWrapper1 (LPWSTR localeString, DWORD flags, LPARAM param) {
  UNREFERENCED_PARAMETER (flags);
#else
static BOOL CALLBACK CallbackWrapper1 (LPWSTR localeString) {
  assert (callback1 != NULL);
#endif

  LocaleCallback1 callback = NULL;
  Locale          locale   = {0};

#if P32_LOCALE_NAMES
  locale.LocaleName = localeString;
  callback          = (LocaleCallback1) param;
#else
  locale.LocaleId = wcstoul (localeString, NULL, 16);
  callback        = callback1;
#endif

  return callback (&locale);
}

void p32_locale_test_func1 (LocaleCallback1 callback) {
#if P32_LOCALE_NAMES
  EnumSystemLocalesEx (CallbackWrapper1, LOCALE_ALL, (LPARAM) callback, NULL);
#else
  callback1 = callback;
  EnumSystemLocalesW (CallbackWrapper1, LCID_SUPPORTED);
#endif
}
