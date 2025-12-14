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

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"
#include "time-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to gather locale information dependant on
 * LC_TIME locale category.
 */

/*******************************************************************************
 * Calendar information.
 */

/**
 * Wrapper around `GetCalendarInfo[Ex]`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetCalendarInfo (wchar_t **address, uintptr_t heap, Locale *locale, Calendar calendar, uint32_t info) {
  HANDLE heapHandle = (HANDLE) heap;

  LPWSTR buffer     = NULL;
  INT    bufferSize = 0;

#if P32_LOCALE_NAMES
  bufferSize = GetCalendarInfoEx (locale->LocaleName, calendar, NULL, info, buffer, bufferSize, NULL);
#else
  bufferSize = GetCalendarInfoW (locale->LocaleId, calendar, info, buffer, bufferSize, NULL);
#endif

  assert (bufferSize != 0);

  if (bufferSize == 0) {
    goto fail;
  }

  buffer = (LPWSTR) HeapAlloc (heapHandle, 0, bufferSize * sizeof (WCHAR));

  if (buffer == NULL) {
    goto fail;
  }

#if P32_LOCALE_NAMES
  INT written = GetCalendarInfoEx (locale->LocaleName, calendar, NULL, info, buffer, bufferSize, NULL);
#else
  INT written = GetCalendarInfoW (locale->LocaleId, calendar, info, buffer, bufferSize, NULL);
#endif

  assert (written == bufferSize);

  if (written == 0 || written != bufferSize) {
    goto fail_free;
  }

  *address = buffer;

  return true;

fail_free:
  HeapFree (heapHandle, 0, buffer);

fail:
  return false;
}

/**
 * Wrapper around `GetCalendarInfo[Ex]` which is used to obtain
 * calendar information as an integer value.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32GetCalendarInfoI (uint32_t *address, Locale *locale, Calendar calendar, uint32_t info) {
  DWORD value = 0;

#if P32_LOCALE_NAMES
  int written = GetCalendarInfoEx (locale->LocaleName, calendar, NULL, CAL_RETURN_NUMBER | info, NULL, 0, &value);
#else
  int written = GetCalendarInfoW (locale->LocaleId, calendar, CAL_RETURN_NUMBER | info, NULL, 0, &value);
#endif

  assert (written == 2);

  if (written != 2) {
    return false;
  }

  *address = value;

  return true;
}

/**
 * Format calendar's era description string.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32CalendearEraString (Era *era, uintptr_t heap) {
  int ret = p32_private_aswprintf (
    &era->String, heap, L"+:%u:%u/1/1:%u/1/1:%s:%s", era->Offset, P32_SYSTEMTIME_YEAR_MIN + era->Offset,
    P32_SYSTEMTIME_YEAR_MAX + era->Offset, era->Name.W, L"%y"
  );

  return ret != -1;
}

/**
 * Get calendar information for locale-specific calendar `calendar`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32CalendarInfo (CalendarInfo *info, uintptr_t heap, Locale *locale, Calendar calendar) {
  /**
   * Date format string as used by `GetDateFormat[Ex]`.
   */
  if (!P32GetCalendarInfo (&info->DateFormat.Format, heap, locale, calendar, CAL_SLONGDATE)) {
    goto fail;
  }

  /**
   * Era information.
   */
  if (!P32GetCalendarInfoI (&info->Era.Offset, locale, calendar, CAL_IYEAROFFSETRANGE)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->Era.Name.W, heap, locale, calendar, CAL_SERASTRING)) {
    goto fail;
  }
  if (!P32CalendearEraString (&info->Era, heap)) {
    goto fail;
  }

  /**
   * Full day names.
   */
  if (!P32GetCalendarInfo (&info->DayNames.Full.Day1.W, heap, locale, calendar, CAL_SDAYNAME1)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Full.Day2.W, heap, locale, calendar, CAL_SDAYNAME2)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Full.Day3.W, heap, locale, calendar, CAL_SDAYNAME3)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Full.Day4.W, heap, locale, calendar, CAL_SDAYNAME4)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Full.Day5.W, heap, locale, calendar, CAL_SDAYNAME5)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Full.Day6.W, heap, locale, calendar, CAL_SDAYNAME6)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Full.Day7.W, heap, locale, calendar, CAL_SDAYNAME7)) {
    goto fail;
  }

  /**
   * Abbreviated day names.
   */
  if (!P32GetCalendarInfo (&info->DayNames.Abbr.Day1.W, heap, locale, calendar, CAL_SABBREVDAYNAME1)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Abbr.Day2.W, heap, locale, calendar, CAL_SABBREVDAYNAME2)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Abbr.Day3.W, heap, locale, calendar, CAL_SABBREVDAYNAME3)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Abbr.Day4.W, heap, locale, calendar, CAL_SABBREVDAYNAME4)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Abbr.Day5.W, heap, locale, calendar, CAL_SABBREVDAYNAME5)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Abbr.Day6.W, heap, locale, calendar, CAL_SABBREVDAYNAME6)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->DayNames.Abbr.Day7.W, heap, locale, calendar, CAL_SABBREVDAYNAME7)) {
    goto fail;
  }

  /**
   * Full month names.
   */
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon1.W, heap, locale, calendar, CAL_SMONTHNAME1)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon2.W, heap, locale, calendar, CAL_SMONTHNAME2)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon3.W, heap, locale, calendar, CAL_SMONTHNAME3)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon4.W, heap, locale, calendar, CAL_SMONTHNAME4)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon5.W, heap, locale, calendar, CAL_SMONTHNAME5)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon6.W, heap, locale, calendar, CAL_SMONTHNAME6)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon7.W, heap, locale, calendar, CAL_SMONTHNAME7)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon8.W, heap, locale, calendar, CAL_SMONTHNAME8)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon9.W, heap, locale, calendar, CAL_SMONTHNAME9)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon10.W, heap, locale, calendar, CAL_SMONTHNAME10)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon11.W, heap, locale, calendar, CAL_SMONTHNAME11)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon12.W, heap, locale, calendar, CAL_SMONTHNAME12)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Full.Mon13.W, heap, locale, calendar, CAL_SMONTHNAME13)) {
    goto fail;
  }

  /**
   * Abbreviated month names.
   */
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon1.W, heap, locale, calendar, CAL_SABBREVMONTHNAME1)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon2.W, heap, locale, calendar, CAL_SABBREVMONTHNAME2)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon3.W, heap, locale, calendar, CAL_SABBREVMONTHNAME3)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon4.W, heap, locale, calendar, CAL_SABBREVMONTHNAME4)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon5.W, heap, locale, calendar, CAL_SABBREVMONTHNAME5)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon6.W, heap, locale, calendar, CAL_SABBREVMONTHNAME6)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon7.W, heap, locale, calendar, CAL_SABBREVMONTHNAME7)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon8.W, heap, locale, calendar, CAL_SABBREVMONTHNAME8)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon9.W, heap, locale, calendar, CAL_SABBREVMONTHNAME9)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon10.W, heap, locale, calendar, CAL_SABBREVMONTHNAME10)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon11.W, heap, locale, calendar, CAL_SABBREVMONTHNAME11)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon12.W, heap, locale, calendar, CAL_SABBREVMONTHNAME12)) {
    goto fail;
  }
  if (!P32GetCalendarInfo (&info->MonthNames.Abbr.Mon13.W, heap, locale, calendar, CAL_SABBREVMONTHNAME13)) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Copy calendar information from `src` to `dest`.
 */
