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

#ifndef LIBPOSIX32_LC_TIME_H_INCLUDED
#define LIBPOSIX32_LC_TIME_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include <wchar.h>

#include "locale_types/alt_digits.h"

/**
 * Era information.
 */
typedef struct Era {
  /**
   * Era's year offset.
   */
  uint32_t Offset;
  /**
   * Era name.
   */
  struct {
    char    *A;
    wchar_t *W;
  } Name;
  /**
   * Era description string for this calendar.
   */
  wchar_t *String;
} Era;

/**
 * Format string for `strftime` and `wcsftime`.
 */
typedef struct DateTimeFormat {
  /**
   * Format string as used by `GetTimeFormat[Ex]` or `GetDateFormat[Ex]`.
   */
  wchar_t *Format;
  /**
   * Transformed format string for use with `strftime` and `wcsftime`.
   */
  struct {
    wchar_t *W;
    char    *A;
  } Crt;
} DateTimeFormat;

/**
 * Names of week days.
 */
typedef struct DayNames {
  struct {
    char    *A;
    wchar_t *W;
  } Day1;
  struct {
    char    *A;
    wchar_t *W;
  } Day2;
  struct {
    char    *A;
    wchar_t *W;
  } Day3;
  struct {
    char    *A;
    wchar_t *W;
  } Day4;
  struct {
    char    *A;
    wchar_t *W;
  } Day5;
  struct {
    char    *A;
    wchar_t *W;
  } Day6;
  struct {
    char    *A;
    wchar_t *W;
  } Day7;
} DayNames;

/**
 * Names of months.
 */
typedef struct MonthNames {
  struct {
    char    *A;
    wchar_t *W;
  } Mon1;
  struct {
    char    *A;
    wchar_t *W;
  } Mon2;
  struct {
    char    *A;
    wchar_t *W;
  } Mon3;
  struct {
    char    *A;
    wchar_t *W;
  } Mon4;
  struct {
    char    *A;
    wchar_t *W;
  } Mon5;
  struct {
    char    *A;
    wchar_t *W;
  } Mon6;
  struct {
    char    *A;
    wchar_t *W;
  } Mon7;
  struct {
    char    *A;
    wchar_t *W;
  } Mon8;
  struct {
    char    *A;
    wchar_t *W;
  } Mon9;
  struct {
    char    *A;
    wchar_t *W;
  } Mon10;
  struct {
    char    *A;
    wchar_t *W;
  } Mon11;
  struct {
    char    *A;
    wchar_t *W;
  } Mon12;
  struct {
    char    *A;
    wchar_t *W;
  } Mon13;
} MonthNames;

/**
 * Calendar information.
 */
typedef struct CalendarInfo {
  Era Era;
  struct {
    DayNames Full;
    DayNames Abbr;
  } DayNames;
  struct {
    MonthNames Full;
    MonthNames Abbr;
  } MonthNames;
  /**
   * Calendar-specific date format string.
   */
  DateTimeFormat DateFormat;
  /**
   * Calendar-specific date-time format string.
   */
  struct {
    char    *A;
    wchar_t *W;
  } DateTimeFormat;
} CalendarInfo;

/**
 * Locale's "AM"/"PM" strings.
 */
typedef struct AmPm {
  /**
   * Position in the string.
   */
  union {
    uint32_t Value;
    enum {
      AM_PM_SUFFIX = 0,
      AM_PM_PREFIX = 1,
    } Position;
  };
  struct {
    char    *A;
    wchar_t *W;
  } Am;
  struct {
    char    *A;
    wchar_t *W;
  } Pm;
} AmPm;

/**
 * Locale information dependant on `LC_TIME` locale category.
 */
typedef struct LcTimeInfo {
  /**
   * Era description string.
   */
  struct {
    char    *A;
    wchar_t *W;
  } EraString;
  /**
   * Information for locale's default calendar.
   */
  CalendarInfo DefaultCalendar;
  /**
   * Information for locale's alternative calendar.
   */
  CalendarInfo AlternativeCalendar;
  /**
   * "AM"/"PM" strings
   */
  AmPm AmPm;
  /**
   * Alternative Digits.
   */
  AltDigits AltDigits;
  /**
   * Format strings for `strftime` and `wcsftime`.
   */
  DateTimeFormat TimeFormat;
  DateTimeFormat TimeFormatAmPm;
  struct {
    char    *A;
    wchar_t *W;
  } DateFormatD;
  struct {
    char    *A;
    wchar_t *W;
  } DateFormatF;
  struct {
    char    *A;
    wchar_t *W;
  } TimeFormat24;
  struct {
    char    *A;
    wchar_t *W;
  } TimeFormatT;
} LcTimeInfo;

/**
 * Gather locale information dependant on `LC_TIME` locale category.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_localeinfo_time (locale_t locale, uintptr_t heap);

/**
 * Copy locale information from `src` to `dest`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
bool p32_localeinfo_time_copy (LcTimeInfo *dest, uintptr_t heap, LcTimeInfo *src);

/**
 * Free locale information stored in `lcNumericInfo`.
 */
void p32_localeinfo_time_free (LcTimeInfo *lcTimeInfo, uintptr_t heap);

#endif /* LIBPOSIX32_LC_TIME_H_INCLUDED */
