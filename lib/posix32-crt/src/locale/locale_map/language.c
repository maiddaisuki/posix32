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
#include <stdlib.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "locale-internal.h"

#if P32_LOCALE_NAMES
#define LANGUAGE_DECL(name, ll, id) {TEXT (name), TEXT (ll)}
#else
#define LANGUAGE_DECL(name, ll, id) {TEXT (name), TEXT (ll), (uint16_t) id}
#endif

static const Language Languages[] = {
  /* clang-format off */
  LANGUAGE_DECL ("Afar",                        "aa",  -1),
  LANGUAGE_DECL ("Afrikaans",                   "af",  LANG_AFRIKAANS),
  LANGUAGE_DECL ("Aghem",                       "agq", -1),
  LANGUAGE_DECL ("Akan",                        "ak",  -1),
  LANGUAGE_DECL ("Amharic",                     "am",  LANG_AMHARIC),
  LANGUAGE_DECL ("Arabic",                      "ar",  LANG_ARABIC),
  LANGUAGE_DECL ("Mapuche",                     "arn", LANG_MAPUDUNGUN),
  LANGUAGE_DECL ("Assamese",                    "as",  LANG_ASSAMESE),
  LANGUAGE_DECL ("Asu",                         "asa", -1),
  LANGUAGE_DECL ("Asturian",                    "ast", -1),
  LANGUAGE_DECL ("Azerbaijani",                 "az",  LANG_AZERBAIJANI),
  LANGUAGE_DECL ("Bashkir",                     "ba",  LANG_BASHKIR),
  LANGUAGE_DECL ("Basaa",                       "bas", -1),
  LANGUAGE_DECL ("Belarusian",                  "be",  LANG_BELARUSIAN),
  LANGUAGE_DECL ("Bemba",                       "bem", -1),
  LANGUAGE_DECL ("Bena",                        "bez", -1),
  LANGUAGE_DECL ("Bulgarian",                   "bg",  LANG_BULGARIAN),
  LANGUAGE_DECL ("Haryanvi",                    "bgc", -1),
  LANGUAGE_DECL ("Bhojpuri",                    "bho", -1),
  LANGUAGE_DECL ("Edo",                         "bin", 0x66),
  LANGUAGE_DECL ("Bamanankan",                  "bm",  -1),
  LANGUAGE_DECL ("Bangla",                      "bn",  LANG_BANGLA),
  LANGUAGE_DECL ("Tibetan",                     "bo",  LANG_TIBETAN),
  LANGUAGE_DECL ("Breton",                      "br",  LANG_BRETON),
  LANGUAGE_DECL ("Bodo",                        "brx", -1),
  LANGUAGE_DECL ("Bosnian",                     "bs",  LANG_BOSNIAN),
  LANGUAGE_DECL ("Blin",                        "byn", -1),
  /**
   * Same as `LANG_VALENCIAN`.
   */
  LANGUAGE_DECL ("Catalan",                     "ca",  LANG_CATALAN),
  LANGUAGE_DECL ("Chakma",                      "ccp", -1),
  LANGUAGE_DECL ("Chechen",                     "ce",  -1),
  LANGUAGE_DECL ("Cebuano",                     "ceb", -1),
  LANGUAGE_DECL ("Chiga",                       "cgg", -1),
  LANGUAGE_DECL ("Cherokee",                    "chr", LANG_CHEROKEE),
  LANGUAGE_DECL ("Corsican",                    "co",  LANG_CORSICAN),
  LANGUAGE_DECL ("Czech",                       "cs",  LANG_CZECH),
  LANGUAGE_DECL ("Church Slavic",               "cu",  -1),
  LANGUAGE_DECL ("Chuvash",                     "cv",  -1),
  LANGUAGE_DECL ("Welsh",                       "cy",  LANG_WELSH),
  LANGUAGE_DECL ("Danish",                      "da",  LANG_DANISH),
  LANGUAGE_DECL ("Taita",                       "dav", -1),
  LANGUAGE_DECL ("German",                      "de",  LANG_GERMAN),
  LANGUAGE_DECL ("Zarma",                       "dje", -1),
  LANGUAGE_DECL ("Dogri",                       "doi", -1),
  LANGUAGE_DECL ("Lower Sorbian",               "dsb", LANG_LOWER_SORBIAN),
  LANGUAGE_DECL ("Duala",                       "dua", -1),
  LANGUAGE_DECL ("Divehi",                      "dv",  LANG_DIVEHI),
  LANGUAGE_DECL ("Jola-Fonyi",                  "dyo", -1),
  LANGUAGE_DECL ("Dzongkha",                    "dz",  LANG_TIBETAN),
  LANGUAGE_DECL ("Embu",                        "ebu", -1),
  LANGUAGE_DECL ("Ewe",                         "ee",  -1),
  LANGUAGE_DECL ("Greek",                       "el",  LANG_GREEK),
  LANGUAGE_DECL ("English",                     "en",  LANG_ENGLISH),
  LANGUAGE_DECL ("Esperanto",                   "eo",  -1),
  LANGUAGE_DECL ("Spanish",                     "es",  LANG_SPANISH),
  LANGUAGE_DECL ("Estonian",                    "et",  LANG_ESTONIAN),
  LANGUAGE_DECL ("Basque",                      "eu",  LANG_BASQUE),
  LANGUAGE_DECL ("Ewondo",                      "ewo", -1),
  LANGUAGE_DECL ("Persian",                     "fa",  LANG_PERSIAN),
  LANGUAGE_DECL ("Fula",                        "ff",  LANG_FULAH),
  LANGUAGE_DECL ("Finnish",                     "fi",  LANG_FINNISH),
  LANGUAGE_DECL ("Filipino",                    "fil", LANG_FILIPINO),
  LANGUAGE_DECL ("Faroese",                     "fo",  LANG_FAEROESE),
  LANGUAGE_DECL ("French",                      "fr",  LANG_FRENCH),
  LANGUAGE_DECL ("Friulian",                    "fur", -1),
  LANGUAGE_DECL ("Frisian",                     "fy",  LANG_FRISIAN),
  LANGUAGE_DECL ("Irish",                       "ga",  LANG_IRISH),
  LANGUAGE_DECL ("Scottish Gaelic",             "gd",  LANG_SCOTTISH_GAELIC),
  LANGUAGE_DECL ("Galician",                    "gl",  LANG_GALICIAN),
  LANGUAGE_DECL ("Guarani",                     "gn",  0x74),
  LANGUAGE_DECL ("Swiss German",                "gsw", LANG_ALSATIAN),
  LANGUAGE_DECL ("Gujarati",                    "gu",  LANG_GUJARATI),
  LANGUAGE_DECL ("Gusii",                       "guz", -1),
  LANGUAGE_DECL ("Manx",                        "gv",  -1),
  LANGUAGE_DECL ("Hausa",                       "ha",  LANG_HAUSA),
  LANGUAGE_DECL ("Hawaiian",                    "haw", LANG_HAWAIIAN),
  LANGUAGE_DECL ("Hebrew",                      "he",  LANG_HEBREW),
  LANGUAGE_DECL ("Hindi",                       "hi",  LANG_HINDI),
  LANGUAGE_DECL ("Croatian",                    "hr",  LANG_CROATIAN),
  LANGUAGE_DECL ("Upper Sorbian",               "hsb", LANG_UPPER_SORBIAN),
  LANGUAGE_DECL ("Hungarian",                   "hu",  LANG_HUNGARIAN),
  LANGUAGE_DECL ("Armenian",                    "hy",  LANG_ARMENIAN),
  LANGUAGE_DECL ("Interlingua",                 "ia",  -1),
  LANGUAGE_DECL ("Ibibio",                      "ibb", 0x69),
  LANGUAGE_DECL ("Indonesian",                  "id",  LANG_INDONESIAN),
  LANGUAGE_DECL ("Igbo",                        "ig",  LANG_IGBO),
  LANGUAGE_DECL ("Yi",                          "ii",  LANG_YI),
  LANGUAGE_DECL ("Icelandic",                   "is",  LANG_ICELANDIC),
  LANGUAGE_DECL ("Italian",                     "it",  LANG_ITALIAN),
  LANGUAGE_DECL ("Inuktitut",                   "iu",  LANG_INUKTITUT),
  // LANGUAGE_DECL ("Invariant Language",          "x",   -1,),
  LANGUAGE_DECL ("Japanese",                    "ja",  LANG_JAPANESE),
  LANGUAGE_DECL ("Ngomba",                      "jgo", -1),
  LANGUAGE_DECL ("Machame",                     "jmc", -1),
  LANGUAGE_DECL ("Javanese",                    "jv",  -1),
  LANGUAGE_DECL ("Georgian",                    "ka",  LANG_GEORGIAN),
  LANGUAGE_DECL ("Kabyle",                      "kab", -1),
  LANGUAGE_DECL ("Kamba",                       "kam", -1),
  LANGUAGE_DECL ("Makonde",                     "kde", -1),
  LANGUAGE_DECL ("Kabuverdianu",                "kea", -1),
  LANGUAGE_DECL ("Kaingang",                    "kgp", -1),
  LANGUAGE_DECL ("Koyra Chiini",                "khq", -1),
  LANGUAGE_DECL ("Kikuyu",                      "ki",  -1),
  LANGUAGE_DECL ("Kazakh",                      "kk",  LANG_KAZAK),
  LANGUAGE_DECL ("Kako",                        "kkj", -1),
  LANGUAGE_DECL ("Kalaallisut",                 "kl",  LANG_GREENLANDIC),
  LANGUAGE_DECL ("Kalenjin",                    "kln", -1),
  LANGUAGE_DECL ("Khmer",                       "km",  LANG_KHMER),
  LANGUAGE_DECL ("Kannada",                     "kn",  LANG_KANNADA),
  LANGUAGE_DECL ("Korean",                      "ko",  LANG_KOREAN),
  LANGUAGE_DECL ("Konkani",                     "kok", LANG_KONKANI),
  LANGUAGE_DECL ("Kanuri",                      "kr",  0x71),
  LANGUAGE_DECL ("Kashmiri",                    "ks",  LANG_KASHMIRI),
  LANGUAGE_DECL ("Shambala",                    "ksb", -1),
  LANGUAGE_DECL ("Bafia",                       "ksf", -1),
  LANGUAGE_DECL ("Colognian",                   "ksh", -1),
  LANGUAGE_DECL ("Central Kurdish",             "ku",  LANG_CENTRAL_KURDISH),
  LANGUAGE_DECL ("Cornish",                     "kw",  -1),
  LANGUAGE_DECL ("Kyrgyz",                      "ky",  LANG_KYRGYZ),
  LANGUAGE_DECL ("Latin",                       "la",  0x76),
  LANGUAGE_DECL ("Langi",                       "lag", -1),
  LANGUAGE_DECL ("Luxembourgish",               "lb",  LANG_LUXEMBOURGISH),
  LANGUAGE_DECL ("Ganda",                       "lg",  -1),
  LANGUAGE_DECL ("Lakota",                      "lkt", -1),
  LANGUAGE_DECL ("Lingala",                     "ln",  -1),
  LANGUAGE_DECL ("Lao",                         "lo",  LANG_LAO),
  LANGUAGE_DECL ("Northern Luri",               "lrc", -1),
  LANGUAGE_DECL ("Lithuanian",                  "lt",  LANG_LITHUANIAN),
  LANGUAGE_DECL ("Luba-Katanga",                "lu",  -1),
  LANGUAGE_DECL ("Luo",                         "luo", -1),
  LANGUAGE_DECL ("Luyia",                       "luy", -1),
  LANGUAGE_DECL ("Latvian",                     "lv",  LANG_LATVIAN),
  LANGUAGE_DECL ("Maithili",                    "mai", -1),
  LANGUAGE_DECL ("Masai",                       "mas", -1),
  LANGUAGE_DECL ("Meru",                        "mer", -1),
  LANGUAGE_DECL ("Morisyen",                    "mfe", -1),
  LANGUAGE_DECL ("Malagasy",                    "mg",  -1),
  LANGUAGE_DECL ("Makhuwa-Meett",               "mgh", -1),
  LANGUAGE_DECL ("Metaʼ",                       "mgo", -1),
  LANGUAGE_DECL ("Māori",                       "mi",  LANG_MAORI),
  LANGUAGE_DECL ("Macedonian",                  "mk",  LANG_MACEDONIAN),
  LANGUAGE_DECL ("Malayalam",                   "ml",  LANG_MALAYALAM),
  LANGUAGE_DECL ("Mongolian",                   "mn",  LANG_MONGOLIAN),
  LANGUAGE_DECL ("Manipuri",                    "mni", 0x58),
  LANGUAGE_DECL ("Mohawk",                      "moh", LANG_MOHAWK),
  LANGUAGE_DECL ("Marathi",                     "mr",  LANG_MARATHI),
  LANGUAGE_DECL ("Malay",                       "ms",  LANG_MALAY),
  LANGUAGE_DECL ("Maltese",                     "mt",  LANG_MALTESE),
  LANGUAGE_DECL ("Mundang",                     "mua", -1),
  LANGUAGE_DECL ("Burmese",                     "my",  0x55),
  LANGUAGE_DECL ("Mazanderani",                 "mzn", -1),
  LANGUAGE_DECL ("Nama",                        "naq", -1),
  LANGUAGE_DECL ("Norwegian Bokmål",            "nb",  LANG_NORWEGIAN),
  LANGUAGE_DECL ("North Ndebele",               "nd",  -1),
  LANGUAGE_DECL ("Low German",                  "nds", -1),
  LANGUAGE_DECL ("Nepali",                      "ne",  LANG_NEPALI),
  LANGUAGE_DECL ("Dutch",                       "nl",  LANG_DUTCH),
  LANGUAGE_DECL ("Kwasio",                      "nmg", -1),
  LANGUAGE_DECL ("Norwegian Nynorsk",           "nn",  LANG_NORWEGIAN),
  LANGUAGE_DECL ("Ngiemboon",                   "nnh", -1),
  LANGUAGE_DECL ("N'ko",                        "nqo", -1),
  LANGUAGE_DECL ("South Ndebele",               "nr",  -1),
  LANGUAGE_DECL ("Sesotho sa Leboa",            "nso", LANG_SOTHO),
  LANGUAGE_DECL ("Nuer",                        "nus", -1),
  LANGUAGE_DECL ("Nyankole",                    "nyn", -1),
  LANGUAGE_DECL ("Occitan",                     "oc",  LANG_OCCITAN),
  LANGUAGE_DECL ("Oromo",                       "om",  0x72),
  LANGUAGE_DECL ("Odia",                        "or",  LANG_ODIA),
  LANGUAGE_DECL ("Ossetic",                     "os",  -1),
  LANGUAGE_DECL ("Punjabi",                     "pa",  LANG_PUNJABI),
  LANGUAGE_DECL ("Papiamento",                  "pap", 0x79),
  LANGUAGE_DECL ("Nigerian Pidgin",             "pcm", -1),
  LANGUAGE_DECL ("Polish",                      "pl",  LANG_POLISH),
  LANGUAGE_DECL ("Prussian",                    "prg", -1),
  /**
   * Locale `fa-AF` and `prs-AF` are the same locale.
   * Unfortunately, `fa-AF` and `fa-IR` use different LANG_* values.
   */
  LANGUAGE_DECL ("Persian",                     "fa",  LANG_DARI),
  LANGUAGE_DECL ("Pashto",                      "ps",  LANG_PASHTO),
  LANGUAGE_DECL ("Portuguese",                  "pt",  LANG_PORTUGUESE),
  LANGUAGE_DECL ("Kʼicheʼ",                     "quc", LANG_KICHE),
  LANGUAGE_DECL ("Quechua",                     "quz", LANG_QUECHUA),
  LANGUAGE_DECL ("Rajasthani",                  "raj", -1),
  LANGUAGE_DECL ("Romansh",                     "rm",  LANG_ROMANSH),
  LANGUAGE_DECL ("Rundi",                       "rn",  -1),
  LANGUAGE_DECL ("Romanian",                    "ro",  LANG_ROMANIAN),
  LANGUAGE_DECL ("Rombo",                       "rof", -1),
  LANGUAGE_DECL ("Russian",                     "ru",  LANG_RUSSIAN),
  LANGUAGE_DECL ("Kinyarwanda",                 "rw",  LANG_KINYARWANDA),
  LANGUAGE_DECL ("Rwa",                         "rwk", -1),
  LANGUAGE_DECL ("Sanskrit",                    "sa",  LANG_SANSKRIT),
  LANGUAGE_DECL ("Yakut",                       "sah", LANG_SAKHA),
  LANGUAGE_DECL ("Samburu",                     "saq", -1),
  LANGUAGE_DECL ("Santali",                     "sat", -1),
  LANGUAGE_DECL ("Sangu",                       "sbp", -1),
  LANGUAGE_DECL ("Sardinian",                   "sc",  -1),
  LANGUAGE_DECL ("Sindhi",                      "sd",  LANG_SINDHI),
  LANGUAGE_DECL ("Sami (Northern)",             "se",  LANG_SAMI),
  LANGUAGE_DECL ("Sena",                        "seh", -1),
  LANGUAGE_DECL ("Koyraboro Senni",             "ses", -1),
  LANGUAGE_DECL ("Sango",                       "sg",  -1),
  LANGUAGE_DECL ("Tachelhit",                   "shi", -1),
  LANGUAGE_DECL ("Sinhala",                     "si",  LANG_SINHALESE),
  LANGUAGE_DECL ("Slovak",                      "sk",  LANG_SLOVAK),
  LANGUAGE_DECL ("Slovenian",                   "sl",  LANG_SLOVENIAN),
  LANGUAGE_DECL ("Sami (Southern)",             "sma", LANG_SAMI),
  LANGUAGE_DECL ("Sami (Lule)",                 "smj", LANG_SAMI),
  LANGUAGE_DECL ("Sami (Inari)",                "smn", LANG_SAMI),
  LANGUAGE_DECL ("Sami (Skolt)",                "sms", LANG_SAMI),
  LANGUAGE_DECL ("Shona",                       "sn",  -1),
  LANGUAGE_DECL ("Somali",                      "so",  0x77),
  LANGUAGE_DECL ("Albanian",                    "sq",  LANG_ALBANIAN),
  LANGUAGE_DECL ("Serbian",                     "sr",  LANG_SERBIAN),
  LANGUAGE_DECL ("siSwati",                     "ss",  -1),
  LANGUAGE_DECL ("Saho",                        "ssy", -1),
  LANGUAGE_DECL ("Sesotho",                     "st",  0x30),
  LANGUAGE_DECL ("Sundanese",                   "su",  -1),
  LANGUAGE_DECL ("Swedish",                     "sv",  LANG_SWEDISH),
  LANGUAGE_DECL ("Kiswahili",                   "sw",  LANG_SWAHILI),
  LANGUAGE_DECL ("Syriac",                      "syr", LANG_SYRIAC),
  LANGUAGE_DECL ("Tamil",                       "ta",  LANG_TAMIL),
  LANGUAGE_DECL ("Telugu",                      "te",  LANG_TELUGU),
  LANGUAGE_DECL ("Teso",                        "teo", -1),
  LANGUAGE_DECL ("Tajik",                       "tg",  LANG_TAJIK),
  LANGUAGE_DECL ("Thai",                        "th",  LANG_THAI),
  LANGUAGE_DECL ("Tigrinya",                    "ti",  LANG_TIGRINYA),
  LANGUAGE_DECL ("Tigre",                       "tig", -1),
  LANGUAGE_DECL ("Turkmen",                     "tk",  LANG_TURKMEN),
  LANGUAGE_DECL ("Setswana",                    "tn",  LANG_TSWANA),
  LANGUAGE_DECL ("Tongan",                      "to",  -1),
  LANGUAGE_DECL ("Turkish",                     "tr",  LANG_TURKISH),
  LANGUAGE_DECL ("Tsonga",                      "ts",  0x31),
  LANGUAGE_DECL ("Tatar",                       "tt",  LANG_TATAR),
  LANGUAGE_DECL ("Tasawaq",                     "twq", -1),
  LANGUAGE_DECL ("Central Atlas Tamazight",     "tzm", LANG_TAMAZIGHT),
  LANGUAGE_DECL ("Uyghur",                      "ug",  LANG_UIGHUR),
  LANGUAGE_DECL ("Ukrainian",                   "uk",  LANG_UKRAINIAN),
  LANGUAGE_DECL ("Urdu",                        "ur",  LANG_URDU),
  LANGUAGE_DECL ("Uzbek",                       "uz",  LANG_UZBEK),
  LANGUAGE_DECL ("Vai",                         "vai", -1),
  LANGUAGE_DECL ("Venda",                       "ve",  0x33),
  LANGUAGE_DECL ("Vietnamese",                  "vi",  LANG_VIETNAMESE),
  LANGUAGE_DECL ("Volapük",                     "vo",  -1),
  LANGUAGE_DECL ("Vunjo",                       "vun", -1),
  LANGUAGE_DECL ("Walser",                      "wae", -1),
  LANGUAGE_DECL ("Wolaytta",                    "wal", -1),
  LANGUAGE_DECL ("Wolof",                       "wo",  LANG_WOLOF),
  LANGUAGE_DECL ("isiXhosa",                    "xh",  LANG_XHOSA),
  LANGUAGE_DECL ("Soga",                        "xog", -1),
  LANGUAGE_DECL ("Yangben",                     "yav", -1),
  LANGUAGE_DECL ("Yiddish",                     "yi",  0x3D),
  LANGUAGE_DECL ("Nheengatu",                   "yrl", -1),
  LANGUAGE_DECL ("Yoruba",                      "yo",  LANG_YORUBA),
  LANGUAGE_DECL ("Standard Moroccan Tamazight", "zgh", -1),
  LANGUAGE_DECL ("Chinese",                     "zh",  LANG_CHINESE),
  LANGUAGE_DECL ("Zulu",                        "zu",  LANG_ZULU),
  /* clang-format on */
};

void p32_language (LanguageIndex index, Language *language) {
  assert (index >= LanguageIndex_MIN && index <= LanguageIndex_MAX);
  *language = Languages[index];
}

#if !P32_LOCALE_NAMES
LanguageIndex p32_language_from_id (uint16_t langId, const wchar_t *ll) {
  for (LanguageIndex i = 0; i < (LanguageIndex) _countof (Languages); i++) {
    if (Languages[i].LangId == langId && wcscmp (Languages[i].Code, ll) == 0) {
      return i;
    }
  }

  return LanguageIndex_invalid;
}
#endif
