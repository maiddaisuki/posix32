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

#ifndef LIBPOSIX32_STRING_TEST_H_INCLUDED
#define LIBPOSIX32_STRING_TEST_H_INCLUDED

#include "tests-internal.h"

#define C(c) (char) (unsigned) c

/**
 * Simple ASCII text.
 */
const char AsciiText[] = "ASCII Text.";

/**
 * |-separated list of ASCII words.
 */
const char AsciiList[] = "One|Two|Three";

/**
 * Code page 1250; just some funny-looking text.
 *
 * [Š   ] [ß   ] [Ç   ] [Ś   ] [    ] [Ţ   ] [ë   ] [×   ] [ť   ] […   ] [NUL ]
 * [0x8A] [0xDF] [0xC7] [0x8C] [0x20] [0xDE] [0xEB] [0xD7] [0x9D] [0x85] [0x00]
 */
const char SBCS[] = {C (0x8A), C (0xDF), C (0xC7), C (0x8C), C (0x20), C (0xDE),
                     C (0xEB), C (0xD7), C (0x9D), C (0x85), C (0x00)};

/**
 * |-separated list of SBCS words.
 *
 * [Ö   ] [ń   ] [ę   ] [|   ] [Ţ   ] [w   ] [ó   ] [|   ] [Ţ   ] [h   ] [ř   ] [é   ] [ę   ] [NUL ]
 * [0xD6] [0xF1] [0xEA] [0x7C] [0xDE] [0x77] [0xF3] [0x7C] [0xDE] [0x68] [0xF8] [0xE9] [0xEA] [0x00]
 */
const char SBCSList[] = {C (0xD6), C (0xF1), C (0xEA), '|',      C (0xDE), C (0x77), C (0xF3),
                         '|',      C (0xDE), C (0x68), C (0xF8), C (0xE9), C (0xEA), C (0x00)};

/**
 * Code page 932.
 *
 * [日        ] [本        ] [語        ] [テ        ] [ク        ] [ス        ] [ト        ] [。        ] [NUL ]
 * [         ] [     {   ] [         ] [     {   ] [         ] [     X   ] [     g   ] [     B   ] [    ]
 * [0x93 0xFA] [0x96 0x7B] [0x8C 0xEA] [0x83 0x7B] [0x83 0x4E] [0x83 0x58] [0x83 0x67] [0x81 0x42] [0x00]
 */
const char DBCS[] = {C (0x93), C (0xFA), C (0x96), C (0x7B), C (0x8C), C (0xEA), C (0x83), C (0x7B), C (0x83),
                     C (0x4E), C (0x83), C (0x58), C (0x83), C (0x67), C (0x81), C (0x42), C (0x00)};

/**
 * |-separated list of DBCS characters.
 *
 * Note that trailing byte of each DBCS character has the same value as '|'.
 *
 * [ポ        ] [|   ] [忿        ] [|   ] [僴        ] [NUL ]
 * [     |   ] [    ] [     |   ] [    ] [     |   ] [    ]
 * [0x83 0x7C] [0x7C] [0x9C 0x7C] [0x7C] [0xFA 0x7C] [0x00]
 */
const char DBCSList[] = {C (0x83), C (0x7C), '|', C (0x9C), C (0x7C), '|', C (0xFA), C (0x7C), C (0x00)};

/**
 * UTF-8.
 */
const char UTF8[] = "日本語テクスト。";

/**
 * |-separated list of UTF-8 characters.
 */
const char UTF8Lits[] = "一|二|三";

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
