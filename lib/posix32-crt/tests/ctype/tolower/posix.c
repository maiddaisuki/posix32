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

#include "ctype-test.h"

/**
 * Test Summary:
 *
 * Test conformance of `tolower` function to POSIX specification.
 *
 * Reference: <https://pubs.opengroup.org/onlinepubs/9799919799.2024edition/basedefs/V1_chap07.html#tag_07_03_01>
 */

int main (void) {
  p32_test_init ();

  assert (setlocale (LC_ALL, "POSIX") != NULL);

  for (int i = 0; i < (int) _countof (CharTypeTable); ++i) {
    assert (tolower (i) == CharTypeTable[i].Lower);
  }

  return EXIT_SUCCESS;
}
