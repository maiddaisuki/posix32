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

/**
 * File Summary:
 *
 * This file contains locale strings for use with ancient CRTs:
 *
 *  - crtdll.dll
 *  - msvcrt10.dll
 *  - msvcrt20.dll
 *  - msvcrt40.dll
 *  - msvcrt.dll, when targeting Windows NT 4.0 or older
 *
 * These CRTs provide very limited support for locale strings using
 * `Language_Country.CodePage` format.
 *
 * Instead, they accept, "language strings" as documented by Microsoft:
 * https://learn.microsoft.com/en-us/cpp/c-runtime-library/language-strings
 *
 * When such "language string" is passed to CRT's `[_w]setlocale`, depending
 * on CRT, it will use either locale's default ANSI or OEM code page.
 *
 * This means that a "language string" can only be used when locale is created
 * with that code page.
 *
 * If locale is created using any other code page or this locale does not have
 * a "language string", we will try to use `Language_Country.CodePage` format,
 * which may or may not work with specific CRT.
 */

typedef struct CrtLocaleString {
  LanguageIndex  Language;
  CountryIndex   Country;
  const wchar_t *LocaleString;
} CrtLocaleString;

/**
 * NOTE: This data was obtained by testsing. Running code on old versions of
 * Windows may result in different behavior.
 *
 * This table contains all locales which could be set with CRTs listed above,
 * as well as locales for which Microsoft documents "language string".
 *
 * Comment preceding each item tells:
 *
 * 1. Which CRTs support this locale.
 *
 *  If this is simple string like `crtdll.dll`, then this locale is supported
 *  in crtdll.dll and all later CRTs.
 *
 * 2. Whether this locale can be set using `Language_Country.CodePage` format.
 *
 *  If this is simple string `yes` or `no`, then this locale is respectively
 *  can or cannot be set using `Langauge_Country.CodePage` format in all CRTs
 *  which support it.
 *
 *  Otherwise, it is the name of earliest CRT which supports using this locale
 *  with `Language_Country.CodePage` format.
 *
 * If there is no comment preceding an item, then that locale is not supported
 * by any of CRTs listed above. We still provide "language string" for them,
 * as using them has the highest chance of success.
 */
