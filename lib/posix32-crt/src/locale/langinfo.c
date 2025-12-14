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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-internal.h"
#endif

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file implements the following functions declared in langinfo.h:
 *
 *  - nl_langinfo
 *  - nl_langinfo_l
 */

/**
 * Return value for `nl_langinfo` when invalid item is requested.
 *
 * Since `nl_langinfo` may return pointers to wide character strings, we must
 * ensure, that when this string is dereferenced as `wchar_t *`, it will not
 * cause access violation.
 */
static char InvalidItem[sizeof (wchar_t)];

/**
 * Internal implementation of `nl_langinfo()`.
 */
static char *P32LangInfo (nl_item item, LocaleInfo *localeInfo) {
  switch (item) {
    /**
     * LC_CTYPE
     */
    case p32_CODESET:
      return localeInfo->LcCtype.CharsetName.A;
    case p32_WCODESET:
      return (char *) localeInfo->LcCtype.CharsetName.W;
    /**
     * LC_MESSAGES
     */
    case p32_YESSTR:
      return localeInfo->LcMessages.YesStr.A;
    case p32_WYESSTR:
      return (char *) localeInfo->LcMessages.YesStr.W;
    case p32_NOSTR:
      return localeInfo->LcMessages.NoStr.A;
    case p32_WNOSTR:
      return (char *) localeInfo->LcMessages.NoStr.W;
    case p32_YESEXPR:
      return localeInfo->LcMessages.YesExpr.A;
    case p32_WYESEXPR:
      return (char *) localeInfo->LcMessages.YesExpr.W;
    case p32_NOEXPR:
      return localeInfo->LcMessages.NoExpr.A;
    case p32_WNOEXPR:
      return (char *) localeInfo->LcMessages.NoExpr.W;
    /**
     * LC_MONETARY
     */
    case p32_CRNCYSTR:
      return localeInfo->LcMonetary.CurrencyString.A;
    case p32_WCRNCYSTR:
      return (char *) localeInfo->LcMonetary.CurrencyString.W;
    /**
     * LC_NUMERIC
     */
    case p32_RADIXCHAR:
      return localeInfo->LcNumeric.RadixChar.A;
    case p32_WRADIXCHAR:
      return (char *) localeInfo->LcNumeric.RadixChar.W;
    case p32_THOUSEP:
      return localeInfo->LcNumeric.ThousandsSeparator.A;
    case p32_WTHOUSEP:
      return (char *) localeInfo->LcNumeric.ThousandsSeparator.W;
    /**
     * LC_TIME
     */
    case p32_T_FMT:
      return localeInfo->LcTime.TimeFormat.Crt.A;
    case p32_WT_FMT:
      return (char *) localeInfo->LcTime.TimeFormat.Crt.W;
    case p32_T_FMT_AMPM:
      return localeInfo->LcTime.TimeFormatAmPm.Crt.A;
    case p32_WT_FMT_AMPM:
      return (char *) localeInfo->LcTime.TimeFormatAmPm.Crt.W;
    case p32_D_FMT:
      return localeInfo->LcTime.DefaultCalendar.DateFormat.Crt.A;
    case p32_WD_FMT:
      return (char *) localeInfo->LcTime.DefaultCalendar.DateFormat.Crt.W;
    case p32_D_T_FMT:
      return localeInfo->LcTime.DefaultCalendar.DateTimeFormat.A;
    case p32_WD_T_FMT:
      return (char *) localeInfo->LcTime.DefaultCalendar.DateTimeFormat.W;
    case p32_DAY_1:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day1.A;
    case p32_DAY_2:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day2.A;
    case p32_DAY_3:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day3.A;
    case p32_DAY_4:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day4.A;
    case p32_DAY_5:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day5.A;
    case p32_DAY_6:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day6.A;
    case p32_DAY_7:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day7.A;
    case p32_WDAY_1:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day1.W;
    case p32_WDAY_2:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day2.W;
    case p32_WDAY_3:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day3.W;
    case p32_WDAY_4:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day4.W;
    case p32_WDAY_5:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day5.W;
    case p32_WDAY_6:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day6.W;
    case p32_WDAY_7:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Full.Day7.W;
    case p32_ABDAY_1:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day1.A;
    case p32_ABDAY_2:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day2.A;
    case p32_ABDAY_3:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day3.A;
    case p32_ABDAY_4:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day4.A;
    case p32_ABDAY_5:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day5.A;
    case p32_ABDAY_6:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day6.A;
    case p32_ABDAY_7:
      return localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day7.A;
    case p32_WABDAY_1:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day1.W;
    case p32_WABDAY_2:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day2.W;
    case p32_WABDAY_3:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day3.W;
    case p32_WABDAY_4:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day4.W;
    case p32_WABDAY_5:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day5.W;
    case p32_WABDAY_6:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day6.W;
    case p32_WABDAY_7:
      return (char *) localeInfo->LcTime.DefaultCalendar.DayNames.Abbr.Day7.W;
    case p32_MON_1:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon1.A;
    case p32_MON_2:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon2.A;
    case p32_MON_3:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon3.A;
    case p32_MON_4:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon4.A;
    case p32_MON_5:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon5.A;
    case p32_MON_6:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon6.A;
    case p32_MON_7:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon7.A;
    case p32_MON_8:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon8.A;
    case p32_MON_9:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon9.A;
    case p32_MON_10:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon10.A;
    case p32_MON_11:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon11.A;
    case p32_MON_12:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon12.A;
    case p32_WMON_1:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon1.W;
    case p32_WMON_2:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon2.W;
    case p32_WMON_3:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon3.W;
    case p32_WMON_4:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon4.W;
    case p32_WMON_5:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon5.W;
    case p32_WMON_6:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon6.W;
    case p32_WMON_7:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon7.W;
    case p32_WMON_8:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon8.W;
    case p32_WMON_9:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon9.W;
    case p32_WMON_10:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon10.W;
    case p32_WMON_11:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon11.W;
    case p32_WMON_12:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Full.Mon12.W;
    case p32_ABMON_1:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon1.A;
    case p32_ABMON_2:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon2.A;
    case p32_ABMON_3:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon3.A;
    case p32_ABMON_4:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon4.A;
    case p32_ABMON_5:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon5.A;
    case p32_ABMON_6:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon6.A;
    case p32_ABMON_7:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon7.A;
    case p32_ABMON_8:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon8.A;
    case p32_ABMON_9:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon9.A;
    case p32_ABMON_10:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon10.A;
    case p32_ABMON_11:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon11.A;
    case p32_ABMON_12:
      return localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon12.A;
    case p32_WABMON_1:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon1.W;
    case p32_WABMON_2:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon2.W;
    case p32_WABMON_3:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon3.W;
    case p32_WABMON_4:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon4.W;
    case p32_WABMON_5:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon5.W;
    case p32_WABMON_6:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon6.W;
    case p32_WABMON_7:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon7.W;
    case p32_WABMON_8:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon8.W;
    case p32_WABMON_9:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon9.W;
    case p32_WABMON_10:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon10.W;
    case p32_WABMON_11:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon11.W;
    case p32_WABMON_12:
      return (char *) localeInfo->LcTime.DefaultCalendar.MonthNames.Abbr.Mon12.W;
    case p32_AM_STR:
      return localeInfo->LcTime.AmPm.Am.A;
    case p32_WAM_STR:
      return (char *) localeInfo->LcTime.AmPm.Am.W;
    case p32_PM_STR:
      return localeInfo->LcTime.AmPm.Pm.A;
    case p32_WPM_STR:
      return (char *) localeInfo->LcTime.AmPm.Pm.W;
    case p32_ERA:
      return localeInfo->LcTime.EraString.A;
    case p32_WERA:
      return (char *) localeInfo->LcTime.EraString.W;
    case p32_ERA_T_FMT:
      return localeInfo->LcTime.TimeFormat.Crt.A;
    case p32_WERA_T_FMT:
      return (char *) localeInfo->LcTime.TimeFormat.Crt.W;
    case p32_ERA_D_FMT:
      return localeInfo->LcTime.AlternativeCalendar.DateFormat.Crt.A;
    case p32_WERA_D_FMT:
      return (char *) localeInfo->LcTime.AlternativeCalendar.DateFormat.Crt.W;
    case p32_ERA_D_T_FMT:
      return localeInfo->LcTime.AlternativeCalendar.DateTimeFormat.A;
    case p32_WERA_D_T_FMT:
      return (char *) localeInfo->LcTime.AlternativeCalendar.DateTimeFormat.W;
    case p32_ALT_DIGITS:
      return localeInfo->LcTime.AltDigits.A;
    case p32_WALT_DIGITS:
      return (char *) localeInfo->LcTime.AltDigits.W;
    default:
      return InvalidItem;
  }
}

char *p32_nl_langinfo_l (nl_item item, locale_t locale) {
  char *ret = P32LangInfo (item, &locale->LocaleInfo);

  if (ret == NULL && (locale->Charset.Flags & P32_CHARSET_EBCDIC) == 0) {
    locale = p32_default_locale ();
    ret    = P32LangInfo (item, &locale->LocaleInfo);
  }

  return ret == NULL ? InvalidItem : ret;
}

char *p32_nl_langinfo (nl_item item) {
  locale_t activeLocale = p32_active_locale ();

#ifdef LIBPOSIX32_TEST
  if (activeLocale == NULL) {
    activeLocale = p32_default_locale ();
  }
#endif

  return p32_nl_langinfo_l (item, activeLocale);
}
