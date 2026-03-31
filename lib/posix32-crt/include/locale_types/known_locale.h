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

#ifndef LIBPOSIX32_KNOWN_LOCALE_H_INCLUDED
#define LIBPOSIX32_KNOWN_LOCALE_H_INCLUDED

typedef enum KnownLocaleIndex {
  KnownLocaleIndex_Invalid = -1,
#define KnownLocaleIndex_MIN KnownLocale_POSIX
  KnownLocale_POSIX,
  KnownLocale_QpsPloc,
  KnownLocale_QpsPloca,
  KnownLocale_QpsPlocm,
#define KnownLocaleIndex_MAX KnownLocale_QpsPlocm
} KnownLocaleIndex;

#endif /* LIBPOSIX32_KNOWN_LOCALE_H_INCLUDED */
