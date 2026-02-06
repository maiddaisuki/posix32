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
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

/**
 * File Summary:
 *
 * This file defines functions to obtain information about code pages.
 */

/**
 * A SBCS code page where all 256 bytes are valid characters.
 */
#define CHARSET_SBCS_FULL (P32_CHARSET_SBCS | P32_CHARSET_FULL)

/**
 * An EBCDIC code page.
 */
#define CHARSET_EBCDIC (P32_CHARSET_SBCS | P32_CHARSET_EBCDIC | P32_CHARSET_REJECT_GLOBAL)

/**
 * An EBCDIC code page where all 256 bytes are valid characters.
 */
#define CHARSET_EBCDIC_FULL (CHARSET_EBCDIC | P32_CHARSET_FULL)

#if P32_CRT == P32_MSVCRT10
/**
 * A DBCS code page.
 *
 * msvcrt10.dll does not have multibyte support.
 * Attempt to set such code pages with `setlocale` will result in crash.
 *
 * We emulate thread locales for this CRT, which means DBCS code pages
 * can be used with thread locales.
 */
#define CHARSET_DBCS (P32_CHARSET_DBCS | P32_CHARSET_REJECT_CRT)
#else
/**
 * A DBCS code page.
 */
#define CHARSET_DBCS (P32_CHARSET_DBCS)
#endif

typedef struct CodePageInfo {
  uint32_t       CodePage;
  uint32_t       Flags;
  const wchar_t *CharsetName;
} CodePageInfo;

