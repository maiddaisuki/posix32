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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-internal.h"
#endif

#include <assert.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "string-test.h"

/**
 * Test Summary:
 *
 * Test `strstr` function with UTF-8 (code page 65001).
 */

#define LOCALE "ja_JP.UTF-8"

static void DoTest (void) {
  const char *text = NULL;

  /**
   * Test ASCII text.
   */
  text = AsciiText;

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, "ASCII") == text);
  assert (strstr (text, "Text") == text + 6);

  /**
   * Test ASCII list.
   */
  text = AsciiList;

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, "One") == text);
  assert (strstr (text, "Two") == text + 4);
  assert (strstr (text, "Three") == text + 8);

  /**
   * Test UTF-8 text.
   */
  text = UTF8;

  assert (strstr (text, "") == text);
  assert (strstr (text, text) == text);
  assert (strstr (text, "日本語") == text);
  assert (strstr (text, "テクスト") == text + 9);
  assert (strstr (text, "。") == text + 21);

  /**
   * Test invalid UTF-8 text.
   */
  char *invalidText = NULL;

  assert ((invalidText = strndup (UTF8Lits, 10)) != NULL);

  assert (strstr (invalidText, "") == invalidText);
  assert (strstr (invalidText, invalidText) == NULL);
  assert (strstr (invalidText, "一") == invalidText);
  assert (strstr (invalidText, "|") == invalidText + 3);
  assert (strstr (invalidText, "二") == invalidText + 4);
  assert (strstr (invalidText, "三") == NULL);

  free (invalidText);
}

static DWORD CALLBACK Thread (PVOID arg) {
  const char *localeString = arg;

  locale_t locale = newlocale (LC_ALL_MASK, localeString, NULL);
  assert (locale != NULL && uselocale (locale) != NULL);

  DoTest ();

  assert (uselocale (LC_GLOBAL_LOCALE) == locale);
  freelocale (locale);

  return EXIT_SUCCESS;
}

int main (void) {
  p32_test_init ();

  assert (setlocale (LC_ALL, LOCALE) != NULL);

  DoTest ();

  assert (setlocale (LC_ALL, "POSIX") != NULL);

  DWORD  exitCode     = EXIT_FAILURE;
  HANDLE threadHandle = NULL;

  assert ((threadHandle = CreateThread (NULL, 0, Thread, LOCALE, 0, NULL)) != NULL);

  WaitForSingleObject (threadHandle, INFINITE);
  GetExitCodeThread (threadHandle, &exitCode);
  CloseHandle (threadHandle);

  return EXIT_SUCCESS;
}