static bool P32CopyCalendarInfoW (CalendarInfo *dest, uintptr_t heap, CalendarInfo *src) {
  if (p32_private_wcsdup (&dest->DateFormat.Format, src->DateFormat.Format, heap) == -1) {
    goto fail;
  }

  if (p32_private_wcsdup (&dest->DateFormat.Crt.W, src->DateFormat.Crt.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DateTimeFormat.W, src->DateTimeFormat.W, heap) == -1) {
    goto fail;
  }

  dest->Era.Offset = src->Era.Offset;
  if (p32_private_wcsdup (&dest->Era.Name.W, src->Era.Name.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->Era.String, src->Era.String, heap) == -1) {
    goto fail;
  }

  if (p32_private_wcsdup (&dest->DayNames.Full.Day1.W, src->DayNames.Full.Day1.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Full.Day2.W, src->DayNames.Full.Day2.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Full.Day3.W, src->DayNames.Full.Day3.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Full.Day4.W, src->DayNames.Full.Day4.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Full.Day5.W, src->DayNames.Full.Day5.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Full.Day6.W, src->DayNames.Full.Day6.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Full.Day7.W, src->DayNames.Full.Day7.W, heap) == -1) {
    goto fail;
  }

  if (p32_private_wcsdup (&dest->DayNames.Abbr.Day1.W, src->DayNames.Abbr.Day1.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Abbr.Day2.W, src->DayNames.Abbr.Day2.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Abbr.Day3.W, src->DayNames.Abbr.Day3.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Abbr.Day4.W, src->DayNames.Abbr.Day4.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Abbr.Day5.W, src->DayNames.Abbr.Day5.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Abbr.Day6.W, src->DayNames.Abbr.Day6.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DayNames.Abbr.Day7.W, src->DayNames.Abbr.Day7.W, heap) == -1) {
    goto fail;
  }

  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon1.W, src->MonthNames.Full.Mon1.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon2.W, src->MonthNames.Full.Mon2.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon3.W, src->MonthNames.Full.Mon3.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon4.W, src->MonthNames.Full.Mon4.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon5.W, src->MonthNames.Full.Mon5.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon6.W, src->MonthNames.Full.Mon6.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon7.W, src->MonthNames.Full.Mon7.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon8.W, src->MonthNames.Full.Mon8.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon9.W, src->MonthNames.Full.Mon9.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon10.W, src->MonthNames.Full.Mon10.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon11.W, src->MonthNames.Full.Mon11.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon12.W, src->MonthNames.Full.Mon12.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Full.Mon13.W, src->MonthNames.Full.Mon13.W, heap) == -1) {
    goto fail;
  }

  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon1.W, src->MonthNames.Abbr.Mon1.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon2.W, src->MonthNames.Abbr.Mon2.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon3.W, src->MonthNames.Abbr.Mon3.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon4.W, src->MonthNames.Abbr.Mon4.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon5.W, src->MonthNames.Abbr.Mon5.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon6.W, src->MonthNames.Abbr.Mon6.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon7.W, src->MonthNames.Abbr.Mon7.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon8.W, src->MonthNames.Abbr.Mon8.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon9.W, src->MonthNames.Abbr.Mon9.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon10.W, src->MonthNames.Abbr.Mon10.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon11.W, src->MonthNames.Abbr.Mon11.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon12.W, src->MonthNames.Abbr.Mon12.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->MonthNames.Abbr.Mon13.W, src->MonthNames.Abbr.Mon13.W, heap) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Free calendar information stored in `info`.
 */
static void P32FreeCalendarInfoW (CalendarInfo *info, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (info->DateFormat.Format != NULL) {
    HeapFree (heapHandle, 0, info->DateFormat.Format);
    info->DateFormat.Format = NULL;
  }

  if (info->DateFormat.Crt.W != NULL) {
    HeapFree (heapHandle, 0, info->DateFormat.Crt.W);
    info->DateFormat.Crt.W = NULL;
  }
  if (info->DateTimeFormat.W != NULL) {
    HeapFree (heapHandle, 0, info->DateTimeFormat.W);
    info->DateTimeFormat.W = NULL;
  }

  info->Era.Offset = 0;
  if (info->Era.Name.W != NULL) {
    HeapFree (heapHandle, 0, info->Era.Name.W);
    info->Era.Name.W = NULL;
  }
  if (info->Era.String != NULL) {
    HeapFree (heapHandle, 0, info->Era.String);
    info->Era.String = NULL;
  }

  if (info->DayNames.Full.Day1.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day1.W);
    info->DayNames.Full.Day1.W = NULL;
  }
  if (info->DayNames.Full.Day2.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day2.W);
    info->DayNames.Full.Day2.W = NULL;
  }
  if (info->DayNames.Full.Day3.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day3.W);
    info->DayNames.Full.Day3.W = NULL;
  }
  if (info->DayNames.Full.Day4.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day4.W);
    info->DayNames.Full.Day4.W = NULL;
  }
  if (info->DayNames.Full.Day5.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day5.W);
    info->DayNames.Full.Day5.W = NULL;
  }
  if (info->DayNames.Full.Day6.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day6.W);
    info->DayNames.Full.Day6.W = NULL;
  }
  if (info->DayNames.Full.Day7.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day7.W);
    info->DayNames.Full.Day7.W = NULL;
  }

  if (info->DayNames.Abbr.Day1.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day1.W);
    info->DayNames.Abbr.Day1.W = NULL;
  }
  if (info->DayNames.Abbr.Day2.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day2.W);
    info->DayNames.Abbr.Day2.W = NULL;
  }
  if (info->DayNames.Abbr.Day3.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day3.W);
    info->DayNames.Abbr.Day3.W = NULL;
  }
  if (info->DayNames.Abbr.Day4.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day4.W);
    info->DayNames.Abbr.Day4.W = NULL;
  }
  if (info->DayNames.Abbr.Day5.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day5.W);
    info->DayNames.Abbr.Day5.W = NULL;
  }
  if (info->DayNames.Abbr.Day6.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day6.W);
    info->DayNames.Abbr.Day6.W = NULL;
  }
  if (info->DayNames.Abbr.Day7.W != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day7.W);
    info->DayNames.Abbr.Day7.W = NULL;
  }

  if (info->MonthNames.Full.Mon1.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon1.W);
    info->MonthNames.Full.Mon1.W = NULL;
  }
  if (info->MonthNames.Full.Mon2.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon2.W);
    info->MonthNames.Full.Mon2.W = NULL;
  }
  if (info->MonthNames.Full.Mon3.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon3.W);
    info->MonthNames.Full.Mon3.W = NULL;
  }
  if (info->MonthNames.Full.Mon4.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon4.W);
    info->MonthNames.Full.Mon4.W = NULL;
  }
  if (info->MonthNames.Full.Mon5.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon5.W);
    info->MonthNames.Full.Mon5.W = NULL;
  }
  if (info->MonthNames.Full.Mon6.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon6.W);
    info->MonthNames.Full.Mon6.W = NULL;
  }
  if (info->MonthNames.Full.Mon7.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon7.W);
    info->MonthNames.Full.Mon7.W = NULL;
  }
  if (info->MonthNames.Full.Mon8.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon8.W);
    info->MonthNames.Full.Mon8.W = NULL;
  }
  if (info->MonthNames.Full.Mon9.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon9.W);
    info->MonthNames.Full.Mon9.W = NULL;
  }
  if (info->MonthNames.Full.Mon10.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon10.W);
    info->MonthNames.Full.Mon10.W = NULL;
  }
  if (info->MonthNames.Full.Mon11.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon11.W);
    info->MonthNames.Full.Mon11.W = NULL;
  }
  if (info->MonthNames.Full.Mon12.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon12.W);
    info->MonthNames.Full.Mon12.W = NULL;
  }
  if (info->MonthNames.Full.Mon13.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon13.W);
    info->MonthNames.Full.Mon13.W = NULL;
  }

  if (info->MonthNames.Abbr.Mon1.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon1.W);
    info->MonthNames.Abbr.Mon1.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon2.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon2.W);
    info->MonthNames.Abbr.Mon2.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon3.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon3.W);
    info->MonthNames.Abbr.Mon3.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon4.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon4.W);
    info->MonthNames.Abbr.Mon4.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon5.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon5.W);
    info->MonthNames.Abbr.Mon5.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon6.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon6.W);
    info->MonthNames.Abbr.Mon6.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon7.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon7.W);
    info->MonthNames.Abbr.Mon7.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon8.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon8.W);
    info->MonthNames.Abbr.Mon8.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon9.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon9.W);
    info->MonthNames.Abbr.Mon9.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon10.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon10.W);
    info->MonthNames.Abbr.Mon10.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon11.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon11.W);
    info->MonthNames.Abbr.Mon11.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon12.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon12.W);
    info->MonthNames.Abbr.Mon12.W = NULL;
  }
  if (info->MonthNames.Abbr.Mon13.W != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon13.W);
    info->MonthNames.Abbr.Mon13.W = NULL;
  }
}

