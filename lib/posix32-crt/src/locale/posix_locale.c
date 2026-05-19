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

#ifdef HAVE_CONFIG_H
#include "config.h"
#else
#include "config-internal.h"
#endif

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <wchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file contains locale information for "POSIX" locale.
 */

#define NO_NUMERIC_DATA (UINT32_C (0x67676767))

/**
 * Structure to store single locale information item.
 */
typedef struct LocaleInfoEntry {
  /**
   * Locale information identifier.
   *
   * This must be some `LOCALE_*`, `CAL_*` or `nl_item` constant.
   */
  uint32_t Info;
  /**
   * If `Info` is representable as a number, this is its numeric value.
   * If not, this field must contain `NO_NUMERIC_DATA`.
   */
  uint32_t Number;
  /**
   * Locale information.
   */
  const wchar_t *Data;
} LocaleInfoEntry;

/**
 * Locale information corresponding to `nl_item` constants.
 */
static const LocaleInfoEntry PosixEntries[] = {
/**
 * LC_COLLATE
 */

/* No entries */

/**
 * LC_CTYPE
 */

#ifndef CODESET
  /**
   * Obtained by calling `p32_charset_name` with `P32_CODEPAGE_POSIX`.
   */
  {CODESET,     NO_NUMERIC_DATA, L""                    },
#endif

  /**
   * LC_MESSAGES
   */

  {NOSTR,       NO_NUMERIC_DATA, L"no"                  },
  {YESSTR,      NO_NUMERIC_DATA, L"yes"                 },
  {NOEXPR,      NO_NUMERIC_DATA, L"^[nN]"               },
  {YESEXPR,     NO_NUMERIC_DATA, L"^[yY]"               },

  /**
   * LC_MONETARY
   */

  {CRNCYSTR,    NO_NUMERIC_DATA, L""                    },

  /**
   * LC_NUMERIC
   */

  {RADIXCHAR,   NO_NUMERIC_DATA, L"."                   },
  {THOUSEP,     NO_NUMERIC_DATA, L""                    },

  /**
   * LC_TIME
   */

  /**
   * Format strings.
   */
  {T_FMT,       NO_NUMERIC_DATA, L"%I:%M:%S"            },
  {T_FMT_AMPM,  NO_NUMERIC_DATA, L"%I:%M:%S %p"         },
  {D_FMT,       NO_NUMERIC_DATA, L"%m/%d/%y"            },
  {D_T_FMT,     NO_NUMERIC_DATA, L"%a %b %e %H:%M:%S %Y"},
  /**
   * Full names of week days.
   */
  {DAY_1,       NO_NUMERIC_DATA, L"Sunday"              },
  {DAY_2,       NO_NUMERIC_DATA, L"Monday"              },
  {DAY_3,       NO_NUMERIC_DATA, L"Tuesday"             },
  {DAY_4,       NO_NUMERIC_DATA, L"Wednesday"           },
  {DAY_5,       NO_NUMERIC_DATA, L"Thursday"            },
  {DAY_6,       NO_NUMERIC_DATA, L"Friday"              },
  {DAY_7,       NO_NUMERIC_DATA, L"Saturday"            },
  /**
   * Abbreviated names of week days.
   */
  {ABDAY_1,     NO_NUMERIC_DATA, L"Sun"                 },
  {ABDAY_2,     NO_NUMERIC_DATA, L"Mon"                 },
  {ABDAY_3,     NO_NUMERIC_DATA, L"Tue"                 },
  {ABDAY_4,     NO_NUMERIC_DATA, L"Wed"                 },
  {ABDAY_5,     NO_NUMERIC_DATA, L"Thu"                 },
  {ABDAY_6,     NO_NUMERIC_DATA, L"Fri"                 },
  {ABDAY_7,     NO_NUMERIC_DATA, L"Sat"                 },
  /**
   * Full names of calendar months.
   */
  {MON_1,       NO_NUMERIC_DATA, L"January"             },
  {MON_2,       NO_NUMERIC_DATA, L"February"            },
  {MON_3,       NO_NUMERIC_DATA, L"March"               },
  {MON_4,       NO_NUMERIC_DATA, L"April"               },
  {MON_5,       NO_NUMERIC_DATA, L"May"                 },
  {MON_6,       NO_NUMERIC_DATA, L"June"                },
  {MON_7,       NO_NUMERIC_DATA, L"July"                },
  {MON_8,       NO_NUMERIC_DATA, L"August"              },
  {MON_9,       NO_NUMERIC_DATA, L"September"           },
  {MON_10,      NO_NUMERIC_DATA, L"October"             },
  {MON_11,      NO_NUMERIC_DATA, L"November"            },
  {MON_12,      NO_NUMERIC_DATA, L"December"            },
  /**
   * Abbreviated names of calendar months.
   */
  {ABMON_1,     NO_NUMERIC_DATA, L"Jan"                 },
  {ABMON_2,     NO_NUMERIC_DATA, L"Feb"                 },
  {ABMON_3,     NO_NUMERIC_DATA, L"Mar"                 },
  {ABMON_4,     NO_NUMERIC_DATA, L"Apr"                 },
  {ABMON_5,     NO_NUMERIC_DATA, L"May"                 },
  {ABMON_6,     NO_NUMERIC_DATA, L"Jun"                 },
  {ABMON_7,     NO_NUMERIC_DATA, L"Jul"                 },
  {ABMON_8,     NO_NUMERIC_DATA, L"Aug"                 },
  {ABMON_9,     NO_NUMERIC_DATA, L"Sep"                 },
  {ABMON_10,    NO_NUMERIC_DATA, L"Oct"                 },
  {ABMON_11,    NO_NUMERIC_DATA, L"Nov"                 },
  {ABMON_12,    NO_NUMERIC_DATA, L"Dec"                 },
  /**
   * AM/PM strings.
   */
  {AM_STR,      NO_NUMERIC_DATA, L"AM"                  },
  {PM_STR,      NO_NUMERIC_DATA, L"PM"                  },
#ifndef ERA
  /**
   * Era information.
   */
  {ERA,         NO_NUMERIC_DATA, L""                    },
#endif
  {ERA_T_FMT,   NO_NUMERIC_DATA, L""                    },
  {ERA_D_FMT,   NO_NUMERIC_DATA, L""                    },
  {ERA_D_T_FMT, NO_NUMERIC_DATA, L""                    },
  /**
   * Alternative digits.
   */
  {ALT_DIGITS,  NO_NUMERIC_DATA, L""                    },
};

