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

#ifndef LIBPOSIX32_STRING_INTERNAL_H_INCLUDED
#define LIBPOSIX32_STRING_INTERNAL_H_INCLUDED

#include "p32_string.h"
#include "p32_strings.h"

P32_TEST_DECL size_t p32_private_strlen_l (const char *str, locale_t locale);
P32_TEST_DECL size_t p32_private_strnlen_l (const char *str, size_t count, locale_t locale);
P32_TEST_DECL char  *p32_private_strcpy_l (char *dest, const char *src, locale_t locale);
P32_TEST_DECL char  *p32_private_strncpy_l (char *dest, const char *src, size_t count, locale_t locale);
P32_TEST_DECL char  *p32_private_strcat_l (char *dest, const char *src, locale_t locale);
P32_TEST_DECL char  *p32_private_strncat_l (char *dest, const char *src, size_t count, locale_t locale);
P32_TEST_DECL char  *p32_private_strdup_l (const char *str, locale_t locale);
P32_TEST_DECL char  *p32_private_strndup_l (const char *str, size_t count, locale_t locale);
P32_TEST_DECL int    p32_private_strcmp_l (const char *s1, const char *s2, locale_t locale);
P32_TEST_DECL int    p32_private_strncmp_l (const char *s1, const char *s2, size_t count, locale_t locale);
P32_TEST_DECL int    p32_private_strcasecmp_l (const char *s1, const char *s2, locale_t locale);
P32_TEST_DECL int    p32_private_strncasecmp_l (const char *s1, const char *s2, size_t count, locale_t locale);
P32_TEST_DECL char  *p32_private_strchr_l (const char *str, int c, locale_t locale);
P32_TEST_DECL char  *p32_private_strrchr_l (const char *str, int c, locale_t locale);
P32_TEST_DECL char  *p32_private_strstr_l (const char *str, const char *substr, locale_t locale);
P32_TEST_DECL char  *p32_private_strpbrk_l (const char *str, const char *set, locale_t locale);
P32_TEST_DECL size_t p32_private_strspn_l (const char *string, const char *set, locale_t locale);
P32_TEST_DECL size_t p32_private_strcspn_l (const char *string, const char *set, locale_t locale);
P32_TEST_DECL char  *p32_private_strtok_l (char *str, const char *delim, locale_t locale);
P32_TEST_DECL char  *p32_private_strtok_r_l (char *str, const char *delim, char **context, locale_t locale);
P32_TEST_DECL int    p32_private_strcoll_l (const char *str1, const char *str2, locale_t locale);
P32_TEST_DECL size_t p32_private_strxfrm_l (char *dest, const char *src, size_t size, locale_t locale);

/**
 * Store locale-specific versions of string.h and strings.h functions in
 * `locale->Functions`.
 */
void p32_string_functions (locale_t locale);

#endif /* LIBPOSIX32_STRING_INTERNAL_H_INCLUDED */
