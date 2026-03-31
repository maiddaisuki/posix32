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

#ifndef LIBPOSIX32_LOCALE_TYPE_H_INCLUDED
#define LIBPOSIX32_LOCALE_TYPE_H_INCLUDED

/**
 * Types of locales.
 */
typedef enum LocaleType {
  /**
   * An invalid `LocaleType` value.
   */
  LocaleType_Invalid = 0,
  /**
   * "POSIX" ("C") locale.
   */
  LocaleType_POSIX,
  /**
   * Normal Windows locale.
   */
  LocaleType_WindowsLocale,
  /**
   * Windows pseudo locale.
   */
  LocaleType_PseudoLocale,
} LocaleType;

#endif /* LIBPOSIX32_LOCALE_TYPE_H_INCLUDED */