/**
 * Convert calendar information in `info` to `locale->Charset.CodePage`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32ConvertCalendarInfo (CalendarInfo *info, uintptr_t heap, locale_t locale) {
  /**
   * Code page to use during conversion.
   */
  uint32_t codePage = locale->Charset.CodePage;

  /**
   * Do not allow best-fit conversion for ASCII.
   */
  bool bestFit = codePage != P32_CODEPAGE_ASCII;

  if (p32_private_wcstombs (&info->DateFormat.Crt.A, info->DateFormat.Crt.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DateTimeFormat.A, info->DateTimeFormat.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  if (p32_private_wcstombs (&info->Era.Name.A, info->Era.Name.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  if (p32_private_wcstombs (&info->DayNames.Full.Day1.A, info->DayNames.Full.Day1.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Full.Day2.A, info->DayNames.Full.Day2.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Full.Day3.A, info->DayNames.Full.Day3.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Full.Day4.A, info->DayNames.Full.Day4.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Full.Day5.A, info->DayNames.Full.Day5.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Full.Day6.A, info->DayNames.Full.Day6.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Full.Day7.A, info->DayNames.Full.Day7.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  if (p32_private_wcstombs (&info->DayNames.Abbr.Day1.A, info->DayNames.Abbr.Day1.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Abbr.Day2.A, info->DayNames.Abbr.Day2.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Abbr.Day3.A, info->DayNames.Abbr.Day3.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Abbr.Day4.A, info->DayNames.Abbr.Day4.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Abbr.Day5.A, info->DayNames.Abbr.Day5.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Abbr.Day6.A, info->DayNames.Abbr.Day6.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DayNames.Abbr.Day7.A, info->DayNames.Abbr.Day7.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  /* clang-format off */
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon1.A, info->MonthNames.Full.Mon1.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon2.A, info->MonthNames.Full.Mon2.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon3.A, info->MonthNames.Full.Mon3.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon4.A, info->MonthNames.Full.Mon4.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon5.A, info->MonthNames.Full.Mon5.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon6.A, info->MonthNames.Full.Mon6.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon7.A, info->MonthNames.Full.Mon7.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon8.A, info->MonthNames.Full.Mon8.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon9.A, info->MonthNames.Full.Mon9.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon10.A, info->MonthNames.Full.Mon10.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon11.A, info->MonthNames.Full.Mon11.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon12.A, info->MonthNames.Full.Mon12.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Full.Mon13.A, info->MonthNames.Full.Mon13.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon1.A, info->MonthNames.Abbr.Mon1.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon2.A, info->MonthNames.Abbr.Mon2.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon3.A, info->MonthNames.Abbr.Mon3.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon4.A, info->MonthNames.Abbr.Mon4.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon5.A, info->MonthNames.Abbr.Mon5.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon6.A, info->MonthNames.Abbr.Mon6.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon7.A, info->MonthNames.Abbr.Mon7.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon8.A, info->MonthNames.Abbr.Mon8.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon9.A, info->MonthNames.Abbr.Mon9.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon10.A, info->MonthNames.Abbr.Mon10.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon11.A, info->MonthNames.Abbr.Mon11.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon12.A, info->MonthNames.Abbr.Mon12.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->MonthNames.Abbr.Mon13.A, info->MonthNames.Abbr.Mon13.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  /* clang-format on */

  return true;

fail:
  return false;
}

/**
 * Copy calendar information from `src` to `dest`.
 */
static bool P32CopyCalendarInfoA (CalendarInfo *dest, uintptr_t heap, CalendarInfo *src) {
  if (p32_private_strdup (&dest->DateFormat.Crt.A, src->DateFormat.Crt.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DateTimeFormat.A, src->DateTimeFormat.A, heap) == -1) {
    goto fail;
  }

  dest->Era.Offset = src->Era.Offset;
  if (p32_private_strdup (&dest->Era.Name.A, src->Era.Name.A, heap) == -1) {
    goto fail;
  }

  if (p32_private_strdup (&dest->DayNames.Full.Day1.A, src->DayNames.Full.Day1.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Full.Day2.A, src->DayNames.Full.Day2.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Full.Day3.A, src->DayNames.Full.Day3.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Full.Day4.A, src->DayNames.Full.Day4.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Full.Day5.A, src->DayNames.Full.Day5.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Full.Day6.A, src->DayNames.Full.Day6.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Full.Day7.A, src->DayNames.Full.Day7.A, heap) == -1) {
    goto fail;
  }

  if (p32_private_strdup (&dest->DayNames.Abbr.Day1.A, src->DayNames.Abbr.Day1.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Abbr.Day2.A, src->DayNames.Abbr.Day2.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Abbr.Day3.A, src->DayNames.Abbr.Day3.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Abbr.Day4.A, src->DayNames.Abbr.Day4.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Abbr.Day5.A, src->DayNames.Abbr.Day5.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Abbr.Day6.A, src->DayNames.Abbr.Day6.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DayNames.Abbr.Day7.A, src->DayNames.Abbr.Day7.A, heap) == -1) {
    goto fail;
  }

  if (p32_private_strdup (&dest->MonthNames.Full.Mon1.A, src->MonthNames.Full.Mon1.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon2.A, src->MonthNames.Full.Mon2.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon3.A, src->MonthNames.Full.Mon3.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon4.A, src->MonthNames.Full.Mon4.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon5.A, src->MonthNames.Full.Mon5.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon6.A, src->MonthNames.Full.Mon6.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon7.A, src->MonthNames.Full.Mon7.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon8.A, src->MonthNames.Full.Mon8.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon9.A, src->MonthNames.Full.Mon9.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon10.A, src->MonthNames.Full.Mon10.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon11.A, src->MonthNames.Full.Mon11.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon12.A, src->MonthNames.Full.Mon12.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Full.Mon13.A, src->MonthNames.Full.Mon13.A, heap) == -1) {
    goto fail;
  }

  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon1.A, src->MonthNames.Abbr.Mon1.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon2.A, src->MonthNames.Abbr.Mon2.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon3.A, src->MonthNames.Abbr.Mon3.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon4.A, src->MonthNames.Abbr.Mon4.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon5.A, src->MonthNames.Abbr.Mon5.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon6.A, src->MonthNames.Abbr.Mon6.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon7.A, src->MonthNames.Abbr.Mon7.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon8.A, src->MonthNames.Abbr.Mon8.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon9.A, src->MonthNames.Abbr.Mon9.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon10.A, src->MonthNames.Abbr.Mon10.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon11.A, src->MonthNames.Abbr.Mon11.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon12.A, src->MonthNames.Abbr.Mon12.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->MonthNames.Abbr.Mon13.A, src->MonthNames.Abbr.Mon13.A, heap) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Free calendar information stored in `info`.
 */
static void P32FreeCalendarInfoA (CalendarInfo *info, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (info->DateFormat.Crt.A != NULL) {
    HeapFree (heapHandle, 0, info->DateFormat.Crt.A);
    info->DateFormat.Crt.A = NULL;
  }
  if (info->DateTimeFormat.A != NULL) {
    HeapFree (heapHandle, 0, info->DateTimeFormat.A);
    info->DateTimeFormat.A = NULL;
  }

  if (info->Era.Name.A != NULL) {
    HeapFree (heapHandle, 0, info->Era.Name.A);
    info->Era.Name.A = NULL;
  }

  if (info->DayNames.Full.Day1.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day1.A);
    info->DayNames.Full.Day1.A = NULL;
  }
  if (info->DayNames.Full.Day2.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day2.A);
    info->DayNames.Full.Day2.A = NULL;
  }
  if (info->DayNames.Full.Day3.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day3.A);
    info->DayNames.Full.Day3.A = NULL;
  }
  if (info->DayNames.Full.Day4.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day4.A);
    info->DayNames.Full.Day4.A = NULL;
  }
  if (info->DayNames.Full.Day5.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day5.A);
    info->DayNames.Full.Day5.A = NULL;
  }
  if (info->DayNames.Full.Day6.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day6.A);
    info->DayNames.Full.Day6.A = NULL;
  }
  if (info->DayNames.Full.Day7.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Full.Day7.A);
    info->DayNames.Full.Day7.A = NULL;
  }

  if (info->DayNames.Abbr.Day1.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day1.A);
    info->DayNames.Abbr.Day1.A = NULL;
  }
  if (info->DayNames.Abbr.Day2.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day2.A);
    info->DayNames.Abbr.Day2.A = NULL;
  }
  if (info->DayNames.Abbr.Day3.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day3.A);
    info->DayNames.Abbr.Day3.A = NULL;
  }
  if (info->DayNames.Abbr.Day4.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day4.A);
    info->DayNames.Abbr.Day4.A = NULL;
  }
  if (info->DayNames.Abbr.Day5.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day5.A);
    info->DayNames.Abbr.Day5.A = NULL;
  }
  if (info->DayNames.Abbr.Day6.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day6.A);
    info->DayNames.Abbr.Day6.A = NULL;
  }
  if (info->DayNames.Abbr.Day7.A != NULL) {
    HeapFree (heapHandle, 0, info->DayNames.Abbr.Day7.A);
    info->DayNames.Abbr.Day7.A = NULL;
  }

  if (info->MonthNames.Full.Mon1.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon1.A);
    info->MonthNames.Full.Mon1.A = NULL;
  }
  if (info->MonthNames.Full.Mon2.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon2.A);
    info->MonthNames.Full.Mon2.A = NULL;
  }
  if (info->MonthNames.Full.Mon3.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon3.A);
    info->MonthNames.Full.Mon3.A = NULL;
  }
  if (info->MonthNames.Full.Mon4.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon4.A);
    info->MonthNames.Full.Mon4.A = NULL;
  }
  if (info->MonthNames.Full.Mon5.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon5.A);
    info->MonthNames.Full.Mon5.A = NULL;
  }
  if (info->MonthNames.Full.Mon6.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon6.A);
    info->MonthNames.Full.Mon6.A = NULL;
  }
  if (info->MonthNames.Full.Mon7.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon7.A);
    info->MonthNames.Full.Mon7.A = NULL;
  }
  if (info->MonthNames.Full.Mon8.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon8.A);
    info->MonthNames.Full.Mon8.A = NULL;
  }
  if (info->MonthNames.Full.Mon9.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon9.A);
    info->MonthNames.Full.Mon9.A = NULL;
  }
  if (info->MonthNames.Full.Mon10.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon10.A);
    info->MonthNames.Full.Mon10.A = NULL;
  }
  if (info->MonthNames.Full.Mon11.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon11.A);
    info->MonthNames.Full.Mon11.A = NULL;
  }
  if (info->MonthNames.Full.Mon12.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon12.A);
    info->MonthNames.Full.Mon12.A = NULL;
  }
  if (info->MonthNames.Full.Mon13.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Full.Mon13.A);
    info->MonthNames.Full.Mon13.A = NULL;
  }

  if (info->MonthNames.Abbr.Mon1.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon1.A);
    info->MonthNames.Abbr.Mon1.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon2.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon2.A);
    info->MonthNames.Abbr.Mon2.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon3.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon3.A);
    info->MonthNames.Abbr.Mon3.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon4.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon4.A);
    info->MonthNames.Abbr.Mon4.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon5.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon5.A);
    info->MonthNames.Abbr.Mon5.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon6.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon6.A);
    info->MonthNames.Abbr.Mon6.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon7.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon7.A);
    info->MonthNames.Abbr.Mon7.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon8.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon8.A);
    info->MonthNames.Abbr.Mon8.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon9.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon9.A);
    info->MonthNames.Abbr.Mon9.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon10.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon10.A);
    info->MonthNames.Abbr.Mon10.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon11.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon11.A);
    info->MonthNames.Abbr.Mon11.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon12.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon12.A);
    info->MonthNames.Abbr.Mon12.A = NULL;
  }
  if (info->MonthNames.Abbr.Mon13.A != NULL) {
    HeapFree (heapHandle, 0, info->MonthNames.Abbr.Mon13.A);
    info->MonthNames.Abbr.Mon13.A = NULL;
  }
}