static const CodePageInfo Charsets[] = {
  /**
   * IBM U.S./Canada.
   */
  {37,    CHARSET_EBCDIC_FULL,                           L"IBM-037"         },
  /**
   * OEM United States.
   */
  {437,   CHARSET_SBCS_FULL,                             L"CP437"           },
  /**
   * IBM International.
   */
  {500,   CHARSET_EBCDIC_FULL,                           L"IBM-500"         },
  /**
   * ASMO-708 with extra code points from code page 437.
   */
  {708,   P32_CHARSET_SBCS,                              L"ASMO-708"        },
  /**
   * ASMO 449+ with extra code points from code page 437.
   */
  {709,   P32_CHARSET_SBCS | P32_CHARSET_REJECT_ASCII,   L"ASMO-449+"       },
  /**
   * Combination of ASMO-708 and code page 437.
   */
  {710,   P32_CHARSET_SBCS | P32_CHARSET_REJECT_LEGACY,  L"710"             },
  /**
   * OEM Arabic.
   *
   * Combination of ASMO-708 and code page 437.
   */
  {720,   CHARSET_SBCS_FULL,                             L"720"             },
  /**
   * OEM Greek.
   */
  {737,   CHARSET_SBCS_FULL,                             L"CP737"           },
  /**
   * OEM Baltic.
   */
  {775,   CHARSET_SBCS_FULL,                             L"CP775"           },
  /**
   * OEM Latin 1.
   */
  {850,   CHARSET_SBCS_FULL,                             L"CP850"           },
  /**
   * OEM Latin 2.
   */
  {852,   CHARSET_SBCS_FULL,                             L"CP852"           },
  /**
   * OEM Cyrillic.
   */
  {855,   CHARSET_SBCS_FULL,                             L"CP855"           },
  /**
   * OEM Turkish.
   */
  {857,   P32_CHARSET_SBCS,                              L"CP857"           },
  /**
   * OEM Latin 1 (+ Euro).
   */
  {858,   CHARSET_SBCS_FULL,                             L"CP858"           },
  /**
   * OEM Portuguese.
   */
  {860,   CHARSET_SBCS_FULL,                             L"CP860"           },
  /**
   * OEM Icelandic.
   */
  {861,   CHARSET_SBCS_FULL,                             L"CP861"           },
  /**
   * OEM Hebrew.
   */
  {862,   CHARSET_SBCS_FULL,                             L"CP862"           },
  /**
   * OEM French Canadian.
   */
  {863,   CHARSET_SBCS_FULL,                             L"CP863"           },
  /**
   * OEM Arabic.
   */
  {864,   P32_CHARSET_SBCS,                              L"CP864"           },
  /**
   * OEM Nordic.
   */
  {865,   CHARSET_SBCS_FULL,                             L"CP865"           },
  /**
   * OEM Russian.
   */
  {866,   CHARSET_SBCS_FULL,                             L"CP866"           },
  /**
   * OEM Modern Greek.
   */
  {869,   CHARSET_SBCS_FULL,                             L"CP869"           },
  /**
   * IBM Latin 2.
   */
  {870,   CHARSET_EBCDIC_FULL,                           L"IBM-870"         },
  /**
   * Thai.
   *
   * Charset: ISO-8859-11 (derived from TIS-620) with extensions
   */
  {874,   P32_CHARSET_SBCS,                              L"CP874"           },
  /**
   * IBM Greek.
   */
  {875,   CHARSET_EBCDIC_FULL,                           L"IBM-875"         },
  /**
   * SHIFT_JIS.
   */
  {932,   CHARSET_DBCS,                                  L"CP932"           },
  /**
   * GBK.
   */
  {936,   CHARSET_DBCS,                                  L"CP936"           },
  /**
   * Unified Hangul Code (extended Wansung, KS X 1001).
   */
  {949,   CHARSET_DBCS,                                  L"CP949"           },
  /**
   * BIG5.
   */
  {950,   CHARSET_DBCS,                                  L"CP950"           },
  /**
   * IBM Turkish.
   */
  {1026,  CHARSET_EBCDIC_FULL,                           L"IBM-1026"        },
  /**
   * IBM Latin 1.
   */
  {1047,  CHARSET_EBCDIC_FULL,                           L"IBM-1047"        },
  /**
   * IBM-037 + Euro.
   */
  {1140,  CHARSET_EBCDIC_FULL,                           L"IBM-1140"        },
  /**
   * IBM-273 + Euro.
   */
  {1141,  CHARSET_EBCDIC_FULL,                           L"IBM-1141"        },
  /**
   * IBM-277 + Euro.
   */
  {1142,  CHARSET_EBCDIC_FULL,                           L"IBM-1142"        },
  /**
   * IBM-278 + Euro.
   */
  {1143,  CHARSET_EBCDIC_FULL,                           L"IBM-1143"        },
  /**
   * IBM-280 + Euro.
   */
  {1144,  CHARSET_EBCDIC_FULL,                           L"IBM-1144"        },
  /**
   * IBM-284 + Euro.
   */
  {1145,  CHARSET_EBCDIC_FULL,                           L"IBM-1145"        },
  /**
   * IBM-285 + Euro.
   */
  {1146,  CHARSET_EBCDIC_FULL,                           L"IBM-1146"        },
  /**
   * IBM-297 + Euro.
   */
  {1147,  CHARSET_EBCDIC_FULL,                           L"IBM-1147"        },
  /**
   * IBM-500 + Euro.
   */
  {1148,  CHARSET_EBCDIC_FULL,                           L"IBM-1148"        },
  /**
   * IBM-871 + Euro.
   */
  {1149,  CHARSET_EBCDIC_FULL,                           L"IBM-1149"        },
  /**
   * UTF-16LE.
   */
  {1200,  P32_CHARSET_REJECT_MANAGED,                    L"UTF-16LE"        },
  /**
   * UTF-16BE.
   */
  {1201,  P32_CHARSET_REJECT_MANAGED,                    L"UTF-16BE"        },
  {1250,  CHARSET_SBCS_FULL,                             L"CP1250"          },
  {1251,  CHARSET_SBCS_FULL,                             L"CP1251"          },
  {1252,  CHARSET_SBCS_FULL,                             L"CP1252"          },
  {1253,  P32_CHARSET_SBCS,                              L"CP1253"          },
  {1254,  CHARSET_SBCS_FULL,                             L"CP1254"          },
  {1255,  P32_CHARSET_SBCS,                              L"CP1255"          },
  {1256,  CHARSET_SBCS_FULL,                             L"CP1256"          },
  {1257,  P32_CHARSET_SBCS,                              L"CP1257"          },
  {1258,  CHARSET_SBCS_FULL,                             L"CP1258"          },
  /**
   * Charset:  KS X 1001 (former KS C 5601)
   * Encoding: Johab
   */
  {1361,  CHARSET_DBCS,                                  L"JOHAB"           },
  /**
   * MAC Roman.
   */
  {10000, P32_CHARSET_SBCS,                              L"MACROMAN"        },
  /**
   * Charset:  JIS X 0208
   * Encoding: Shift JIS with extensions
   */
  {10001, CHARSET_DBCS | P32_CHARSET_REJECT_CRT,         L"SHIFT_JIS"       },
  /**
   * Charset: BIG5 with extensions
   */
  {10002, CHARSET_DBCS,                                  L"BIG5"            },
  /**
   * Charset:  KS X 1001 (former KS C 5601)
   * Encoding: EUC-KR (Wansung)
   */
  {10003, CHARSET_DBCS,                                  L"EUC-KR"          },
  /**
   * MAC Arabic.
   */
  {10004, P32_CHARSET_SBCS,                              L"MACARABIC"       },
  /**
   * MAC Hebrew.
   */
  {10005, P32_CHARSET_SBCS,                              L"MACHEBREW"       },
  /**
   * MAC Greek.
   */
  {10006, P32_CHARSET_SBCS,                              L"MACGREEK"        },
  /**
   * MAC Cyrillic.
   */
  {10007, CHARSET_SBCS_FULL,                             L"MACCYRILLIC"     },
  /**
   * Charset:  GB2312 (GB_2312-80)
   * Encoding: EUC-CN
   */
  {10008, CHARSET_DBCS,                                  L"EUC-CN"          },
  /**
   * MAC Romanian.
   */
  {10010, P32_CHARSET_SBCS,                              L"MACROMANIA"      },
  /**
   * MAC Ukrainian.
   */
  {10017, CHARSET_SBCS_FULL,                             L"MACUKRAINE"      },
  /**
   * MAC Thai.
   *
   * Charset: ISO-8859-11 (derived from TIS-620) with extensions
   */
  {10021, P32_CHARSET_SBCS,                              L"MACTHAI"         },
  /**
   * MAC Central Europe.
   */
  {10029, CHARSET_SBCS_FULL,                             L"MACCENTRALEUROPE"},
  /**
   * MAC Icelandian.
   */
  {10079, P32_CHARSET_SBCS,                              L"MACICELAND"      },
  /**
   * MAC Turkish.
   */
  {10081, P32_CHARSET_SBCS,                              L"MACTURKISH"      },
  /**
   * MAC Croatian.
   */
  {10082, P32_CHARSET_SBCS,                              L"MACCROATIAN"     },
  /**
   * UTF-32LE.
   */
  {12000, P32_CHARSET_REJECT_MANAGED,                    L"UTF-32LE"        },
  /**
   * UTF-32BE.
   */
  {12001, P32_CHARSET_REJECT_MANAGED,                    L"UTF-32BE"        },
  /**
   * TODO: ???
   */
  {20000, CHARSET_DBCS,                                  L"20000"           },
  {20001, CHARSET_DBCS,                                  L"20001"           },
  {20002, CHARSET_DBCS,                                  L"20002"           },
  {20003, CHARSET_DBCS,                                  L"20003"           },
  {20004, CHARSET_DBCS,                                  L"20004"           },
  {20005, CHARSET_DBCS,                                  L"20005"           },
  /**
   * IA5 IRV (T.50).
   */
  {20105, P32_CHARSET_SBCS | P32_CHARSET_REJECT_ASCII,   NULL               },
  /**
   * IA5 German (T.50).
   */
  {20106, P32_CHARSET_SBCS | P32_CHARSET_REJECT_ASCII,   NULL               },
  /**
   * IA5 Swedish (T.50).
   */
  {20107, P32_CHARSET_SBCS | P32_CHARSET_REJECT_ASCII,   NULL               },
  /**
   * IA5 Norwegian (T.50).
   */
  {20108, P32_CHARSET_SBCS | P32_CHARSET_REJECT_ASCII,   NULL               },
  /**
   * ASCII.
   */
  {20127, P32_CHARSET_SBCS,                              L"ASCII"           },
  /**
   * T.61.
   */
  {20261, CHARSET_DBCS | P32_CHARSET_REJECT_ASCII,       L"ISO-IR-103"      },
  /**
   * T.51.
   */
  {20269, P32_CHARSET_SBCS,                              L"ISO-IR-156"      },
  /**
   * IBM Germany.
   */
  {20273, CHARSET_EBCDIC_FULL,                           L"IBM-273"         },
  /**
   * IBM Denmark/Norway.
   */
  {20277, CHARSET_EBCDIC_FULL,                           L"IBM-277"         },
  /**
   * IBM Finland/Sweden.
   */
  {20278, CHARSET_EBCDIC_FULL,                           L"IBM-278"         },
  /**
   * IBM Italy.
   */
  {20280, CHARSET_EBCDIC_FULL,                           L"IBM-280"         },
  /**
   * IBM Latin America/Spain.
   */
  {20284, CHARSET_EBCDIC_FULL,                           L"IBM-284"         },
  /**
   * IBM United Kingdom.
   */
  {20285, CHARSET_EBCDIC_FULL,                           L"IBM-285"         },
  /**
   * IBM Japanese Katakana Extended.
   */
  {20290, CHARSET_EBCDIC_FULL,                           L"IBM-290"         },
  /**
   * IBM France.
   */
  {20297, CHARSET_EBCDIC_FULL,                           L"IBM-297"         },
  /**
   * IBM Arabic.
   */
  {20420, P32_CHARSET_EBCDIC | P32_CHARSET_REJECT_ASCII, L"IBM-420"         },
  /**
   * IBM Greek.
   */
  {20423, P32_CHARSET_EBCDIC | P32_CHARSET_REJECT_ASCII, L"IBM-423"         },
  /**
   * IBM Hebrew.
   */
  {20424, CHARSET_EBCDIC_FULL,                           L"IBM-424"         },
  /**
   * IBM Korean Extended.
   */
  {20833, CHARSET_EBCDIC_FULL,                           L"IBM-833"         },
  /**
   * IBM Thai.
   */
  {20838, CHARSET_EBCDIC_FULL,                           L"IBM-838"         },
  /**
   * KOI8-R.
   */
  {20866, CHARSET_SBCS_FULL,                             L"KOI8-R"          },
  /**
   * IBM Icelandic.
   */
  {20871, CHARSET_EBCDIC_FULL,                           L"IBM-871"         },
  /**
   * IBM Russian.
   */
  {20880, CHARSET_EBCDIC_FULL,                           L"IBM-880"         },
  /**
   * IBM Turkish.
   */
  {20905, CHARSET_EBCDIC_FULL,                           L"IBM-905"         },
  /**
   * IBM Latin 1 (+ Euro).
   */
  {20924, CHARSET_EBCDIC_FULL,                           L"IBM-924"         },
  /**
   * Charset:  JIS X 0208
   * Encoding: EUC-JP.
   */
  {20932, CHARSET_DBCS,                                  L"EUC-JP"          },
  /**
   * Charset:  GB2312 (GB_2312-80)
   * Encoding: EUC-CN
   */
  {20936, CHARSET_DBCS,                                  L"EUC-CN"          },
  /**
   * Charset:  KS X 1001 (former KS C 5601)
   * Encoding: EUC-KR (Wansung)
   */
  {20949, CHARSET_DBCS,                                  L"EUC-KR"          },
  /**
   * IBM Serbian/Bulgarian (Cyrillic).
   */
  {21025, CHARSET_EBCDIC_FULL,                           L"IBM-1025"        },
  /**
   * IBM Japanese Katakana (incomplete).
   */
  {21027, P32_CHARSET_EBCDIC | P32_CHARSET_REJECT_ASCII, L"IBM-1027"        },
  /**
   * KOI8-U.
   */
  {21866, CHARSET_SBCS_FULL,                             L"KOI8-U"          },
  /**
   * ISO-8859-1.
   */
  {28591, CHARSET_SBCS_FULL,                             L"ISO-8859-1"      },
  /**
   * ISO-8859-2.
   */
  {28592, CHARSET_SBCS_FULL,                             L"ISO-8859-2"      },
  /**
   * ISO-8859-3.
   */
  {28593, CHARSET_SBCS_FULL,                             L"ISO-8859-3"      },
  /**
   * ISO-8859-4.
   */
  {28594, CHARSET_SBCS_FULL,                             L"ISO-8859-4"      },
  /**
   * ISO-8859-5.
   */
  {28595, CHARSET_SBCS_FULL,                             L"ISO-8859-5"      },
  /**
   * ISO-8859-6.
   */
  {28596, CHARSET_SBCS_FULL,                             L"ISO-8859-6"      },
  /**
   * ISO-8859-7.
   */
  {28597, CHARSET_SBCS_FULL,                             L"ISO-8859-7"      },
  /**
   * ISO-8859-8.
   */
  {28598, CHARSET_SBCS_FULL,                             L"ISO-8859-8"      },
  /**
   * ISO-8859-9.
   */
  {28599, CHARSET_SBCS_FULL,                             L"ISO-8859-9"      },
  /**
   * ISO-8859-13.
   */
  {28603, CHARSET_SBCS_FULL,                             L"ISO-8859-13"     },
  /**
   * ISO-8859-15.
   */
  {28605, CHARSET_SBCS_FULL,                             L"ISO-8859-15"     },
  /**
   * TODO: ???
   */
  {29001, P32_CHARSET_REJECT_LEGACY,                     NULL               },
  /**
   * ISO-8859-8-I.
   */
  {38598, P32_CHARSET_SBCS,                              L"ISO-8859-8-I"    },
  /**
   * TODO: ISO-2022-JP.
   */
  {50220, P32_CHARSET_REJECT_BROKEN,                     NULL               },
  {50221, P32_CHARSET_REJECT_BROKEN,                     NULL               },
  {50222, P32_CHARSET_REJECT_BROKEN,                     NULL               },
  /**
   * ISO-2022-KR.
   */
  {50225, P32_CHARSET_REJECT_BROKEN,                     NULL               },
  /**
   * TODO: ISO-2022-CN.
   */
  {50227, P32_CHARSET_REJECT_BROKEN,                     NULL               },
  {50229, P32_CHARSET_REJECT_BROKEN,                     NULL               },
  /**
   * EBCDIC Japanese (Katakana) Extended.
   */
  {50930, CHARSET_EBCDIC | P32_CHARSET_REJECT_LEGACY,    NULL               },
  /**
   * EBCDIC U.S./Canada and Japanese.
   */
  {50931, CHARSET_EBCDIC | P32_CHARSET_REJECT_LEGACY,    NULL               },
  /**
   * EBCDIC Korean Extended and Korean.
   */
  {50933, CHARSET_EBCDIC | P32_CHARSET_REJECT_LEGACY,    NULL               },
  /**
   * EBCDIC Simplified Chinese Extended and Simplified Chinese.
   */
  {50935, CHARSET_EBCDIC | P32_CHARSET_REJECT_LEGACY,    NULL               },
  /**
   * EBCDIC Simplified Chinese.
   */
  {50936, CHARSET_EBCDIC | P32_CHARSET_REJECT_LEGACY,    NULL               },
  /**
   * EBCDIC U.S./Canada and Traditional Chinese.
   */
  {50937, CHARSET_EBCDIC | P32_CHARSET_REJECT_LEGACY,    NULL               },
  /**
   * EBCDIC Japanese (Latin) Extended and Japanese.
   */
  {50939, CHARSET_EBCDIC | P32_CHARSET_REJECT_LEGACY,    NULL               },
  /**
   * TODO: EUC-JP.
   */
  {51932, P32_CHARSET_REJECT_UNSUPPORTED,                L"EUC-JP"          },
  /**
   * TODO: EUC-CN.
   */
  {51936, P32_CHARSET_REJECT_UNSUPPORTED,                L"EUC-CN"          },
  /**
   * Wansung, EUC-KR (KS X 1001).
   */
  {51949, CHARSET_DBCS,                                  L"EUC-KR"          },
  /**
   * TODO: EUC-TW.
   */
  {51950, P32_CHARSET_REJECT_UNSUPPORTED,                L"EUC-TW"          },
  /**
   * Charset:  GB2312 (GB_2312-80)
   * Encoding: HZ-GB-2312
   */
  {52936, P32_CHARSET_REJECT_UNSUPPORTED,                L"HZ-GB-2312"      },
  /**
   * GB18030.
   */
  {54936, P32_CHARSET_REJECT_BROKEN,                     L"GB18030"         },
  /**
   * TODO: ???
   */
  {55000, P32_CHARSET_REJECT_UNDOCUMENTED,               NULL               },
  {55001, P32_CHARSET_REJECT_UNDOCUMENTED,               NULL               },
  {55002, P32_CHARSET_REJECT_UNDOCUMENTED,               NULL               },
  {55003, P32_CHARSET_REJECT_UNDOCUMENTED,               NULL               },
  {55004, P32_CHARSET_REJECT_UNDOCUMENTED,               NULL               },
  /**
   * ISCII Devanagari.
   */
  {57002, P32_CHARSET_REJECT_UNSUPPORTED,                NULL               },
  /**
   * ISCII Bangla.
   */
  {57003, P32_CHARSET_REJECT_UNSUPPORTED,                NULL               },
  /**
   * ISCII Tamil.
   */
  {57004, P32_CHARSET_REJECT_UNSUPPORTED,                NULL               },
  /**
   * ISCII Telugu.
   */
  {57005, P32_CHARSET_REJECT_UNSUPPORTED,                NULL               },
  /**
   * ISCII Assamese.
   */
  {57006, P32_CHARSET_REJECT_UNSUPPORTED,                NULL               },
  /**
   * ISCII Odia.
   */
  {57007, P32_CHARSET_REJECT_UNSUPPORTED,                NULL               },
  /**
   * ISCII Kannada.
   */
  {57008, P32_CHARSET_REJECT_UNSUPPORTED,                NULL               },
  /**
   * ISCII Malayalam.
   */
  {57009, P32_CHARSET_REJECT_UNSUPPORTED,                NULL               },
  /**
   * ISCII Gujarati.
   */
  {57010, P32_CHARSET_REJECT_UNSUPPORTED,                NULL               },
  /**
   * ISCII Punjabi.
   */
  {57011, P32_CHARSET_REJECT_UNSUPPORTED,                NULL               },
  /**
   * UTF-7.
   */
  {65000, P32_CHARSET_REJECT_UNSUPPORTED,                L"UTF-7"           },
  /**
   * UTF-8.
   */
  {65001, 0,                                             L"UTF-8"           },
};