/**
 * Locale information corresponding to `LOCALE_*` constants.
 */
static const LocaleInfoEntry LocaleInfoEntries[] = {
  /**
   * LC_COLLATE
   */

  /* No entries */

  /**
   * LC_CTYPE
   */

  /* No entries */

  /**
   * LC_MESSAGES
   */

  /* No entries */

  /**
   * LC_MONETARY
   */

  /**
   * This one is different from `CRNCYSTR`, which also contains formattiong
   * information. However, they are the same in "POSIX" locale.
   */
  {LOCALE_SCURRENCY,     NO_NUMERIC_DATA, L""        },

  /**
   * LC_NUMERIC
   */

  /**
   * Same as `RADIXCHAR`.
   */
  {LOCALE_SDECIMAL,      NO_NUMERIC_DATA, L"."       },
  /**
   * Same as `THOUSEP`.
   */
  {LOCALE_STHOUSAND,     NO_NUMERIC_DATA, L""        },

  /**
   * LC_TIME
   */

  /**
   * Same as `AM_STR`.
   */
  {LOCALE_S1159,         NO_NUMERIC_DATA, L"AM"      },
  /**
   * Same as `PM_STR`.
   */
  {LOCALE_S2359,         NO_NUMERIC_DATA, L"PM"      },
  {LOCALE_STIMEFORMAT,   NO_NUMERIC_DATA, L"hh:mm:ss"},
  {LOCALE_ITIMEMARKPOSN, AM_PM_SUFFIX,    L"0"       },
  /**
   * Same as "ALT_DIGITS".
   */
  {LOCALE_SNATIVEDIGITS, NO_NUMERIC_DATA, L""        },
};

/**
 * Locale information corresponding to `CAL_*` constants.
 */
