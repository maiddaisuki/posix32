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

#include "string-internal.h"

char *p32_strtok_r (char *str, const char *delim, char **context) {
  /**
   * Start of the next token.
   */
  char *start;

  if (str != NULL) {
    start = str;
  } else {
    start = *context;
  }

  /**
   * Return value;
   */
  char *ret = NULL;

  /**
   * Skip initial part consisting of characters in `delim`.
   */
  start += strspn (start, delim);

  /**
   * Token pointed by `start` consists entirely of characters in `delim`.
   */
  if (start[0] == '\0') {
    /**
     * Ensure that next call with the same `context` returns `NULL`.
     */
    *context = start;
    goto done;
  }

  ret = start;

  /**
   * Advance to the end of token pointed by `start`.
   */
  char *next = start + strcspn (start, delim);
  assert (next != ret);

  /**
   * If token pointed by `start` does not extend to the end of the string,
   * overwrite separator pointed by `next` with NUL and advance `next` past
   * written NUL byte.
   */
  if (next[0] != '\0') {
    *next  = '\0';
    next  += 1;
  }

  *context = next;

done:
  return ret;
}