/*******************************************************************************
 * Locale information.
 */

/**
 * Get locale information dependant on `LC_TIME` locale category.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32LcTimeInfo (LcTimeInfo *info, uintptr_t heap, Locale *locale) {
  /**
   * Time format string used by `GetTimeFormat[Ex]`.
   */
  if (!p32_winlocale_getinfo (&info->TimeFormat.Format, heap, locale, LOCALE_STIMEFORMAT)) {
    goto fail;
  }

  /**
   * Locale's AM/PM strings and their position in the time string.
   */
  if (!p32_winlocale_getinfo_number (&info->AmPm.Value, locale, LOCALE_ITIMEMARKPOSN)) {
    goto fail;
  }
  assert (info->AmPm.Position == AM_PM_PREFIX || info->AmPm.Position == AM_PM_SUFFIX);
  if (!p32_winlocale_getinfo (&info->AmPm.Am.W, heap, locale, LOCALE_S1159)) {
    goto fail;
  }
  if (!p32_winlocale_getinfo (&info->AmPm.Pm.W, heap, locale, LOCALE_S2359)) {
    goto fail;
  }

  /**
   * Locale's alternative digits.
   */
  if (!p32_winlocale_getinfo (&info->AltDigits.W, heap, locale, LOCALE_SNATIVEDIGITS)) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Format era description string and store it in `info->EraString.W`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32EraString (LcTimeInfo *info, uintptr_t heap) {
  int ret;

  if (info->AlternativeCalendar.Era.String != NULL) {
    ret = p32_private_aswprintf (
      &info->EraString.W, heap, L"%s;%s", info->DefaultCalendar.Era.String, info->AlternativeCalendar.Era.String
    );
  } else {
    ret = p32_private_wcsdup (&info->EraString.W, info->DefaultCalendar.Era.String, heap);
  }

  return ret != -1;
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcTimeInfoW (LcTimeInfo *dest, uintptr_t heap, LcTimeInfo *src) {
  if (p32_private_wcsdup (&dest->EraString.W, src->EraString.W, heap) == -1) {
    goto fail;
  }

  if (p32_private_wcsdup (&dest->TimeFormat.Format, src->TimeFormat.Format, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->TimeFormat.Crt.W, src->TimeFormat.Crt.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->TimeFormatAmPm.Format, src->TimeFormatAmPm.Format, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->TimeFormatAmPm.Crt.W, src->TimeFormatAmPm.Crt.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->TimeFormat24.W, src->TimeFormat24.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->TimeFormatT.W, src->TimeFormatT.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DateFormatD.W, src->DateFormatD.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->DateFormatF.W, src->DateFormatF.W, heap) == -1) {
    goto fail;
  }

  dest->AmPm.Value = src->AmPm.Value;
  if (p32_private_wcsdup (&dest->AmPm.Am.W, src->AmPm.Am.W, heap) == -1) {
    goto fail;
  }
  if (p32_private_wcsdup (&dest->AmPm.Pm.W, src->AmPm.Pm.W, heap) == -1) {
    goto fail;
  }

  if (p32_private_wcsdup (&dest->AltDigits.W, src->AltDigits.W, heap) == -1) {
    goto fail;
  }

  return true;

fail:
  return false;
}

/**
 * Free locale information stored in `lcTimeInfo`.
 */
static void P32FreeLcTimeInfoW (LcTimeInfo *lcTimeInfo, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (lcTimeInfo->EraString.W != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->EraString.W);
    lcTimeInfo->EraString.W = NULL;
  }

  if (lcTimeInfo->TimeFormat.Format != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->TimeFormat.Format);
    lcTimeInfo->TimeFormat.Format = NULL;
  }
  if (lcTimeInfo->TimeFormat.Crt.W != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->TimeFormat.Crt.W);
    lcTimeInfo->TimeFormat.Crt.W = NULL;
  }
  if (lcTimeInfo->TimeFormatAmPm.Format != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->TimeFormatAmPm.Format);
    lcTimeInfo->TimeFormatAmPm.Format = NULL;
  }
  if (lcTimeInfo->TimeFormatAmPm.Crt.W != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->TimeFormatAmPm.Crt.W);
    lcTimeInfo->TimeFormatAmPm.Crt.W = NULL;
  }
  if (lcTimeInfo->TimeFormat24.W != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->TimeFormat24.W);
    lcTimeInfo->TimeFormat24.W = NULL;
  }
  if (lcTimeInfo->TimeFormatT.W != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->TimeFormatT.W);
    lcTimeInfo->TimeFormatT.W = NULL;
  }
  if (lcTimeInfo->DateFormatD.W != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->DateFormatD.W);
    lcTimeInfo->DateFormatD.W = NULL;
  }
  if (lcTimeInfo->DateFormatF.W != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->DateFormatF.W);
    lcTimeInfo->DateFormatF.W = NULL;
  }

  lcTimeInfo->AmPm.Value = 0;
  if (lcTimeInfo->AmPm.Am.W != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->AmPm.Am.W);
    lcTimeInfo->AmPm.Am.W = NULL;
  }
  if (lcTimeInfo->AmPm.Pm.W != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->AmPm.Pm.W);
    lcTimeInfo->AmPm.Pm.W = NULL;
  }

  if (lcTimeInfo->AltDigits.W != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->AltDigits.W);
    lcTimeInfo->AltDigits.W = NULL;
  }
}

