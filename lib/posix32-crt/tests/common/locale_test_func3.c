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

static LocaleCallback3 localeCallback3 = NULL;

static BOOL CALLBACK P32CallbackWrapper3 (wchar_t *codePageString) {
  assert (localeCallback3 != NULL);

  uint32_t codePage = wcstoul (codePageString, NULL, 10);
  return localeCallback3 (codePage);
}

void p32_locale_test_func3 (LocaleCallback3 callback) {
  localeCallback3 = callback;
  EnumSystemCodePagesW (P32CallbackWrapper3, CP_SUPPORTED);
}
