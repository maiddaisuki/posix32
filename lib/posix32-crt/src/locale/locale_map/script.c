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

#define SCRIPT_DECL(name, string) {TEXT (name)}

static const Script Scripts[] = {
  /* clang-format off */
  SCRIPT_DECL ("Adlm", "Adlam"),
  SCRIPT_DECL ("Arab", "Arabic"),
  SCRIPT_DECL ("Armn", ""),
  SCRIPT_DECL ("Beng", "Bangla"),
  SCRIPT_DECL ("Cakm", "Chakma"),
  SCRIPT_DECL ("Cans", "Syllabics"),
  SCRIPT_DECL ("Cher", "Cherokee"),
  SCRIPT_DECL ("Cyrl", "Cyrillic"),
  SCRIPT_DECL ("Deva", "Devanagari"),
  SCRIPT_DECL ("Ethi", ""),
  SCRIPT_DECL ("Geor", ""),
  SCRIPT_DECL ("Gree", ""),
  SCRIPT_DECL ("Gujr", ""),
  SCRIPT_DECL ("Guru", ""),
  SCRIPT_DECL ("Hang", ""),
  SCRIPT_DECL ("Hani", ""),
  SCRIPT_DECL ("Hans", "Simplified"),
  SCRIPT_DECL ("Hant", "Traditional"),
  SCRIPT_DECL ("Hebr", ""),
  SCRIPT_DECL ("Hira", ""),
  SCRIPT_DECL ("Java", "Javanese"),
  SCRIPT_DECL ("Jpan", ""),
  SCRIPT_DECL ("Kana", ""),
  SCRIPT_DECL ("Khmr", ""),
  SCRIPT_DECL ("Knda", ""),
  SCRIPT_DECL ("Kore", ""),
  SCRIPT_DECL ("Laoo", ""),
  SCRIPT_DECL ("Latn", "Latin"),
  SCRIPT_DECL ("Mlym", ""),
  SCRIPT_DECL ("Mong", "Traditional Mongolian"),
  SCRIPT_DECL ("Mymr", ""),
  SCRIPT_DECL ("Nkoo", ""),
  SCRIPT_DECL ("Olck", "Ol Chiki"),
  SCRIPT_DECL ("Orya", ""),
  SCRIPT_DECL ("Sinh", ""),
  SCRIPT_DECL ("Syrc", ""),
  SCRIPT_DECL ("Taml", ""),
  SCRIPT_DECL ("Telu", ""),
  SCRIPT_DECL ("Tfng", "Tifinagh"),
  SCRIPT_DECL ("Thai", ""),
  SCRIPT_DECL ("Tibt", ""),
  SCRIPT_DECL ("Thaa", ""),
  SCRIPT_DECL ("Vaii", "Vai"),
  SCRIPT_DECL ("Yaii", ""),
  /* clang-format on */
};

void p32_script (ScriptIndex index, Script *script) {
  assert (index >= ScriptIndex_MIN && index <= ScriptIndex_MAX);
  *script = Scripts[index];
}