/**
 * Convert locale information in `info` to `locale->Charset.CodePage`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32ConvertLcTimeInfo (LcTimeInfo *info, uintptr_t heap, locale_t locale) {
  /**
   * Code page to use during conversion.
   */
  uint32_t codePage = locale->Charset.CodePage;

  /**
   * Do not allow best-fit conversion for ASCII.
   */
  bool bestFit = codePage != P32_CODEPAGE_ASCII;

  /**
   * Format strings are constructed from Win32 format strings which may
   * contain characters which cannot be represented by SBCS/DBCS code pages.
   */
  if (p32_private_wcstombs (&info->TimeFormat.Crt.A, info->TimeFormat.Crt.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->TimeFormatAmPm.Crt.A, info->TimeFormatAmPm.Crt.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  /**
   * These should convert losslessly as they contain only ASCII characters.
   */
  if (p32_private_wcstombs (&info->TimeFormat24.A, info->TimeFormat24.W, heap, codePage, false) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->TimeFormatT.A, info->TimeFormatT.W, heap, codePage, false) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DateFormatD.A, info->DateFormatD.W, heap, codePage, false) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->DateFormatF.A, info->DateFormatF.W, heap, codePage, false) == -1) {
    goto fail;
  }

  /**
   * AM/PM strings may contain characters which cannot be represented by
   * SBCS/DBCS code pages.
   */
  if (p32_private_wcstombs (&info->AmPm.Am.A, info->AmPm.Am.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }
  if (p32_private_wcstombs (&info->AmPm.Pm.A, info->AmPm.Pm.W, heap, codePage, bestFit) == -1) {
    goto fail;
  }

  /**
   * We allow situation when locale's alternative digits cannot be converted
   * to `codePage`. However, we do not allow lossy (best-fit) conversion.
   */
  if (p32_private_wcstombs (&info->AltDigits.A, info->AltDigits.W, heap, codePage, false) != -1) {
    /**
     * If `info->AltDigits.W` has been successfully converted to `codePage`,
     * transform converted string to a usable form.
     */
    if (!p32_alt_digits (&info->AltDigits, locale)) {
      goto fail;
    }
  }

  return true;

fail:
  return false;
}

/**
 * Convert era description string to `locale->Charset.CodePage`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32ConvertEraString (LcTimeInfo *info, uintptr_t heap, locale_t locale) {
  /**
   * Code page to use during conversion.
   */
  uint32_t codePage = locale->Charset.CodePage;

  /**
   * Do not allow best-fit conversion for ASCII.
   */
  bool bestFit = codePage != P32_CODEPAGE_ASCII;

  /**
   * We should convert alternative calendar's era description string only
   * if succeeded at converting alternative calendar's information.
   */
  if (info->AlternativeCalendar.Era.Name.A == NULL) {
    if (p32_private_wcstombs (&info->EraString.A, info->DefaultCalendar.Era.String, heap, codePage, bestFit) == -1) {
      goto fail;
    }
  } else {
    if (p32_private_wcstombs (&info->EraString.A, info->EraString.W, heap, codePage, bestFit) == -1) {
      goto fail;
    }
  }

  return true;

