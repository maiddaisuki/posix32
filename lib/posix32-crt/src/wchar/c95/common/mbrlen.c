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

/**
 * File Summary:
 *
 * This file contains generic implementation of `mbrlen` function.
 */

size_t p32_private_mbrlen_l (
  const char *P32_RESTRICT mbs,
  size_t                   count,
  mbstate_t *P32_RESTRICT  state,
  locale_t                 locale
) {
  assert (state != NULL);
  return locale->Functions.F_mbrtoc32 (NULL, mbs, count, state, &locale->Charset);
}
