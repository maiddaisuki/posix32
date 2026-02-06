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

#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "uchar-internal.h"

/**
 * File Summary:
 *
 * This file defines DBCS versions of functions declared in uchar.h:
 *
 *  - mbrtoc8
 *  - c8rtomb
 *  - mbrtoc16
 *  - c16rtomb
 *  - mbrtoc32
 *  - c32rtomb
 */

#define P32_UCHAR_IMPL P32_UCHAR_IMPL_DBCS
#include "common/uchar.c"