static const LocaleInfoEntry CalendarInfoEntries[] = {
  /**
   * Era information.
   */
  {CAL_SERASTRING,         NO_NUMERIC_DATA, L"A.D."     },
  {CAL_SLONGDATE,          NO_NUMERIC_DATA, L"MM/dd/yy" },
  {CAL_IYEAROFFSETRANGE,   0,               L"0"        },
  /**
   * Full names of week days.
   */
  {CAL_SDAYNAME1,          NO_NUMERIC_DATA, L"Sunday"   },
  {CAL_SDAYNAME2,          NO_NUMERIC_DATA, L"Monday"   },
  {CAL_SDAYNAME3,          NO_NUMERIC_DATA, L"Tuesday"  },
  {CAL_SDAYNAME4,          NO_NUMERIC_DATA, L"Wednesday"},
  {CAL_SDAYNAME5,          NO_NUMERIC_DATA, L"Thursday" },
  {CAL_SDAYNAME6,          NO_NUMERIC_DATA, L"Friday"   },
  {CAL_SDAYNAME7,          NO_NUMERIC_DATA, L"Saturday" },
  /**
   * Abbreviated names of week days.
   */
  {CAL_SABBREVDAYNAME1,    NO_NUMERIC_DATA, L"Sun"      },
  {CAL_SABBREVDAYNAME2,    NO_NUMERIC_DATA, L"Mon"      },
  {CAL_SABBREVDAYNAME3,    NO_NUMERIC_DATA, L"Tue"      },
  {CAL_SABBREVDAYNAME4,    NO_NUMERIC_DATA, L"Wed"      },
  {CAL_SABBREVDAYNAME5,    NO_NUMERIC_DATA, L"Thu"      },
  {CAL_SABBREVDAYNAME6,    NO_NUMERIC_DATA, L"Fri"      },
  {CAL_SABBREVDAYNAME7,    NO_NUMERIC_DATA, L"Sat"      },
  /**
   * Full names of calendar months.
   */
  {CAL_SMONTHNAME1,        NO_NUMERIC_DATA, L"January"  },
  {CAL_SMONTHNAME2,        NO_NUMERIC_DATA, L"February" },
  {CAL_SMONTHNAME3,        NO_NUMERIC_DATA, L"March"    },
  {CAL_SMONTHNAME4,        NO_NUMERIC_DATA, L"April"    },
  {CAL_SMONTHNAME5,        NO_NUMERIC_DATA, L"May"      },
  {CAL_SMONTHNAME6,        NO_NUMERIC_DATA, L"June"     },
  {CAL_SMONTHNAME7,        NO_NUMERIC_DATA, L"July"     },
  {CAL_SMONTHNAME8,        NO_NUMERIC_DATA, L"August"   },
  {CAL_SMONTHNAME9,        NO_NUMERIC_DATA, L"September"},
  {CAL_SMONTHNAME10,       NO_NUMERIC_DATA, L"October"  },
  {CAL_SMONTHNAME11,       NO_NUMERIC_DATA, L"November" },
  {CAL_SMONTHNAME12,       NO_NUMERIC_DATA, L"December" },
  {CAL_SMONTHNAME13,       NO_NUMERIC_DATA, L""         },
  /**
   * Abbreviated names of calendar months.
   */
  {CAL_SABBREVMONTHNAME1,  NO_NUMERIC_DATA, L"Jan"      },
  {CAL_SABBREVMONTHNAME2,  NO_NUMERIC_DATA, L"Feb"      },
  {CAL_SABBREVMONTHNAME3,  NO_NUMERIC_DATA, L"Mar"      },
  {CAL_SABBREVMONTHNAME4,  NO_NUMERIC_DATA, L"Apr"      },
  {CAL_SABBREVMONTHNAME5,  NO_NUMERIC_DATA, L"May"      },
  {CAL_SABBREVMONTHNAME6,  NO_NUMERIC_DATA, L"Jun"      },
  {CAL_SABBREVMONTHNAME7,  NO_NUMERIC_DATA, L"Jul"      },
  {CAL_SABBREVMONTHNAME8,  NO_NUMERIC_DATA, L"Aug"      },
  {CAL_SABBREVMONTHNAME9,  NO_NUMERIC_DATA, L"Sep"      },
  {CAL_SABBREVMONTHNAME10, NO_NUMERIC_DATA, L"Oct"      },
  {CAL_SABBREVMONTHNAME11, NO_NUMERIC_DATA, L"Nov"      },
  {CAL_SABBREVMONTHNAME12, NO_NUMERIC_DATA, L"Dec"      },
  {CAL_SABBREVMONTHNAME13, NO_NUMERIC_DATA, L""         },
};

/**
 * Lookup entry for `info` in `PosixEntries`.
 *
 * On success, returns pointer to corresponding `LocaleInfoEntry` object.
 * On failure, returns `NULL`.
 */
static const LocaleInfoEntry *P32GetPosixEntry (uint32_t info) {
  for (size_t i = 0; i < _countof (PosixEntries); ++i) {
    if (PosixEntries[i].Info == info) {
      return &PosixEntries[i];
    }
  }

  return NULL;
}

/**
 * Lookup entry for `info` in `LocaleInfoEntries`.
 *
 * On success, returns pointer to corresponding `LocaleInfoEntry` object.
 * On failure, returns `NULL`.
 */
static const LocaleInfoEntry *P32GetLocaleInfoEntry (uint32_t info) {
  for (size_t i = 0; i < _countof (LocaleInfoEntries); ++i) {
    if (LocaleInfoEntries[i].Info == info) {
      return &LocaleInfoEntries[i];
    }
  }

  return NULL;
}