bool p32_charset_usable (uint32_t codePage, int rejectMask, int allowMask) {
  const int defaultRejectMask =
    (P32_CHARSET_REJECT_ASCII | P32_CHARSET_REJECT_BROKEN | P32_CHARSET_REJECT_LEGACY | P32_CHARSET_REJECT_MANAGED
     | P32_CHARSET_REJECT_UNDOCUMENTED | P32_CHARSET_REJECT_UNSUPPORTED);

  const CodePageInfo *info = NULL;

  for (size_t i = 0; i < _countof (Charsets); ++i) {
    if (Charsets[i].CodePage == codePage) {
      info = &Charsets[i];
      break;
    }
  }

  assert (info != NULL);

  if (info == NULL || (info->Flags & ((defaultRejectMask & ~allowMask) | rejectMask))) {
    return false;
  }

  return true;
}

void p32_charset_conversion_flags (Charset *charset, bool bestFit) {
  uint32_t toMb = 0;
  uint32_t toWc = 0;

  /**
   * No flags allowed with the following code pages.
   */
  switch (charset->CodePage) {
    case 50220: /* ISO-2022-JP ???  */
    case 50221: /* ISO-2022-JP ???  */
    case 50222: /* ISO-2022-JP ???  */
    case 50225: /* ISO-2022-KR      */
    case 50227: /* ISO-2022-CH ???  */
    case 50229: /* ISO-2022-CH ???  */
    case 57002: /* ISCII Devanagari */
    case 57003: /* ISCII Bangla     */
    case 57004: /* ISCII Tamil      */
    case 57005: /* ISCII Telugu     */
    case 57006: /* ISCII Assamese   */
    case 57007: /* ISCII Odia       */
    case 57008: /* ISCII Kannada    */
    case 57009: /* ISCII Malayalam  */
    case 57010: /* ISCII Gujarati   */
    case 57011: /* ISCII Punjabi    */
    case 65000: /* UTF-7            */
      goto done;
  }

  toWc |= MB_ERR_INVALID_CHARS;

  switch (charset->CodePage) {
    case 54936: /* GB18030 */
    case 65001: /* UTF-8   */
#if _WIN32_WINNT >= _WIN32_WINNT_VISTA
      /**
       * WC_ERR_INVALID_CHARS is only allowed with GB18030 and UTF-8, and this
       * is the only flag allowed to be used with these code pages.
       */
      toMb |= WC_ERR_INVALID_CHARS;
#endif
      goto done;
  }

  if (!bestFit) {
    toMb |= WC_NO_BEST_FIT_CHARS;
  }

  /**
   * The following SBCS code pages use nonspacing characters.
   *
   * We avoid using `WC_COMPOSITECHECK` with these code pages as it may lead
   * to unexpected results.
   */
  switch (charset->CodePage) {
    case 1258:
    case 20269:
      charset->ToMultiByte = toMb;
      charset->ToWideChar  = toWc;
      goto done;
  }

  const CodePageInfo *info = NULL;

  for (size_t i = 0; i < _countof (Charsets); ++i) {
    if (Charsets[i].CodePage == charset->CodePage) {
      info = &Charsets[i];
      break;
    }
  }

  assert (info != NULL);

  if (info != NULL && (info->Flags & (P32_CHARSET_SBCS | P32_CHARSET_DBCS))) {
    toMb |= (WC_COMPOSITECHECK | WC_SEPCHARS);
  }

done:
  charset->ToMultiByte = toMb;
  charset->ToWideChar  = toWc;
}