fail:
  return false;
}

/**
 * Copy locale information from `src` to `dest`.
 */
static bool P32CopyLcTimeInfoA (LcTimeInfo *dest, uintptr_t heap, LcTimeInfo *src) {
  if (p32_private_strdup (&dest->EraString.A, src->EraString.A, heap) == -1) {
    goto fail;
  }

  if (p32_private_strdup (&dest->TimeFormat.Crt.A, src->TimeFormat.Crt.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->TimeFormatAmPm.Crt.A, src->TimeFormatAmPm.Crt.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->TimeFormat24.A, src->TimeFormat24.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->TimeFormatT.A, src->TimeFormatT.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DateFormatD.A, src->DateFormatD.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->DateFormatF.A, src->DateFormatF.A, heap) == -1) {
    goto fail;
  }

  dest->AmPm.Value = src->AmPm.Value;
  if (p32_private_strdup (&dest->AmPm.Am.A, src->AmPm.Am.A, heap) == -1) {
    goto fail;
  }
  if (p32_private_strdup (&dest->AmPm.Pm.A, src->AmPm.Pm.A, heap) == -1) {
    goto fail;
  }

  /**
   * `src->AltDigits.A` may be NULL if locale's alternative digits cannot be
   * converted to SBCS/DBCS code pages.
   */
  if (src->AltDigits.A != NULL) {
    if (p32_private_strdup (&dest->AltDigits.A, src->AltDigits.A, heap) == -1) {
      goto fail;
    }
    memcpy (dest->AltDigits.Map, src->AltDigits.Map, sizeof (dest->AltDigits.Map));
  }

  return true;

fail:
  return false;
}

/**
 * Free locale information stored in `lcTimeInfo`.
 */
static void P32FreeLcTimeInfoA (LcTimeInfo *lcTimeInfo, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  if (lcTimeInfo->EraString.A != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->EraString.A);
    lcTimeInfo->EraString.A = NULL;
  }

  if (lcTimeInfo->TimeFormat.Crt.A != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->TimeFormat.Crt.A);
    lcTimeInfo->TimeFormat.Crt.A = NULL;
  }
  if (lcTimeInfo->TimeFormatAmPm.Crt.A != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->TimeFormatAmPm.Crt.A);
    lcTimeInfo->TimeFormatAmPm.Crt.A = NULL;
  }
  if (lcTimeInfo->TimeFormat24.A != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->TimeFormat24.A);
    lcTimeInfo->TimeFormat24.A = NULL;
  }
  if (lcTimeInfo->TimeFormatT.A != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->TimeFormatT.A);
    lcTimeInfo->TimeFormatT.A = NULL;
  }
  if (lcTimeInfo->DateFormatD.A != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->DateFormatD.A);
    lcTimeInfo->DateFormatD.A = NULL;
  }
  if (lcTimeInfo->DateFormatF.A != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->DateFormatF.A);
    lcTimeInfo->DateFormatF.A = NULL;
  }

  if (lcTimeInfo->AmPm.Am.A != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->AmPm.Am.A);
    lcTimeInfo->AmPm.Am.A = NULL;
  }
  if (lcTimeInfo->AmPm.Pm.A != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->AmPm.Pm.A);
    lcTimeInfo->AmPm.Pm.A = NULL;
  }

  if (lcTimeInfo->AltDigits.A != NULL) {
    HeapFree (heapHandle, 0, lcTimeInfo->AltDigits.A);
    lcTimeInfo->AltDigits.A = NULL;
  }
  memset (lcTimeInfo->AltDigits.Map, 0, sizeof (lcTimeInfo->AltDigits.Map));
}

/*******************************************************************************
 * Date/Time Format Strings.
 */

/**
 * Structure to store mappings between format specifiers used by Win32
 * `GetTimeFormat[Ex]`/`GetDateFormat[Ex]` and ISO C `strftime`/`wscftime`.
 */
typedef struct FormatMap {
  const wchar_t *Win;
  int            WinLength;
  const wchar_t *Crt;
  int            CrtLength;
} FormatMap;

/**
 * Mappings between time format specifiers used by `GetTimeFormat[Ex]`
 * and `strftime`/`wcsftime`.
 */
static const FormatMap TimeFormatMap[] = {
  {L"hh", 2, L"%I", 2},
  {L"h",  1, L"%I", 2},
  {L"HH", 2, L"%H", 2},
  {L"H",  1, L"%H", 2},
  {L"mm", 2, L"%M", 2},
  {L"m",  1, L"%M", 2},
  {L"ss", 2, L"%S", 2},
  {L"s",  1, L"%S", 2},
  {L"tt", 2, L"%p", 2},
  {L"t",  1, L"%p", 2},
  {L"%",  1, L"%%", 2},
};

/**
 * Mappings between time format specifiers used by `GetTimeFormat[Ex]`
 * and `strftime`/`wcsftime`.
 *
 * Unlike `TimeFormatMap` above, this mapping maps 24-hour format specifiers
 * to 12-hour format specifiers.
 */
static const FormatMap TimeFormatAmPmMap[] = {
  {L"hh", 2, L"%I", 2},
  {L"h",  1, L"%I", 2},
  {L"HH", 2, L"%I", 2},
  {L"H",  1, L"%I", 2},
  {L"mm", 2, L"%M", 2},
  {L"m",  1, L"%M", 2},
  {L"ss", 2, L"%S", 2},
  {L"s",  1, L"%S", 2},
  {L"tt", 2, L"%p", 2},
  {L"t",  1, L"%p", 2},
  {L"%",  1, L"%%", 2},
};

/**
 * Mappings between date format specifiers used by `GetDateFormat[Ex]`
 * and `strftime`/`wcsftime`.
 */
static const FormatMap DateFormatMap[] = {
  {L"dddd",  4, L"%A",  2},
  {L"ddd",   3, L"%a",  2},
  {L"dd",    2, L"%e",  2},
  {L"d",     1, L"%d",  2},
  {L"MMMM",  4, L"%B",  2},
  {L"MMM",   3, L"%b",  2},
  {L"MM",    2, L"%m",  2},
  {L"M",     1, L"%m",  2},
  {L"yyyyy", 5, L"%Y",  2},
  {L"yyyy",  4, L"%Y",  2},
  {L"yy",    2, L"%y",  2},
  {L"y",     1, L"%y",  2},
  {L"gg",    2, L"%EC", 0},
  {L"g",     1, L"%EC", 0},
  {L"%",     1, L"%%",  2},
};

