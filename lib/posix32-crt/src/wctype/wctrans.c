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
#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "ctype-internal.h"
#include "locale-internal.h"
#include "string-internal.h"

/**
 * File Summary:
 *
 * This file defines character mapping functions declared in wctype.h.
 */

/**
 * Character mappings.
 */
#include "common/wctrans.c"

/**
 * Generic character mapping functions.
 */
#include "common/towctrans.c"

/**
 * Specific character mapping functions.
 */
#include "common/towlower.c"
#include "common/towupper.c"