bool p32_charset_info (Charset *charset) {
  CPINFO cpInfo = {0};

  if (!GetCPInfo (charset->CodePage, &cpInfo)) {
    return false;
  }

  charset->MaxLength = cpInfo.MaxCharSize;
  memcpy (charset->Map, cpInfo.LeadByte, MAX_LEADBYTES);

  p32_charset_conversion_flags (charset, false);

  if (charset->CodePage == CP_UTF8) {
    charset->ReplacementChar.Length  = 3;
    charset->ReplacementChar.Char[0] = (char) (unsigned) 0xEF;
    charset->ReplacementChar.Char[1] = (char) (unsigned) 0xBF;
    charset->ReplacementChar.Char[2] = (char) (unsigned) 0xBD;
  } else {
    charset->ReplacementChar.Length  = 1;
    charset->ReplacementChar.Char[0] = (char) cpInfo.DefaultChar[0];
  }

  return true;
}

bool p32_charset_name (wchar_t **address, uintptr_t heap, uint32_t codePage) {
  const CodePageInfo *info = NULL;

  for (size_t i = 0; i < _countof (Charsets); ++i) {
    if (Charsets[i].CodePage == codePage) {
      info = &Charsets[i];
      break;
    }
  }

  assert (info != NULL);

  if (info == NULL || info->CharsetName == NULL) {
    if (p32_private_aswprintf (address, heap, L"CP%u", codePage) == -1) {
      return false;
    }
  } else {
    if (p32_private_wcsdup (address, info->CharsetName, heap) == -1) {
      return false;
    }
  }

  return true;
}
