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

#ifndef LIBPOSIX32_STRING_TEST_H_INCLUDED
#define LIBPOSIX32_STRING_TEST_H_INCLUDED

#include "tests-internal.h"

/**
 * Input for `{str|wcs}[n]casecmp` and `{str|wcs}coll` functions.
 */

#define STRING(str) str, TEXT (str)

typedef struct {
  const char    *A;
  const wchar_t *W;
} Text;

Text ASCIIString = {STRING ("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz1234567890")};
Text ASCIILower  = {STRING ("abcdefghijklmnopqrstuvwxyz1234567890")};
Text ASCIIUpper  = {STRING ("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890")};

Text Unicode1String = {STRING ("")};
Text Unicode1Lower  = {STRING ("àáâãäåæçèéêëìíîïðñòóôõöøùúûüýµß")};
Text Unicode1Upper  = {STRING ("ÀÁÂÃÄÅÆÇÈÉÊËÌÍÎÏÐÑÒÓÔÕÖØÙÚÛÜÝµß")};

Text Unicode2String = {STRING ("🧡🐔")};
Text Unicode2Lower  = {STRING ("doooom?!🐔 what do you mean doom?!🧡")};
Text Unicode2Upper  = {STRING ("DOOOOM?!🐔 WHAT DO YOU MEAN DOOM?!🧡")};

#endif /* LIBPOSIX32_STRING_TEST_H_INCLUDED */
