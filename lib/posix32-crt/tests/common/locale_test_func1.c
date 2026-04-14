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

static bool P32LocaleTestFunc1 (Locale *locale, void *localeTestFuncData) {
  LocaleTestFuncData *data = (LocaleTestFuncData *) localeTestFuncData;
  return data->Callback1 (locale);
}

void p32_locale_test_func1 (LocaleCallback1 callback) {
  LocaleTestFuncData localeTestFuncData = {0};

  localeTestFuncData.HeapHandle = GetProcessHeap ();
  localeTestFuncData.Callback1  = callback;

  p32_winlocale_enum_system_locales (P32LocaleTestFunc1, localeTestFuncData.Heap, &localeTestFuncData);
}
