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
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"
#include "string-internal.h"
#include "uchar-internal.h"
#include "wchar-internal.h"

/**
 * File Summary:
 *
 * This file defines multibyte string functions declared in string-internal.h.
 */

/**
 * Evaluates to non-zero if `Charset` is a SBCS code page where all 256 bytes
 * are assigned code points.
 */
#define P32_IS_SBCS(c) ((c)->MaxLength == 1 && ((c)->Flags & P32_CHARSET_FULL))

/**
 * Functions dependant on character set (LC_CTYPE).
 */
#include "common/strcat.c"
#include "common/strchr.c"
#include "common/strcmp.c"
#include "common/strcpy.c"
#include "common/strcspn.c"
#include "common/strdup.c"
#include "common/strlen.c"
#include "common/strncat.c"
#include "common/strncmp.c"
#include "common/strncpy.c"
#include "common/strndup.c"
#include "common/strnlen.c"
#include "common/strpbrk.c"
#include "common/strrchr.c"
#include "common/strspn.c"
#include "common/strstr.c"
#include "common/strtok.c"
#include "common/strtok_r.c"

/*******************************************************************************
 * External Functions
 */

void p32_mbstring_functions (locale_t locale) {
  /**
   * Functions dependant on character set (LC_CTYPE).
   */
  P32LocaleFunction_strlen (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strnlen (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strcpy (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strncpy (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strcat (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strncat (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strdup (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strndup (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strcmp (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strncmp (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strchr (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strrchr (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strpbrk (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strspn (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strcspn (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strtok_r (&locale->Functions, &locale->Charset);
  P32LocaleFunction_strstr (&locale->Functions, &locale->Charset);
}
