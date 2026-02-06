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

#ifndef LIBPOSIX32_LC_MESSAGES_H_INCLUDED
#define LIBPOSIX32_LC_MESSAGES_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

/**
 * Locale information dependant on `LC_MESSAGES` locale category.
 */
typedef struct LcMessagesInfo {
  struct {
    char    *A;
    wchar_t *W;
  } NoStr;
  struct {
    char    *A;
    wchar_t *W;
  } YesStr;
  struct {
    char    *A;
    wchar_t *W;
  } NoExpr;
  struct {
    char    *A;
    wchar_t *W;
  } YesExpr;
} LcMessagesInfo;

/**
 * Gather locale information dependant on `LC_MESSAGES` locale category.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_localeinfo_messages (locale_t locale, uintptr_t heap);

/**
 * Copy locale information from `src` to `dest`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_localeinfo_messages_copy (LcMessagesInfo *dest, uintptr_t heap, LcMessagesInfo *src);

/**
 * Free locale information stored in `lcMessagesInfo`.
 */
void p32_localeinfo_messages_free (LcMessagesInfo *lcMessagesInfo, uintptr_t heap);

#endif /* LIBPOSIX32_LC_MESSAGES_H_INCLUDED */
