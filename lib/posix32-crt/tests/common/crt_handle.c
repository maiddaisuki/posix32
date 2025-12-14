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

#if P32_CRT == P32_UCRT
#define P32_CRT_BASENAME TEXT ("ucrtbase")
#elif P32_CRT == P32_MSVCR120
#define P32_CRT_BASENAME TEXT ("msvcr120")
#elif P32_CRT == P32_MSVCR110
#define P32_CRT_BASENAME TEXT ("msvcr110")
#elif P32_CRT == P32_MSVCR100
#define P32_CRT_BASENAME TEXT ("msvcr100")
#elif P32_CRT == P32_MSVCR90
#define P32_CRT_BASENAME TEXT ("msvcr90")
#elif P32_CRT == P32_MSVCR80
#define P32_CRT_BASENAME TEXT ("msvcr80")
#elif P32_CRT == P32_MSVCR71
#define P32_CRT_BASENAME TEXT ("msvcr71")
#elif P32_CRT == P32_MSVCR70
#define P32_CRT_BASENAME TEXT ("msvcr70")
#elif P32_CRT >= P32_MSVCRT42
#define P32_CRT_BASENAME TEXT ("msvcrt")
#elif P32_CRT >= P32_MSVCRT40
#define P32_CRT_BASENAME TEXT ("msvcrt40")
#elif P32_CRT == P32_MSVCRT20
#define P32_CRT_BASENAME TEXT ("msvcrt20")
#elif P32_CRT == P32_MSVCRT10
#define P32_CRT_BASENAME TEXT ("msvcrt10")
#else
#define P32_CRT_BASENAME TEXT ("crtdll")
#endif

#ifdef _DEBUG
#define P32_CRT_SUFFIX TEXT ("d")
#else
#define P32_CRT_SUFFIX
#endif

#define P32_CRT_EXT TEXT (".dll")

#define P32_CRT_FILENAME P32_CRT_BASENAME P32_CRT_SUFFIX P32_CRT_EXT

#ifdef _DLL
static uintptr_t      P32CrtHandle     = 0;
static pthread_once_t P32CrtHandleOnce = PTHREAD_ONCE_INIT;

static void P32InitCrtHandle (void) {
  HANDLE crtHandle = GetModuleHandleW (P32_CRT_FILENAME);

  if (crtHandle == NULL) {
    p32_terminate (L"Failed to obtain handle to CRT (" P32_CRT_FILENAME L").");
  }

  P32CrtHandle = (uintptr_t) crtHandle;
}
#endif /* _DLL */

uintptr_t p32_crt_handle (void) {
#ifdef _DLL
  pthread_once (&P32CrtHandleOnce, P32InitCrtHandle);
  return P32CrtHandle;
#else
  return 0;
#endif
}
