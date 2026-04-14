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

static bool P32LocaleTestFunc3 (uint32_t codePage, void *localeTestFuncData) {
  LocaleTestFuncData *data = (LocaleTestFuncData *) localeTestFuncData;
  return data->Callback3 (codePage);
}

void p32_locale_test_func3 (LocaleCallback3 callback) {
  LocaleTestFuncData localeTestFuncData = {0};

  localeTestFuncData.HeapHandle = GetProcessHeap ();
  localeTestFuncData.Callback3  = callback;

  p32_charset_enum_system_code_pages (P32LocaleTestFunc3, &localeTestFuncData);
}