static const CrtLocaleString CrtLocaleStrings[] = {
  /**
   * msvcrt10.dll only | ll_CC: no
   */
  {ar, SA,    L"arabic"                    },
  /**
   * msvcrt10.dll only | ll_CC: no
   */
  {bg, BG,    L"bulgarian"                 },
  /**
   * msvcrt10.dll only | ll_CC: yes
   */
  {ca, ES,    L"catalan"                   },
  /**
   * crtdll.dll | ll_CC: no
   */
  {cs, CZ,    L"czech"                     },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {da, DK,    L"danish"                    },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {de, AT,    L"german-austrian"           },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {de, CH,    L"german-swiss"              },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {de, DE,    L"german"                    },
  {de, LI,    L"german-lichtenstein"       },
  {de, LU,    L"german-luxembourg"         },
  /**
   * crtdll.dll | ll_CC: msvcrt20.dll
   */
  {el, GR,    L"greek"                     },
  {en, R_029, L"english-caribbean"         },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {en, AU,    L"english-aus"               },
  {en, BZ,    L"english-belize"            },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {en, CA,    L"english-can"               },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {en, GB,    L"english-uk"                },
  /**
   * msvcrt20.dll | ll_CC: yes
   *
   * crtdll.dll and msvcrt10.dll accept locale string `English_Ireland`, but
   * set locale to `English_United Kingdom`.
   */
  {en, IE,    L"english-ire"               },
  {en, JM,    L"english-jamaica"           },
#if 0
  /**
   * msvcrt20.dll only | ll_CC: yes
   */
  {en, SG,    NULL                         },
#endif
  /**
   * crtdll.dll | ll_CC: yes
   */
  {en, NZ,    L"english-nz"                },
  {en, TT,    L"english-trinidad y tobago" },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {en, US,    L"english-us"                },
  {en, ZA,    L"english-south africa"      },
  {es, AR,    L"spanish-argentina"         },
  {es, BO,    L"spanish-bolivia"           },
  {es, CL,    L"spanish-chile"             },
  {es, CO,    L"spanish-colombia"          },
  {es, CR,    L"spanish-costa rica"        },
  {es, DO,    L"spanish-dominican republic"},
  {es, EC,    L"spanish-ecuador"           },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {es, ES,    L"spanish-modern"            },
  {es, GT,    L"spanish-guatemala"         },
  {es, HN,    L"spanish-honduras"          },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {es, MX,    L"spanish-mexican"           },
  {es, NI,    L"spanish-nicaragua"         },
  {es, PA,    L"spanish-panama"            },
  {es, PE,    L"spanish-peru"              },
  {es, PR,    L"spanish-puerto rico"       },
  {es, PY,    L"spanish-paraguay"          },
  {es, SV,    L"spanish-el salvador"       },
#if 0
  /**
   * crtdll.dll | ll_CC: yes
   */
  {es, US,    NULL                         },
#endif
  {es, UY,    L"spanish-uruguay"           },
  {es, VE,    L"spanish-venezuela"         },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {fi, FI,    L"finnish"                   },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {fr, BE,    L"french-belgian"            },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {fr, CA,    L"french-canadian"           },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {fr, CH,    L"french-swiss"              },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {fr, FR,    L"french"                    },
  {fr, LU,    L"french-luxembourg"         },
  /**
   * msvcrt10.dll only | ll_CC: no
   */
  {he, IL,    L"hebrew"                    },
  /**
   * msvcrt10.dll only | ll_CC: no
   */
  {hr, HR,    L"croatian"                  },
  /**
   * crtdll.dll | ll_CC: msvcrt20.dll
   */
  {hu, HU,    L"hungarian"                 },
  /**
   * msvcrt10.dll only | ll_CC: no
   */
  {id, ID,    L"indonesian"                },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {is, IS,    L"icelandic"                 },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {it, CH,    L"italian-swiss"             },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {it, IT,    L"italian"                   },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {ja, JP,    L"japanese"                  },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {ko, KR,    L"korean"                    },
  /**
   * crtdll.dll | ll_CC: no
   */
  {nb, NO,    L"norwegian-bokmal"          },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {nl, BE,    L"dutch-belgian"             },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {nl, NL,    L"dutch"                     },
  /**
   * crtdll.dll | ll_CC: no
   */
  {nn, NO,    L"norwegian-nynorsk"         },
  /**
   * crtdll.dll | ll_CC: msvcrt20.dll
   */
  {pl, PL,    L"polish"                    },
  /**
   * crtdll.dll | ll_CC: yes
   */
  {pt, BR,    L"portuguese-brazilian"      },
#if !P32_MSVCRT
  /**
   * crtdll.dll | ll_CC: yes
   *
   * Versions of msvcrt.dll which fully support `Language_Country.CodePage`
   * format will set locale to `Portuguese_Brazil` when string `portuguese`
   * is passed to `[_w]setlocale`.
   *
   * We have to rely on using `Portuguese_Portugal` with ancient versions
   * of msvcrt.dll.
   */
  {pt, PT,    L"portuguese"                },
#endif
  /**
   * msvcrt10.dll only | ll_CC: no
   */
  {ro, RO,    L"romanian"                  },
  /**
   * crtdll.dll | ll_CC: msvcrt20.dll
   */
  {ru, RU,    L"russian"                   },
  /**
   * crtdll.dll | ll_CC: no
   */
  {sk, SK,    L"slovak"                    },
  /**
   * msvcrt10.dll only | ll_CC: no
   */
  {sq, AL,    L"albanian"                  },
#if 0
  /**
   * crtdll.dll | ll_CC: yes
   *
   * Language string "swedish-finland" is not recognized by CRTs which do not
   * fully support `Langauge_Country` format.
   */
  {sv, FI,    L"swedish-finland"           },
#endif
  /**
   * crtdll.dll | ll_CC: yes
   */
  {sv, SE,    L"swedish"                   },
  /**
   * msvcrt10.dll only | ll_CC: no
   */
  {th, TH,    L"thai"                      },
  /**
   * crtdll.dll | ll_CC: no
   */
  {tr, TR,    L"turkish"                   },
  /**
   * msvcrt10.dll only | ll_CC: no
   */
  {ur, PK,    L"urdu"                      },
  /**
   * crtdll.dll | ll_CC: no
   */
  {zh, CN,    L"chinese-simplified"        },
  /**
   * msvcrt20.dll | ll_CC: no
   */
  {zh, HK,    L"chinese-hongkong"          },
  /**
   * msvcrt20.dll | ll_CC: no
   */
  {zh, SG,    L"chinese-singapore"         },
  /**
   * crtdll.dll | ll_CC: no
   */
  {zh, TW,    L"chinese-traditional"       },
};
