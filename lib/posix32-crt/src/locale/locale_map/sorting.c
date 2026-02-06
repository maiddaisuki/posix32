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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

#define SORTING_DECL(name) {TEXT (name)}

static const Sorting SortingTable[] = {
  /* clang-format off */
  SORTING_DECL ("matham"),
  SORTING_DECL ("modern"),
  SORTING_DECL ("phoneb"),
  SORTING_DECL ("pronun"),
  SORTING_DECL ("radstr"),
  SORTING_DECL ("stroke"),
  SORTING_DECL ("technl"),
  SORTING_DECL ("tradnl"),
  /* clang-format on */
};

void p32_sorting (SortingIndex index, Sorting *sorting) {
  assert (index >= SortingIndex_MIN && index <= SortingIndex_MAX);
  *sorting = SortingTable[index];
}