/**
 * Lookup entry for `info` in `CalendarInfoEntries`.
 *
 * On success, returns pointer to corresponding `LocaleInfoEntry` object.
 * On failure, returns `NULL`.
 */
static const LocaleInfoEntry *P32GetCalendarInfoEntry (uint32_t info) {
  for (size_t i = 0; i < _countof (CalendarInfoEntries); ++i) {
    if (CalendarInfoEntries[i].Info == info) {
      return &CalendarInfoEntries[i];
    }
  }

  return NULL;
}

/**
 * Retrieve numeric value from `entry`.
 *
 * If `entry` contains numeric data, this function stores it in `*address`
 * and returns `0`.
 *
 * Otherwise, this function returns `-1` to indicate an error.
 */
static int P32GetNumericData (uint32_t *address, const LocaleInfoEntry *entry) {
  assert (entry->Number != NO_NUMERIC_DATA);

  if (entry->Number == NO_NUMERIC_DATA) {
    return -1;
  }

  *address = entry->Number;

  return 0;
}

/**
 * Convert locale information in `entry` and store it in `*address`.
 *
 * Returns `true` on success and `false` otherwise.
 */
static bool P32ConvertEntryData (char **address, uintptr_t heap, const LocaleInfoEntry *entry, Charset *charset) {
  CharsetConversionRequest conversionRequest = {0};

  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_WC_TO_MB;
  conversionRequest.Flags    |= P32_CHARSET_CONVERSION_NO_BEST_FIT;
  conversionRequest.Charset   = charset;
  conversionRequest.Input.W   = entry->Data;
  conversionRequest.Output.A  = address;

  if (p32_charset_convert (&conversionRequest, heap) == -1) {
    assert (conversionRequest.Status != CharsetConversionRequestNoConversion);
    return false;
  }

  return true;
}

/*******************************************************************************
 * External Functions
 */

int p32_posix_get_locale_info (LocaleInfoRequest *request, uintptr_t heap, int flags) {
  HANDLE heapHandle = (HANDLE) heap;

  const LocaleInfoEntry *entry = NULL;

  if (flags & P32_POSIX_LOCALE_INFO_NL_ITEM) {
    entry = P32GetPosixEntry (request->Info);
    assert (entry != NULL);

    if (entry == NULL) {
      return -1;
    }
  } else {
    entry = P32GetLocaleInfoEntry (request->Info);

    if (entry == NULL) {
      return 1;
    }
  }

  if (request->Flags & P32_LOCALE_INFO_REQUEST_NUMERIC) {
    return P32GetNumericData (request->Output, entry);
  }

  wchar_t *data = NULL;

  if ((flags & P32_POSIX_LOCALE_INFO_FALLBACK) == 0) {
    if (p32_private_wcsdup (&data, entry->Data, heap) == -1) {
      goto fail;
    }
  }

  if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT) {
    if (!P32ConvertEntryData (request->OutputA, heap, entry, request->charset)) {
      goto fail_free;
    }
  }

  if (data != NULL) {
    *request->OutputW = data;
  }

  return 0;

fail_free:
  if (data != NULL) {
    HeapFree (heapHandle, 0, data);
  }

fail:
  return -1;
}

int p32_posix_get_calendar_info (CalendarInfoRequest *request, uintptr_t heap, int flags) {
  HANDLE heapHandle = (HANDLE) heap;

  const LocaleInfoEntry *entry = NULL;

  if (flags & P32_POSIX_LOCALE_INFO_NL_ITEM) {
    entry = P32GetPosixEntry (request->Info);
    assert (entry != NULL);

    if (entry == NULL) {
      return -1;
    }
  } else {
    entry = P32GetCalendarInfoEntry (request->Info);

    if (entry == NULL) {
      return 1;
    }
  }

  if (request->Flags & P32_LOCALE_INFO_REQUEST_NUMERIC) {
    return P32GetNumericData (request->Output, entry);
  }

  wchar_t *data = NULL;

  if ((flags & P32_POSIX_LOCALE_INFO_FALLBACK) == 0) {
    if (p32_private_wcsdup (&data, entry->Data, heap) == -1) {
      goto fail;
    }
  }

  if (request->Flags & P32_LOCALE_INFO_REQUEST_CONVERT) {
    if (!P32ConvertEntryData (request->OutputA, heap, entry, request->charset)) {
      goto fail_free;
    }
  }

  if (data != NULL) {
    *request->OutputW = data;
  }

  return 0;

fail_free:
  if (data != NULL) {
    HeapFree (heapHandle, 0, data);
  }

fail:
  return -1;
}