/**
 * Map date/time format string used by `GetTimeFormat[Ex]`/`GetDateFormat[Ex]`
 * to equivalent format string to be used with `strftime`/`wcsftime`.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32MapFormatString (DateTimeFormat *format, uintptr_t heap, const FormatMap *map, const size_t mapSize) {
  HANDLE heapHandle = (HANDLE) heap;

  /**
   * Size of buffer to hold mapped format string.
   */
  int bufferSize = BUFSIZ;

  /**
   * Buffer to hold mapped format string.
   */
  wchar_t *buffer = HeapAlloc (heapHandle, 0, bufferSize * sizeof (wchar_t));

  if (buffer == NULL) {
    return false;
  }

  wchar_t *in  = format->Format;
  wchar_t *out = buffer;

  /**
   * Number of wide characters written to `buffer`.
   */
  int written = 0;

  /**
   * Set to `true` when processing part of `in` within single quotes,
   * which must be copied literally to `out`.
   */
  bool inQuotes = false;

  while (1) {
    /**
     * Reached the end of `in`.
     */
    if (*in == L'\0') {
      assert (!inQuotes);

      *out     = L'\0';
      written += 1;

      break;
    }

    /**
     * Text within single quotes will be copied to `out` literally.
     *
     * One important detail to keep in mind is that two single quotes in a row
     * within a single-quoted part must produce a single qoute rather than
     * end the quoted part.
     *
     * For example, "''''" must produce just "'".
     */
    if (*in == L'\'') {
      in += 1;

      if (!inQuotes) {
        inQuotes = true;
        continue;
      }

      /**
       * If `in` does not points to a single quote, set `inQuotes` to `false`
       * to mark the end of quoted part.
       */
      if (*in != L'\'') {
        inQuotes = false;
        continue;
      }

      assert (inQuotes);
    }

    if (!inQuotes) {
      bool skip = false;

      for (size_t i = 0; i < mapSize; ++i) {
        if (wcsncmp (in, map[i].Win, map[i].WinLength) == 0) {
          skip = true;

          wmemcpy (out, map[i].Crt, map[i].CrtLength);
          written += map[i].CrtLength;

          in  += map[i].WinLength;
          out += map[i].CrtLength;

          break;
        }
      }

      if (skip) {
        continue;
      }
    }

    *out     = *in;
    written += 1;

    in  += 1;
    out += 1;
  }

  assert (written <= bufferSize);

  wchar_t *address = HeapReAlloc (heapHandle, 0, buffer, written * sizeof (wchar_t));

  if (address == NULL) {
    goto fail_free;
  }

  format->Crt.W = address;

  return true;

fail_free:
  HeapFree (heapHandle, 0, buffer);

  return false;
}

/**
 * Construct time format string for use with AM/PM format.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32TimeAmPmFormat (LcTimeInfo *info, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  /**
   * Whether locale has AM/PM strings.
   */
  bool haveAmPm = (info->AmPm.Am.W[0] != L'\0' && info->AmPm.Pm.W[0] != L'\0');

  /**
   * Whetehr locale's time format string includes 'tt' specifier (AM/PM).
   */
  bool haveAmPmSpec = !!wcsstr (info->TimeFormat.Format, L"tt");

  /**
   * Situation when locale's time format string contains 'tt' format specifier
   * but locale does not have AM/PM strings does not make sense.
   */
  assert (haveAmPm || !haveAmPmSpec);

  /**
   * If locale has AM/PM string but its default time format string does not
   * include 'tt' format specifier, add it to the beginning or the end of
   * format string.
   */
  if (haveAmPm && !haveAmPmSpec) {
    size_t formatLength = wcslen (info->TimeFormat.Format);

    size_t   bufferSize = formatLength + 4;
    wchar_t *buffer     = (wchar_t *) HeapAlloc (heapHandle, 0, bufferSize * sizeof (wchar_t));

    if (buffer == NULL) {
      return false;
    }

    if (info->AmPm.Position == AM_PM_PREFIX) {
      wmemcpy (buffer, L"tt ", 3);
      wmemcpy (buffer + 3, info->TimeFormat.Format, formatLength + 1);
    } else if (info->AmPm.Position == AM_PM_SUFFIX) {
      wmemcpy (buffer, info->TimeFormat.Format, formatLength);
      wmemcpy (buffer + formatLength, L" tt", 4);
    }

    info->TimeFormatAmPm.Format = buffer;

    return P32MapFormatString (&info->TimeFormatAmPm, heap, TimeFormatAmPmMap, _countof (TimeFormatAmPmMap));
  }

  /**
   * Otherwise, use locale's default time format string.
   *
   * This means either:
   *
   * 1. locale's default time format string includes 'tt' specifier in which
   *    case it we use it as-is.
   *
   * 2. locale does not have AM/PM string, in which case 24-hour format string
   *    will be used.
   */
  if (p32_private_wcsdup (&info->TimeFormatAmPm.Format, info->TimeFormat.Format, heap) == -1) {
    return false;
  }

  return P32MapFormatString (&info->TimeFormatAmPm, heap, TimeFormatMap, _countof (TimeFormatMap));
}

/**
 * Format Date-Time format string.
 *
 * We join calendar's default date format string and locale's default time
 * format string with a single space in between.
 *
 * Returns `true` on success, and `false` otherwise.
 */
static bool P32DateTimeFormat (LcTimeInfo *info, CalendarInfo *calendarInfo, uintptr_t heap) {
  HANDLE heapHandle = (HANDLE) heap;

  size_t dateFormatLength = wcslen (calendarInfo->DateFormat.Crt.W);
  size_t timeFormatLength = wcslen (info->TimeFormat.Crt.W);

  size_t   bufferSize = dateFormatLength + timeFormatLength + 2;
  wchar_t *buffer     = HeapAlloc (heapHandle, 0, bufferSize * sizeof (wchar_t));

  if (buffer == NULL) {
    return false;
  }

  wchar_t *address = buffer;

  wmemcpy (address, calendarInfo->DateFormat.Crt.W, dateFormatLength);
  address += dateFormatLength;

  *address  = L' ';
  address  += 1;

  wmemcpy (address, info->TimeFormat.Crt.W, timeFormatLength);
  address += timeFormatLength;

  *address = L'\0';

  calendarInfo->DateTimeFormat.W = buffer;

  return true;
}

/**
 * Get locale's date/time format strings.
 */
static bool P32DateTime (LcTimeInfo *lcTimeInfo, uintptr_t heap, bool useAlternativeCalendar) {
  CalendarInfo *defaultCalendar     = &lcTimeInfo->DefaultCalendar;
  CalendarInfo *alternativeCalendar = &lcTimeInfo->AlternativeCalendar;

  /**
   * Time format string for "%X" format specifier.
   */
  if (!P32MapFormatString (&lcTimeInfo->TimeFormat, heap, TimeFormatMap, _countof (TimeFormatMap))) {
    goto fail;
  }

  /**
   * Time format string for "%r" format specifier.
   */
  if (!P32TimeAmPmFormat (lcTimeInfo, heap)) {
    goto fail;
  }

  /**
   * Time format string for "%R" format specifier.
   */
  if (p32_private_wcsdup (&lcTimeInfo->TimeFormat24.W, L"%H:%M", heap) == -1) {
    return false;
  }

  /**
   * Time format string for "%T" format specifier.
   */
  if (p32_private_wcsdup (&lcTimeInfo->TimeFormatT.W, L"%H:%M:%S", heap) == -1) {
    return false;
  }

  /**
   * Date format string for "%x" format specifier.
   */
  if (!P32MapFormatString (&defaultCalendar->DateFormat, heap, DateFormatMap, _countof (DateFormatMap))) {
    goto fail;
  }

  /**
   * Date format string for "%Ex" format specifier.
   */
  if (useAlternativeCalendar) {
    if (!P32MapFormatString (&alternativeCalendar->DateFormat, heap, DateFormatMap, _countof (DateFormatMap))) {
      goto fail;
    }
  }

  /**
   * Date format string for "%D" format specifier.
   */
  if (p32_private_wcsdup (&lcTimeInfo->DateFormatD.W, L"%m/%d/%y", heap) == -1) {
    return false;
  }

  /**
   * Date format string for "%F" format specifier.
   */
  if (p32_private_wcsdup (&lcTimeInfo->DateFormatF.W, L"%Y-%m-%d", heap) == -1) {
    return false;
  }

  /**
   * Date-Time format string for "%c" format specifier.
   */
  if (!P32DateTimeFormat (lcTimeInfo, &lcTimeInfo->DefaultCalendar, heap)) {
    goto fail;
  }

  /**
   * Date-Time format string for "%Ec" format specifier.
   */
  if (useAlternativeCalendar) {
    if (!P32DateTimeFormat (lcTimeInfo, &lcTimeInfo->AlternativeCalendar, heap)) {
      goto fail;
    }
  }

  return true;

fail:
  return false;
}

