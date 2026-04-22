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
#include <locale.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "core-test.h"

/**
 * Test Summary:
 *
 * Test `p32_crt_handle` function.
 *
 * When building against static CRT (/MT and /MTd swiches),
 * it must return zero to indicate that library is built against static CRT.
 *
 * Otherwise, it must return valid module handle which can be passed to
 * `GetProcAddress`.
 */

typedef char *(__cdecl *setlocale_t) (int, const char *);

int main (void) {
  p32_test_init ();

  assert (setlocale (LC_ALL, "C") != NULL);

  /**
   * Obtain CRT handle.
   */
  HMODULE crtHandle = (HMODULE) p32_crt_handle ();

#ifdef _DLL
  /**
   * Returned `crtHandle` must be a valid handle.
   */
  assert (crtHandle != NULL);

  /**
   * Dynamically lookup `setlocale`.
   */
  setlocale_t crtSetlocale = (setlocale_t) (UINT_PTR) GetProcAddress (crtHandle, "setlocale");
  assert (crtSetlocale != NULL);

  /**
   * Call `crtSetlocale` to set locale.
   */
  char *crtLocale = crtSetlocale (LC_ALL, "");
  assert (crtLocale != NULL);
  assert ((crtLocale = _strdup (crtLocale)) != NULL);

  /**
   * Verify that `crtSetlocale` is really `setlocale`.
   */
  assert (strcmp (setlocale (LC_ALL, NULL), crtLocale) == 0);

  free (crtLocale);
#else  /* Static CRT */
  /**
   * `p32_crt_handle` must return zero to indicate that we are linked against
   * static CRT.
   */
  assert (crtHandle == NULL);
#endif /* Static CRT */

  return EXIT_SUCCESS;
}
