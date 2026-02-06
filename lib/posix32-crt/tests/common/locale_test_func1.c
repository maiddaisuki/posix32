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

  HANDLE    heapHandle = GetProcessHeap ();
  uintptr_t heap       = (uintptr_t) heapHandle;

  LocaleCallback1 callback = NULL;
  Locale          locale   = {0};

#if P32_LOCALE_NAMES
  if (localeString[0] == L'\0') {
    return true;
  } else if (wcscmp (localeString, L"x-IV_mathan") == 0) {
    return true;
  }

  assert (p32_private_wcsdup (&locale.LocaleName, localeString, heap) != -1);

  callback = (LocaleCallback1) param;
#else
  uint32_t localeId = wcstoul (localeString, NULL, 16);

  if (PRIMARYLANGID (LANGIDFROMLCID (localeId)) == LANG_INVARIANT) {
    return true;
  }

  assert (p32_winlocale_from_lcid (&locale, heap, localeId));
  assert (locale.LocaleName != NULL);

  callback = callback1;
#endif

  bool keepGoing = callback (&locale);
  p32_winlocale_destroy (&locale, heap);

  return keepGoing;
}

void p32_locale_test_func1 (LocaleCallback1 callback) {
#if P32_LOCALE_NAMES
  EnumSystemLocalesEx (CallbackWrapper1, LOCALE_ALL, (LPARAM) callback, NULL);
#else
  callback1 = callback;
  EnumSystemLocalesW (CallbackWrapper1, LCID_INSTALLED | LCID_ALTERNATE_SORTS);
#endif
}
