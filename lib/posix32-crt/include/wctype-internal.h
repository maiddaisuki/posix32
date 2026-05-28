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

#ifndef LIBPOSIX32_WCTYPE_INTERNAL_H_INCLUDED
#define LIBPOSIX32_WCTYPE_INTERNAL_H_INCLUDED

#include "p32_wctype.h"

#include "ctype-internal.h"

/**
 * Internal version of `iswctype_l`.
 *
 * Argument `wctype` may contain multiple `P32_CHARTYPE_*` bits to check
 * whether `wc` belongs to at least one of those character classes.
 *
 * For example, if `wctype` is `P32_CHARTYPE_ALL`, then returned value will
 * contain `P32_CHARTYPE_*` bit for each character class `wc` belongs to.
 */
int p32_private_iswctype_l (wint_t wc, wctype_t wctype, locale_t locale);

/**
 * Internal versions of class-specific `wctype.h` functions.
 */
int p32_private_iswalnum_l (wint_t, locale_t);
int p32_private_iswalpha_l (wint_t, locale_t);
int p32_private_iswblank_l (wint_t, locale_t);
int p32_private_iswcntrl_l (wint_t, locale_t);
int p32_private_iswdigit_l (wint_t, locale_t);
int p32_private_iswgraph_l (wint_t, locale_t);
int p32_private_iswlower_l (wint_t, locale_t);
int p32_private_iswprint_l (wint_t, locale_t);
int p32_private_iswpunct_l (wint_t, locale_t);
int p32_private_iswspace_l (wint_t, locale_t);
int p32_private_iswupper_l (wint_t, locale_t);
int p32_private_iswxdigit_l (wint_t, locale_t);

/**
 * Internal version of `towctrans_l`.
 *
 * Argument `wctrans` can be any `P32_WCTRANS_*` mapping.
 */
wint_t p32_private_towctrans_l (wint_t wc, wctrans_t wctrans, locale_t locale);

/**
 * Internal versions of mapping-specific `wctype.h` functions.
 */
wint_t p32_private_towlower_l (wint_t, locale_t);
wint_t p32_private_towupper_l (wint_t, locale_t);

#endif /* LIBPOSIX32_WCTYPE_INTERNAL_H_INCLUDED */