/*******************************************************************************
 * External Functions.
 */

bool p32_localeinfo_time (locale_t locale, uintptr_t heap) {
  Locale     *lcTime     = &locale->WinLocale.LcTime;
  LcTimeInfo *lcTimeInfo = &locale->LocaleInfo.LcTime;

  /**
   * Use locale's alternative calendar if it has one.
   */
  bool useAlternativeCalendar = (lcTime->AlternativeCalendar != 0);

  if (!P32LcTimeInfo (lcTimeInfo, heap, lcTime)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"LC_TIME(%s): failed to obtain locale information\n", locale->WindowsLocaleStrings.W.LcTime);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail;
  }

  if (!P32CalendarInfo (&lcTimeInfo->DefaultCalendar, heap, lcTime, lcTime->Calendar)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"LC_TIME(%s): failed to obtain calendar information\n", locale->WindowsLocaleStrings.W.LcTime);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_default_calendar;
  }

  if (useAlternativeCalendar) {
    if (!P32CalendarInfo (&lcTimeInfo->AlternativeCalendar, heap, lcTime, lcTime->AlternativeCalendar)) {
#ifdef LIBPOSIX32_TEST
      _RPTW1 (
        _CRT_ERROR, L"LC_TIME(%s): failed to obtain alternative calendar information\n",
        locale->WindowsLocaleStrings.W.LcTime
      );

      if (IsDebuggerPresent ()) {
        DebugBreak ();
      }
#endif

      goto fail_alternative_calendar;
    }
  }

  if (!P32DateTime (lcTimeInfo, heap, useAlternativeCalendar)) {
    goto fail_alternative_calendar;
  }

  if (!P32EraString (lcTimeInfo, heap)) {
    goto fail_alternative_calendar;
  }

  if (!P32ConvertLcTimeInfo (lcTimeInfo, heap, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (_CRT_ERROR, L"LC_TIME(%s): failed to convert locale information\n", locale->WindowsLocaleStrings.W.LcTime);

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_convert;
  }

  if (!P32ConvertCalendarInfo (&lcTimeInfo->DefaultCalendar, heap, locale)) {
#ifdef LIBPOSIX32_TEST
    _RPTW1 (
      _CRT_ERROR, L"LC_TIME(%s): failed to convert calendar information\n", locale->WindowsLocaleStrings.W.LcTime
    );

    if (IsDebuggerPresent ()) {
      DebugBreak ();
    }
#endif

    goto fail_convert_default_calendar;
  }

  /**
   * We allow situation when we cannot convert alternaive calendar info to
   * `locale->Charset.CodePage`.
   *
   * This is a common case for many SBCS/DBCS locales.
   */
  if (useAlternativeCalendar) {
    if (!P32ConvertCalendarInfo (&lcTimeInfo->AlternativeCalendar, heap, locale)) {
#ifdef LIBPOSIX32_TEST
      _RPTW1 (
        _CRT_WARN, L"LC_TIME(%s): failed to convert alternative calendar information\n",
        locale->WindowsLocaleStrings.W.LcTime
      );
#endif

      P32FreeCalendarInfoA (&lcTimeInfo->AlternativeCalendar, heap);
    }
  }

  if (!P32ConvertEraString (lcTimeInfo, heap, locale)) {
    goto fail_convert_default_calendar;
  }

  return true;

fail_convert_default_calendar:
  P32FreeCalendarInfoA (&lcTimeInfo->DefaultCalendar, heap);

fail_convert:
  P32FreeLcTimeInfoA (lcTimeInfo, heap);

fail_alternative_calendar:
  P32FreeCalendarInfoW (&lcTimeInfo->AlternativeCalendar, heap);

fail_default_calendar:
  P32FreeCalendarInfoW (&lcTimeInfo->DefaultCalendar, heap);

fail:
  P32FreeLcTimeInfoW (lcTimeInfo, heap);

  return false;
}

bool p32_localeinfo_time_copy (LcTimeInfo *dest, uintptr_t heap, LcTimeInfo *src) {
  if (!P32CopyLcTimeInfoW (dest, heap, src)) {
    goto fail_w;
  }

  if (!P32CopyLcTimeInfoA (dest, heap, src)) {
    goto fail_a;
  }

  if (!P32CopyCalendarInfoW (&dest->DefaultCalendar, heap, &src->DefaultCalendar)) {
    goto fail_default_calendar_w;
  }

  if (!P32CopyCalendarInfoA (&dest->DefaultCalendar, heap, &src->DefaultCalendar)) {
    goto fail_default_calendar_a;
  }

  if (src->AlternativeCalendar.Era.Name.W != NULL) {
    if (!P32CopyCalendarInfoW (&dest->AlternativeCalendar, heap, &src->AlternativeCalendar)) {
      goto fail_alternative_calendar_w;
    }
  }

  if (src->AlternativeCalendar.Era.Name.A != NULL) {
    if (!P32CopyCalendarInfoA (&dest->AlternativeCalendar, heap, &src->AlternativeCalendar)) {
      goto fail_alternative_calendar_a;
    }
  }

  return true;

fail_alternative_calendar_a:
  P32FreeCalendarInfoA (&dest->AlternativeCalendar, heap);
fail_alternative_calendar_w:
  P32FreeCalendarInfoW (&dest->AlternativeCalendar, heap);
fail_default_calendar_a:
  P32FreeCalendarInfoA (&dest->DefaultCalendar, heap);
fail_default_calendar_w:
  P32FreeCalendarInfoW (&dest->DefaultCalendar, heap);
fail_a:
  P32FreeLcTimeInfoA (dest, heap);
fail_w:
  P32FreeLcTimeInfoW (dest, heap);

  return false;
}

void p32_localeinfo_time_free (LcTimeInfo *lcTimeInfo, uintptr_t heap) {
  P32FreeLcTimeInfoA (lcTimeInfo, heap);
  P32FreeLcTimeInfoW (lcTimeInfo, heap);
  P32FreeCalendarInfoA (&lcTimeInfo->DefaultCalendar, heap);
  P32FreeCalendarInfoW (&lcTimeInfo->DefaultCalendar, heap);
  P32FreeCalendarInfoA (&lcTimeInfo->AlternativeCalendar, heap);
  P32FreeCalendarInfoW (&lcTimeInfo->AlternativeCalendar, heap);
}
