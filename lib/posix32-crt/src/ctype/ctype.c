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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-internal.h"
#endif

#define _CTYPE_DISABLE_MACROS
#include <ctype.h>
#include <stdio.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "ctype-internal.h"
#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines functions declared in ctype.h.
 */

#include "common/isalnum.c"
#include "common/isalpha.c"
#include "common/isblank.c"
#include "common/iscntrl.c"
#include "common/isdigit.c"
#include "common/isgraph.c"
#include "common/islower.c"
#include "common/isprint.c"
#include "common/ispunct.c"
#include "common/isspace.c"
#include "common/isupper.c"
#include "common/isxdigit.c"
#include "common/tolower.c"
#include "common/toupper.c"

/**
 * Locale-independant functions.
 */
#include "common/isascii.c"
#include "common/toascii.c"
