/**
 * Copyright 2026 Kirill Makurin
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

#include "core-test.h"

/**
 * Test Summary:
 *
 * Test `p32_normalize_unicode_string` function with normalization form KD.
 */

typedef struct StringPair {
  const wchar_t *From;
  const wchar_t *To;
} StringPair;

int main (void) {
  p32_test_init ();

  /**
   * Windows 9x systems do not have Unicode support.
   */
  if (p32_platform () == WindowsPlatform9x) {
    return 77;
  }

  StringPair TestNormKD = {L"\u00C4\uFB01", L"\x41\u0308\x66\x69"};

  wchar_t *str1 = NULL;
  wchar_t *str2 = NULL;

  NormalizationRequest request = {0};

  request.Form   = NormForm_KD;
  request.Flags |= P32_NORMALIZATION_REQUEST_MALLOC;

  /**
   * Normalize `TestNormC.From`.
   */
  request.Input  = TestNormKD.From;
  request.Output = &str1;

  assert (p32_normalize_unicode_string (&request, 0) != -1);
  assert (request.Status == NormalizationRequestSuccess);
  assert (wcscmp (TestNormKD.To, str1) == 0);

  /**
   * Attempt to normalize already normalized string.
   */
  request.Input  = str1;
  request.Output = &str2;

  assert (p32_normalize_unicode_string (&request, 0) == -1);
  assert (request.Status == NormalizationRequestNormalized);
  assert (str2 == NULL);

  free (str1);

  return EXIT_SUCCESS;
}
