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

/**
 * File Summary:
 *
 * This file provides generic implementation for functions declared in uchar.h:
 *
 *  - mbrtoc8
 *  - c8rtomb
 *  - mbrtoc16
 *  - c16rtomb
 *  - mbrtoc32
 *  - c32rtomb
 *
 * Macro `P32_UCHAR_IMPL` must be defined to one of the following values
 * before including this source file to choose particular implemetation:
 *
 *  - P32_UCHAR_IMPL_POSIX
 *  - P32_UCHAR_IMPL_ASCII
 *  - P32_UCHAR_IMPL_SBCS
 *  - P32_UCHAR_IMPL_DBCS
 *  - P32_UCHAR_IMPL_UNICODE
 */

#ifndef P32_UCHAR_IMPL
#error P32_UCHAR_IMPL must be defined before including uchar.c
#endif

#define P32_UCHAR_IMPL_POSIX   0
#define P32_UCHAR_IMPL_ASCII   1
#define P32_UCHAR_IMPL_SBCS    2
#define P32_UCHAR_IMPL_DBCS    3
#define P32_UCHAR_IMPL_UNICODE 4

#if P32_UCHAR_IMPL == P32_UCHAR_IMPL_POSIX
#define P32_UCHAR_SBCS
#define mbrtoc8  p32_private_mbrtoc8_posix
#define c8rtomb  p32_private_c8rtomb_posix
#define mbrtoc16 p32_private_mbrtoc16_posix
#define c16rtomb p32_private_c16rtomb_posix
#define mbrtoc32 p32_private_mbrtoc32_posix
#define c32rtomb p32_private_c32rtomb_posix
#elif P32_UCHAR_IMPL == P32_UCHAR_IMPL_ASCII
#define P32_UCHAR_SBCS
#define mbrtoc8  p32_private_mbrtoc8_ascii
#define c8rtomb  p32_private_c8rtomb_ascii
#define mbrtoc16 p32_private_mbrtoc16_ascii
#define c16rtomb p32_private_c16rtomb_ascii
#define mbrtoc32 p32_private_mbrtoc32_ascii
#define c32rtomb p32_private_c32rtomb_ascii
#elif P32_UCHAR_IMPL == P32_UCHAR_IMPL_SBCS
#define P32_UCHAR_SBCS
#define mbrtoc8  p32_private_mbrtoc8_sbcs
#define c8rtomb  p32_private_c8rtomb_sbcs
#define mbrtoc16 p32_private_mbrtoc16_sbcs
#define c16rtomb p32_private_c16rtomb_sbcs
#define mbrtoc32 p32_private_mbrtoc32_sbcs
#define c32rtomb p32_private_c32rtomb_sbcs
#elif P32_UCHAR_IMPL == P32_UCHAR_IMPL_DBCS
#define P32_UCHAR_DBCS
#define mbrtoc8  p32_private_mbrtoc8_dbcs
#define c8rtomb  p32_private_c8rtomb_dbcs
#define mbrtoc16 p32_private_mbrtoc16_dbcs
#define c16rtomb p32_private_c16rtomb_dbcs
#define mbrtoc32 p32_private_mbrtoc32_dbcs
#define c32rtomb p32_private_c32rtomb_dbcs
#elif P32_UCHAR_IMPL == P32_UCHAR_IMPL_UNICODE
#define P32_UCHAR_UTF8
#define mbrtoc8  p32_private_mbrtoc8_utf8
#define c8rtomb  p32_private_c8rtomb_utf8
#define mbrtoc16 p32_private_mbrtoc16_utf8
#define c16rtomb p32_private_c16rtomb_utf8
#define mbrtoc32 p32_private_mbrtoc32_utf8
#define c32rtomb p32_private_c32rtomb_utf8
#else
#error P32_UCHAR_IMPL is set to invalid value
#endif

/**
 * Bits distribution of an Unicode Code Point.
 */
typedef union {
  uint32_t Value;
  /**
   * Bits distribution for UTF-32 Encoding Form.
   */
  struct {
    uint32_t Bits    : 24;
    uint32_t Padding : 8;
  } UTF32;
  /**
   * Bits distribution for UTF-16 Encoding Form.
   */
  union {
    /**
     * UTF-16 Code Unit Sequence of length 1.
     */
    struct {
      uint32_t Bits1   : 16;
      uint32_t Padding : 16;
    } L1;
    /**
     * UTF-16 Code Unit Sequence of length 2.
     */
    struct {
      uint32_t Bits3   : 10;
      uint32_t Bits2   : 6;
      uint32_t Bits1   : 5;
      uint32_t Padding : 11;
    } L2;
  } UTF16;
  /**
   * Bits distribution for UTF-8 Encoding Form.
   */
  union {
    /**
     * UTF-8 Code Unit Sequence of length 1.
     */
    struct {
      uint32_t Bits1   : 7;
      uint32_t Padding : 25;
    } Unit1;
    /**
     * UTF-8 Code Unit Sequence of length 2.
     */
    struct {
      uint32_t Bits2   : 6;
      uint32_t Bits1   : 5;
      uint32_t Padding : 21;
    } Unit2;
    /**
     * UTF-8 Code Unit Sequence of length 3.
     */
    struct {
      uint32_t Bits3   : 6;
      uint32_t Bits2   : 6;
      uint32_t Bits1   : 4;
      uint32_t Padding : 16;
    } Unit3;
    /**
     * UTF-8 Code Unit Sequence of length 4.
     */
    struct {
      uint32_t Bits5   : 6;
      uint32_t Bits4   : 6;
      uint32_t Bits3   : 4;
      uint32_t Bits2   : 2;
      uint32_t Bits1   : 3;
      uint32_t Padding : 11;
    } Unit4;
  } UTF8;
} CodePoint;

P32_STATIC_ASSERT (sizeof (CodePoint) == 4, "Size of CodePoint must 4 bytes");

#define P32_MBSTATE_SBCS_MAGIC  0x00
#define P32_MBSTATE_DBCS_MAGIC  0x01
#define P32_MBSTATE_UTF8_MAGIC  0x02
#define P32_MBSTATE_UTF16_MAGIC 0x03

/**
 * Internal interpretation of `mbstate_t`.
 *
 * This union, as a whole, is stored within `mbstate_t` object.
 */
typedef union {
  /**
   * Value of this union as an `int`.
   */
  int Bytes;

#if P32_CRT < P32_UCRT
  /**
   * Bits used to store information about conversion state.
   */
  struct {
    uint8_t Byte1;
    uint8_t Byte2;
    uint8_t Byte3;
    union {
      uint8_t Value;
      struct {
        uint8_t Magic  : 2;
        uint8_t State  : 2;
        uint8_t Length : 2;
        uint8_t Bytes  : 2;
      };
    };
  } Info;

  /**
   * Prior to UCRT, `mbstate_t` was just an `int`. This significantly limits
   * amount of information we can store in `mbstate_t` objects.
   *
   * In particuler, since we use one byte of `mbstate_t` object to store
   * information about conversion state, we cannot store complete UTF-8
   * Code Unit Sequence of length 4 in `mbstate_t`.
   *
   * We need to do it in order to properly implement `mbrtoc8`, `c8rtomb` and
   * `mbrtoc16` functions.
   *
   * To deal with it, we store it as a single UTF-32 Code Point. A valid UTF-32
   * Code Point only uses 21 lower bits of 4-byte object used to store it,
   * which allows us to use its 8 higher bits for our own needs.
   */
  CodePoint CodePoint;
#endif

  /**
   * Conversion state for UTF-16.
   */
  struct {
    uint16_t Unit;
#if P32_CRT >= P32_UCRT
    uint16_t Reserved;
#else
    uint8_t Reserved;
    union {
      struct {
        uint8_t Magic  : 2;
        uint8_t State  : 2;
        uint8_t Length : 2;
        uint8_t Units  : 2;
      } Info;
    };
#endif
  } UTF16;

  /**
   * Conversion state for UTF-8.
   */
  struct {
#if P32_CRT >= P32_UCRT
    uint8_t Units[4];
#else
    uint8_t Units[3];
    union {
      struct {
        uint8_t Magic  : 2;
        uint8_t State  : 2;
        uint8_t Length : 2;
        uint8_t Bytes  : 2;
      } Info;
    };
#endif
  } UTF8;

  /**
   * Conversion state for DBCS.
   */
  struct {
    uint16_t CodePage;
    char     Byte;
#if P32_CRT >= P32_UCRT
    uint8_t Guard;
#else
    union {
      struct {
        uint8_t Magic  : 2;
        uint8_t State  : 2;
        uint8_t Length : 2;
        uint8_t Bytes  : 2;
      } Info;
    };
#endif
  } DBCS;
} ConversionState;

P32_STATIC_ASSERT (sizeof (ConversionState) == 4, "Size of ConversionState must be 4 bytes");

#if P32_CRT < P32_UCRT
P32_STATIC_ASSERT (offsetof (ConversionState, Info.Value) == offsetof (ConversionState, UTF8.Info), "");
P32_STATIC_ASSERT (offsetof (ConversionState, Info.Value) == offsetof (ConversionState, DBCS.Info), "");
P32_STATIC_ASSERT (offsetof (ConversionState, Info.Value) == offsetof (ConversionState, UTF16.Info), "");
#endif

/**
 * Infromation about conversions state.
 */
typedef struct {
  /**
   * Identifies conversion state.
   */
  uint8_t Magic;
  /**
   * Describes how to interpret `Length` and Bytes` members.
   */
  uint8_t State;
  /**
   * Length of multibyte sequence.
   *
   * For UTF-16, this is the number of UTF-16 Code Units, not bytes.
   */
  uint8_t Length;
  /**
   * When `state == P32_MBSTATE_INCOMPLETE`, this is number of bytes that has
   * been consumed and stored in `mbstate_t` object.
   *
   * When `state == P32_MBSTATE_COMPLETE`, this is number of Code Units left to
   * write. This is used by `mbrtoc8` and `mbrtoc16`, since they may require
   * multiple calls to write converted Code Unit Sequence.
   *
   * For UTF-16, this is the number of UTF-16 Code Units, not bytes.
   */
  uint8_t Bytes;
} ConversionStateInfo;

/**
 * Extract conversion state information from `mbstate_t` object `state` and
 * store it in `info`.
 */
static void P32GetConversionState (ConversionStateInfo *info, ConversionState *mbState, mbstate_t *state) {
#if P32_CRT >= P32_UCRT
  mbState->Bytes = state->_Wchar;

  info->Magic  = (state->_State & 0xFF00) >> 8;
  info->State  = (state->_State & 0x00FF);
  info->Length = (state->_Byte & 0xFF00) >> 8;
  info->Bytes  = (state->_Byte & 0x00FF);
#else
  mbState->Bytes = *state;

  info->Magic  = mbState->Info.Magic;
  info->State  = mbState->Info.State;
  info->Length = mbState->Info.Length;
  info->Bytes  = mbState->Info.Bytes;
#endif
}

/*******************************************************************************
 * UTF-16
 */

#define P32_UTF16_HIGH_SURROGATE_MAGIC 0x36
#define P32_UTF16_LOW_SURROGATE_MAGIC  0x37

/**
 * Bits distribution for an UTF-16 Code Unit.
 */
typedef union {
  uint16_t Value;
  /**
   * Bits distribution for High Surrogate UTF-16 Code Unit.
   */
  struct {
    uint16_t Bits2 : 6;
    uint16_t Bits1 : 4;
    uint16_t Magic : 6;
  } HighSurrogate;
  /**
   * Bits distribution for Low Surrogate UTF-16 Code Unit.
   */
  struct {
    uint16_t Bits3 : 10;
    uint16_t Magic : 6;
  } LowSurrogate;
} UTF16Unit;

P32_STATIC_ASSERT (sizeof (UTF16Unit) == 2, "Size of UTF16Unit must be 2 bytes");

#define P32_MBSTATE_UTF16_INITIAL    0x00
#define P32_MBSTATE_UTF16_INCOMPLETE 0x01
#define P32_MBSTATE_UTF16_COMPLETE   0x02
#define P32_MBSTATE_UTF16_RESERVED   0x03

/**
 * Internal conversion state for UTF-16.
 */
typedef struct {
  ConversionStateInfo Info;
  union {
    char16_t Buffer[2];
    struct {
      UTF16Unit Unit1;
      UTF16Unit Unit2;
    };
  };
} UTF16ConversionState;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

/**
 * Convert Unicode Code Point to UTF-16 Code Unit Sequence.
 */
static bool P32CodePointToUTF16 (UTF16ConversionState *utf16, CodePoint *codePoint) {
  if (codePoint->Value > 0x0010FFFF) {
    return false;
  }

  if (IS_HIGH_SURROGATE (codePoint->Value) || IS_LOW_SURROGATE (codePoint->Value)) {
    return false;
  }

  if (codePoint->UTF16.L1.Padding == 0) {
    utf16->Info.Length = 1;
    utf16->Info.Bytes  = 1;
    utf16->Unit1.Value = codePoint->UTF16.L1.Bits1;
  } else {
    assert (codePoint->UTF16.L2.Padding == 0);
    utf16->Info.Length               = 2;
    utf16->Info.Bytes                = 2;
    utf16->Unit1.HighSurrogate.Magic = P32_UTF16_HIGH_SURROGATE_MAGIC;
    utf16->Unit1.HighSurrogate.Bits1 = codePoint->UTF16.L2.Bits1 - 1;
    utf16->Unit1.HighSurrogate.Bits2 = codePoint->UTF16.L2.Bits2;
    utf16->Unit2.LowSurrogate.Magic  = P32_UTF16_LOW_SURROGATE_MAGIC;
    utf16->Unit2.LowSurrogate.Bits3  = codePoint->UTF16.L2.Bits3;
  }

  return true;
}

/**
 * Convert UTF-16 Code Unit Sequence to Unicode Code Point.
 */
static bool P32UTF16ToCodePoint (CodePoint *codePoint, UTF16ConversionState *utf16) {
  if (utf16->Info.Length == 1) {
    codePoint->UTF16.L1.Bits1 = utf16->Unit1.Value;
  } else {
    assert (utf16->Info.Length == 2);
    codePoint->UTF16.L2.Bits1 = utf16->Unit1.HighSurrogate.Bits1 + 1;
    codePoint->UTF16.L2.Bits2 = utf16->Unit1.HighSurrogate.Bits2;
    codePoint->UTF16.L2.Bits3 = utf16->Unit2.LowSurrogate.Bits3;
  }

  assert (codePoint->Value <= 0x0010FFFF);
  assert (!IS_HIGH_SURROGATE (codePoint->Value) && !IS_LOW_SURROGATE (codePoint->Value));

  return true;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/**
 * Store UTF-16 conversion state `utf16` in `mbstate_t` object `state`.
 */
static void P32SetUTF16ConversionState (mbstate_t *state, UTF16ConversionState *utf16) {
  /**
   * Set `state` to initial conversion state.
   */
  if (utf16 == NULL) {
    memset (state, 0, sizeof (mbstate_t));
    return;
  }

  assert (utf16->Info.State == P32_MBSTATE_UTF16_INCOMPLETE);

  /**
   * Packed conversion state.
   */
  ConversionState conversionState = {0};

  conversionState.UTF16.Unit = utf16->Unit1.Value;

#if P32_CRT >= P32_UCRT
  state->_Byte  = (utf16->Info.Length << 8) | (utf16->Info.Bytes);
  state->_State = (utf16->Info.Magic << 8) | (utf16->Info.State);
  state->_Wchar = conversionState.Bytes;
#else
  conversionState.UTF16.Info.Magic  = utf16->Info.Magic;
  conversionState.UTF16.Info.State  = utf16->Info.State;
  conversionState.UTF16.Info.Length = utf16->Info.Length;
  conversionState.UTF16.Info.Units  = utf16->Info.Bytes;

  *state = conversionState.Bytes;
#endif
}

/**
 * Get UTF-16 conversion state `utf16` from `mbstate_t` object `state`.
 */
static bool P32GetUTF16ConversionState (UTF16ConversionState *utf16, mbstate_t *state) {
  ConversionState conversionState = {0};

  P32GetConversionState (&utf16->Info, &conversionState, state);

  /**
   * This field must always be zero.
   *
   * This may occur if `mbstate_t` object was not properly initialized.
   */
  if (conversionState.UTF16.Reserved != 0) {
    return false;
  }

  /**
   * `csMagic == 0` is valid for initial conversion state.
   */
  if (utf16->Info.Magic != 0 && utf16->Info.Magic != P32_MBSTATE_UTF16_MAGIC) {
    return false;
  }

  if (utf16->Info.State == P32_MBSTATE_UTF16_INITIAL) {
    /**
     * In initial conversion state all information bits must be zero.
     */
    if (utf16->Info.Magic != 0 || utf16->Info.Length != 0 || utf16->Info.Bytes != 0) {
      return false;
    }
  } else if (utf16->Info.State == P32_MBSTATE_UTF16_INCOMPLETE) {
    /**
     * This state may only occur when `c16rtomb` has consumed high surrogate.
     */
    if (utf16->Info.Magic != P32_MBSTATE_UTF16_MAGIC || utf16->Info.Length != 2 || utf16->Info.Bytes != 1) {
      return false;
    }
  } else {
    return false;
  }

  utf16->Unit1.Value = conversionState.UTF16.Unit;

  if (utf16->Info.Length == 0) {
    if (utf16->Unit1.Value != 0) {
      return false;
    }
  } else if (utf16->Info.Length == 2) {
    if (!IS_HIGH_SURROGATE (utf16->Unit1.Value)) {
      return false;
    }
  }

  return true;
}

/*******************************************************************************
 * UTF-8
 */

#define P32_UTF8_L1_MAGIC 0x00
#define P32_UTF8_L2_MAGIC 0x06
#define P32_UTF8_L3_MAGIC 0x0E
#define P32_UTF8_L4_MAGIC 0x1E
#define P32_UTF8_MAGIC    0x02

/**
 * Bits distribution for an UTF-8 Code Unit.
 */
typedef union {
  uint8_t Value;
  /**
   * Bits distribution for UTF-8 Code Unit Sequence of length 1.
   */
  union {
    struct {
      uint8_t Bits1 : 7;
      uint8_t Magic : 1;
    } Unit1;
  } L1;
  /**
   * Bits distribution for UTF-8 Code Unit Sequence of length 2.
   */
  union {
    struct {
      uint8_t Bits1 : 5;
      uint8_t Magic : 3;
    } Unit1;
    struct {
      uint8_t Bits2 : 6;
      uint8_t Magic : 2;
    } Unit2;
  } L2;
  /**
   * Bits distribution for UTF-8 Code Unit Sequence of length 3.
   */
  union {
    struct {
      uint8_t Bits1 : 4;
      uint8_t Magic : 4;
    } Unit1;
    struct {
      uint8_t Bits2 : 6;
      uint8_t Magic : 2;
    } Unit2;
    struct {
      uint8_t Bits3 : 6;
      uint8_t Magic : 2;
    } Unit3;
  } L3;
  /**
   * Bits distribution for UTF-8 Code Unit Sequence of length 4.
   */
  union {
    struct {
      uint8_t Bits1 : 3;
      uint8_t Magic : 5;
    } Unit1;
    struct {
      uint8_t Bits3 : 4;
      uint8_t Bits2 : 2;
      uint8_t Magic : 2;
    } Unit2;
    struct {
      uint8_t Bits4 : 6;
      uint8_t Magic : 2;
    } Unit3;
    struct {
      uint8_t Bits5 : 6;
      uint8_t Magic : 2;
    } Unit4;
  } L4;
} UTF8Unit;

P32_STATIC_ASSERT (sizeof (UTF8Unit) == 1, "Size of UTF8Unit must be 1 byte");

#define P32_MBSTATE_UTF8_INITIAL    0x00
#define P32_MBSTATE_UTF8_INCOMPLETE 0x01
#define P32_MBSTATE_UTF8_COMPLETE   0x02
#define P32_MBSTATE_UTF8_RESERVED   0x03

/**
 * Internal conversion state for UTF-8.
 */
typedef struct {
  ConversionStateInfo Info;
  union {
    char Buffer[4];
    struct {
      UTF8Unit Unit1;
      UTF8Unit Unit2;
      UTF8Unit Unit3;
      UTF8Unit Unit4;
    };
  };
} UTF8ConversionState;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

/**
 * Convert Unicode Code Point to UTF-8 Code Unit Sequence.
 */
static bool P32CodePointToUTF8 (UTF8ConversionState *utf8, CodePoint *codePoint) {
  if (codePoint->Value > 0x0010FFFF) {
    return false;
  }

  if (IS_HIGH_SURROGATE (codePoint->Value) || IS_LOW_SURROGATE (codePoint->Value)) {
    return false;
  }

  if (codePoint->UTF8.Unit1.Padding == 0) {
    utf8->Info.Length          = 1;
    utf8->Info.Bytes           = 1;
    utf8->Unit1.L1.Unit1.Magic = P32_UTF8_L1_MAGIC;
    utf8->Unit1.L1.Unit1.Bits1 = codePoint->UTF8.Unit1.Bits1;
  } else if (codePoint->UTF8.Unit2.Padding == 0) {
    utf8->Info.Length          = 2;
    utf8->Info.Bytes           = 2;
    utf8->Unit1.L2.Unit1.Magic = P32_UTF8_L2_MAGIC;
    utf8->Unit1.L2.Unit1.Bits1 = codePoint->UTF8.Unit2.Bits1;
    utf8->Unit2.L2.Unit2.Magic = P32_UTF8_MAGIC;
    utf8->Unit2.L2.Unit2.Bits2 = codePoint->UTF8.Unit2.Bits2;
  } else if (codePoint->UTF8.Unit3.Padding == 0) {
    utf8->Info.Length          = 3;
    utf8->Info.Bytes           = 3;
    utf8->Unit1.L3.Unit1.Magic = P32_UTF8_L3_MAGIC;
    utf8->Unit1.L3.Unit1.Bits1 = codePoint->UTF8.Unit3.Bits1;
    utf8->Unit2.L3.Unit2.Magic = P32_UTF8_MAGIC;
    utf8->Unit2.L3.Unit2.Bits2 = codePoint->UTF8.Unit3.Bits2;
    utf8->Unit3.L3.Unit3.Magic = P32_UTF8_MAGIC;
    utf8->Unit3.L3.Unit3.Bits3 = codePoint->UTF8.Unit3.Bits3;
  } else {
    assert (codePoint->UTF8.Unit4.Padding == 0);
    utf8->Info.Length          = 4;
    utf8->Info.Bytes           = 4;
    utf8->Unit1.L4.Unit1.Magic = P32_UTF8_L4_MAGIC;
    utf8->Unit1.L4.Unit1.Bits1 = codePoint->UTF8.Unit4.Bits1;
    utf8->Unit2.L4.Unit2.Magic = P32_UTF8_MAGIC;
    utf8->Unit2.L4.Unit2.Bits2 = codePoint->UTF8.Unit4.Bits2;
    utf8->Unit2.L4.Unit2.Bits3 = codePoint->UTF8.Unit4.Bits3;
    utf8->Unit3.L4.Unit3.Magic = P32_UTF8_MAGIC;
    utf8->Unit3.L4.Unit3.Bits4 = codePoint->UTF8.Unit4.Bits4;
    utf8->Unit4.L4.Unit4.Magic = P32_UTF8_MAGIC;
    utf8->Unit4.L4.Unit4.Bits5 = codePoint->UTF8.Unit4.Bits5;
  }

  return true;
}

/**
 * Convert UTF-8 Code Unit Sequence to Unicode Code Point.
 */
static bool P32UTF8ToCodePoint (CodePoint *codePoint, UTF8ConversionState *utf8) {
  if (utf8->Info.Length == 1) {
    codePoint->UTF8.Unit1.Bits1 = utf8->Unit1.L1.Unit1.Bits1;
  } else if (utf8->Info.Length == 2) {
    codePoint->UTF8.Unit2.Bits1 = utf8->Unit1.L2.Unit1.Bits1;
    codePoint->UTF8.Unit2.Bits2 = utf8->Unit2.L2.Unit2.Bits2;
  } else if (utf8->Info.Length == 3) {
    codePoint->UTF8.Unit3.Bits1 = utf8->Unit1.L3.Unit1.Bits1;
    codePoint->UTF8.Unit3.Bits2 = utf8->Unit2.L3.Unit2.Bits2;
    codePoint->UTF8.Unit3.Bits3 = utf8->Unit3.L3.Unit3.Bits3;
  } else if (utf8->Info.Length == 4) {
    codePoint->UTF8.Unit4.Bits1 = utf8->Unit1.L4.Unit1.Bits1;
    codePoint->UTF8.Unit4.Bits2 = utf8->Unit2.L4.Unit2.Bits2;
    codePoint->UTF8.Unit4.Bits3 = utf8->Unit2.L4.Unit2.Bits3;
    codePoint->UTF8.Unit4.Bits4 = utf8->Unit3.L4.Unit3.Bits4;
    codePoint->UTF8.Unit4.Bits5 = utf8->Unit4.L4.Unit4.Bits5;
  }

  assert (codePoint->Value <= 0x0010FFFF);

  if (IS_HIGH_SURROGATE (codePoint->Value) || IS_LOW_SURROGATE (codePoint->Value)) {
    return false;
  }

  return true;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

/**
 * Store UTF-8 conversion state `utf8` in `mbstate_t` object `state`.
 */
static void P32SetUTF8ConversionState (mbstate_t *state, UTF8ConversionState *utf8) {
  /**
   * Set `state` to initial conversion state.
   *
   * Situation when `utf8->Info.Length == 0` can occur when `count` argument
   * to mbrtoc{8,16,32} was `0`.
   */
  if (utf8 == NULL || utf8->Info.Length == 0) {
    memset (state, 0, sizeof (mbstate_t));
    return;
  }

  uint8_t bytesToStore = 0;

  /**
   * When `utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE`, we must store
   * `utf8->Info.Length` bytes in `state`, since `utf8->Info.Bytes`
   * no longer reflects number of bytes stored in `utf8`.
   *
   * Otherwise, we store `utf8->Info.Bytes` bytes in `state`, which is number
   * of bytes stored in `utf8` so far.
   */
  if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE) {
    bytesToStore = utf8->Info.Length;
  } else {
    assert (utf8->Info.State == P32_MBSTATE_UTF8_INCOMPLETE);
    bytesToStore = utf8->Info.Bytes;
  }

  /**
   * Packed conversion state.
   */
  ConversionState conversionState = {0};

  if (bytesToStore == 1) {
    assert (utf8->Unit1.Value != 0);
    conversionState.UTF8.Units[0] = utf8->Unit1.Value;
    assert (utf8->Unit2.Value == 0);
    assert (utf8->Unit3.Value == 0);
    assert (utf8->Unit4.Value == 0);
  } else if (bytesToStore == 2) {
    assert (utf8->Unit1.Value != 0);
    conversionState.UTF8.Units[0] = utf8->Unit1.Value;
    assert (utf8->Unit2.Value != 0);
    conversionState.UTF8.Units[1] = utf8->Unit2.Value;
    assert (utf8->Unit3.Value == 0);
    assert (utf8->Unit4.Value == 0);
  } else if (bytesToStore == 3) {
    assert (utf8->Unit1.Value != 0);
    conversionState.UTF8.Units[0] = utf8->Unit1.Value;
    assert (utf8->Unit2.Value != 0);
    conversionState.UTF8.Units[1] = utf8->Unit2.Value;
    assert (utf8->Unit3.Value != 0);
    conversionState.UTF8.Units[2] = utf8->Unit3.Value;
    assert (utf8->Unit4.Value == 0);
  } else if (bytesToStore == 4) {
#if P32_CRT >= P32_UCRT
    assert (utf8->Unit1.Value != 0);
    conversionState.UTF8.Units[0] = utf8->Unit1.Value;
    assert (utf8->Unit2.Value != 0);
    conversionState.UTF8.Units[1] = utf8->Unit2.Value;
    assert (utf8->Unit3.Value != 0);
    conversionState.UTF8.Units[2] = utf8->Unit3.Value;
    assert (utf8->Unit4.Value != 0);
    conversionState.UTF8.Units[3] = utf8->Unit4.Value;
#else
    /**
     * This must never fail.
     *
     * "DOOOOM?! WHAT DO YOU MEAN DOOM?!"
     *
     *  - Takanashi Kiara, 2025
     */
    P32UTF8ToCodePoint (&conversionState.CodePoint, utf8);
#endif
  }

#if P32_CRT >= P32_UCRT
  state->_Byte  = (utf8->Info.Length << 8) | (utf8->Info.Bytes);
  state->_State = (utf8->Info.Magic << 8) | (utf8->Info.State);
  state->_Wchar = conversionState.Bytes;
#else
  conversionState.UTF8.Info.Magic  = utf8->Info.Magic;
  conversionState.UTF8.Info.State  = utf8->Info.State;
  conversionState.UTF8.Info.Length = utf8->Info.Length - 1;

  if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE) {
    assert (utf8->Info.Bytes < 4);
    conversionState.UTF8.Info.Bytes = utf8->Info.Bytes;
  } else if (utf8->Info.State == P32_MBSTATE_UTF8_INCOMPLETE) {
    conversionState.UTF8.Info.Bytes = utf8->Info.Bytes - 1;
  }

  *state = conversionState.Bytes;
#endif
}

/**
 * Get UTF-8 conversion state `utf8` from `mbstate_t` object `state`.
 */
static bool P32GetUTF8ConversionState (UTF8ConversionState *utf8, ConversionState *conversionState, const int magic) {
  /**
   * `csMagic == 0` is valid for initial conversion state.
   */
  if (utf8->Info.Magic != 0 && utf8->Info.Magic != magic) {
    return false;
  }

  if (utf8->Info.State == P32_MBSTATE_UTF8_INITIAL) {
    /**
     * In initial conversion state all information bits must be zero.
     */
    if (utf8->Info.Magic != 0 || utf8->Info.Length != 0 || utf8->Info.Bytes != 0) {
      return false;
    }
  } else if (utf8->Info.State == P32_MBSTATE_UTF8_INCOMPLETE) {
    /**
     * Check for `utf8->Info.Length` and `utf8->Info.Bytes` is delayed.
     */
    if (utf8->Info.Magic != magic) {
      return false;
    }
  } else if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE) {
    /**
     * Check for `utf8->Info.Length` and `utf8->Info.Bytes` is delayed.
     */
    if (utf8->Info.Magic != magic) {
      return false;
    }
  } else {
    return false;
  }

#if P32_CRT < P32_UCRT
  /**
   * We use two bits of `mbstate_t` to store this information, which means
   * we cannot store value `4` in those bit fields.
   *
   * When we store conversion state in `mbstate_t`, these values cannot be 0,
   * so we decrement them by 1. Increment them by 1 to get actual values.
   */
  if (utf8->Info.State == P32_MBSTATE_UTF8_INCOMPLETE) {
    utf8->Info.Length += 1;
    utf8->Info.Bytes  += 1;
  } else if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE) {
    utf8->Info.Length += 1;
  }
#endif

  if (utf8->Info.Length > 4 || utf8->Info.Bytes > utf8->Info.Length) {
    return false;
  }

  if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE) {
    /**
     * We do not store conversion state for UTF-8 Code Unit Sequence shorter
     * than 2.
     */
    if (utf8->Info.Length < 2) {
      return false;
    }
    /**
     * `mbrtoc8` stores number of UTF-8 Code Units left to write, which can be
     * 1, 2 or 3.
     *
     * `mbrtoc16` stores zero to indicate that the next call to `mbrtoc16`
     * must write UTF-16 low surrogate.
     */
    if (utf8->Info.Bytes > 3) {
      return false;
    }
  } else if (utf8->Info.State == P32_MBSTATE_UTF8_INCOMPLETE) {
    /**
     * We do not store conversion state for UTF-8 Code Unit Sequence shorter
     * than 2.
     */
    if (utf8->Info.Length < 2) {
      return false;
    }
    /**
     * At least one and no more than three UTF-8 Code Units must be stored.
     */
    if (utf8->Info.Bytes < 1 || utf8->Info.Bytes > 3) {
      return false;
    }
  }

#if P32_CRT >= P32_UCRT
  if (1) {
#else
  if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE && utf8->Info.Length == 4) {
    uint8_t csLength = utf8->Info.Length;
    uint8_t csBytes  = utf8->Info.Bytes;

    /**
     * Overwrite bits used to store information about conversion state.
     */
    conversionState->Info.Value = 0;

    if (!P32CodePointToUTF8 (utf8, &conversionState->CodePoint)) {
      return false;
    }

    if (utf8->Info.Length != csLength) {
      return false;
    }

    utf8->Info.Bytes = csBytes;
  } else {
#endif
    utf8->Unit1.Value = conversionState->UTF8.Units[0];
    utf8->Unit2.Value = conversionState->UTF8.Units[1];
    utf8->Unit3.Value = conversionState->UTF8.Units[2];
#if P32_CRT >= P32_UCRT
    utf8->Unit4.Value = conversionState->UTF8.Units[3];
#endif
  }

  if (utf8->Info.Length == 0) {
    /**
     * All UTF-8 Code Units stored in `utf8` must be zero.
     */
    if (utf8->Unit1.Value != 0 || utf8->Unit2.Value != 0 || utf8->Unit3.Value != 0 || utf8->Unit4.Value != 0) {
      return false;
    }
  } else if (utf8->Info.Length == 2) {
    /**
     * Validate first UTF-8 Code Unit.
     */
    if (utf8->Unit1.L2.Unit1.Magic != P32_UTF8_L2_MAGIC || utf8->Unit1.L2.Unit1.Bits1 == 0) {
      return false;
    }
    /**
     * Validate second UTF-8 Code Unit.
     */
    if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE || utf8->Info.Bytes >= 2) {
      if (utf8->Unit2.L2.Unit2.Magic != P32_UTF8_MAGIC) {
        return false;
      }
    } else if (utf8->Unit2.Value != 0) {
      return false;
    }
    /**
     * Third and fourth UTF-8 Code Units must be zero.
     */
    if (utf8->Unit3.Value != 0 || utf8->Unit4.Value != 0) {
      return false;
    }
  } else if (utf8->Info.Length == 3) {
    /**
     * Validate first UTF-8 Code Unit.
     */
    if (utf8->Unit1.L3.Unit1.Magic != P32_UTF8_L3_MAGIC) {
      return false;
    }
    /**
     * Validate second UTF-8 Code Unit.
     */
    if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE || utf8->Info.Bytes >= 2) {
      /**
       * This check cannot be done until two Code Untis are stored in `utf8`.
       */
      if (utf8->Unit1.L3.Unit1.Bits1 == 0 && (utf8->Unit2.L3.Unit2.Bits2 & 0x20) == 0) {
        return false;
      }
      if (utf8->Unit2.L3.Unit2.Magic != P32_UTF8_MAGIC) {
        return false;
      }
    } else if (utf8->Unit2.Value != 0) {
      return false;
    }
    /**
     * Validate third UTF-8 Code Unit.
     */
    if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE || utf8->Info.Bytes >= 3) {
      if (utf8->Unit3.L3.Unit3.Magic != P32_UTF8_MAGIC) {
        return false;
      }
    } else if (utf8->Unit3.Value != 0) {
      return false;
    }
    /**
     * Fourth UTF-8 Code Unit must be zero.
     */
    if (utf8->Unit4.Value != 0) {
      return false;
    }
  } else if (utf8->Info.Length == 4) {
    /**
     * Validate first UTF-8 Code Unit.
     */
    if (utf8->Unit1.L4.Unit1.Magic != P32_UTF8_L4_MAGIC) {
      return false;
    }
    /**
     * Validate second UTF-8 Code Unit.
     */
    if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE || utf8->Info.Bytes >= 2) {
      /**
       * This check cannot be done until two Code Untis are stored in `utf8`.
       */
      if (utf8->Unit1.L4.Unit1.Bits1 == 0 && utf8->Unit2.L4.Unit2.Bits2 == 0) {
        return false;
      }
      if (utf8->Unit2.L4.Unit2.Magic != P32_UTF8_MAGIC) {
        return false;
      }
    } else if (utf8->Unit2.Value != 0) {
      return false;
    }
    /**
     * Validate third UTF-8 Code Unit.
     */
    if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE || utf8->Info.Bytes >= 3) {
      if (utf8->Unit3.L4.Unit3.Magic != P32_UTF8_MAGIC) {
        return false;
      }
    } else if (utf8->Unit3.Value != 0) {
      return false;
    }
    /**
     * Validate fourth UTF-8 Code Unit.
     */
    if (utf8->Info.State == P32_MBSTATE_UTF8_COMPLETE || utf8->Info.Bytes >= 4) {
      if (utf8->Unit4.L4.Unit4.Magic != P32_UTF8_MAGIC) {
        return false;
      }
    } else if (utf8->Unit4.Value != 0) {
      return false;
    }
  }

  return true;
}

/**
 * Consume single UTF-8 Code Unit from `str` and validate if is a valid
 * Code Unit according to conversion state stored in `utf8`.
 *
 * If consumed UTF-8 Code Unit is a valid Code Unit, store it in `utf8` and
 * update conversion state.
 *
 * Return values:
 *  `0`: UTF-8 Code Unit coressponding to NUL character has been consumed
 *
 *  `1`: single valid UTF-8 Code Unit has been consumed
 *
 *  `(size_t)-2`: all bytes from `str` have been consumed and more bytes need
 *    to be examined
 *
 *  `(size_t)-1`: consumed UTF-8 Code Unit is not a valid Code Unit for
 *    conversion state stored in `utf8`
 */
static size_t P32GetUTF8Unit (UTF8ConversionState *utf8, const char *str, size_t *count) {
  /**
   * We need to consume at least one more byte.
   */
  if (*count == 0) {
    return (size_t) -2;
  }

  UTF8Unit codeUnit = {0};
  codeUnit.Value    = (uint8_t) str[0];

  /**
   * Conversion state is in initial state.
   *
   * Check if consumed Code Unit is a valid leading UTF-8 Code Unit, and if so,
   * determine length of UTF-8 Code Unit Sequence to be stored in `utf8`.
   */
  if (utf8->Info.State == P32_MBSTATE_UTF8_INITIAL) {
    if (codeUnit.L1.Unit1.Magic == P32_UTF8_L1_MAGIC) {
      utf8->Info.Length = 1;
    } else if (codeUnit.L2.Unit1.Magic == P32_UTF8_L2_MAGIC) {
      /**
       * Validte cosumed UTF-8 Code Unit.
       */
      if (codeUnit.L2.Unit1.Bits1 == 0) {
        goto eilseq;
      }
      utf8->Info.Length = 2;
    } else if (codeUnit.L3.Unit1.Magic == P32_UTF8_L3_MAGIC) {
      utf8->Info.Length = 3;
    } else if (codeUnit.L4.Unit1.Magic == P32_UTF8_L4_MAGIC) {
      utf8->Info.Length = 4;
    } else {
      goto eilseq;
    }

    utf8->Info.Bytes  += 1;
    utf8->Unit1.Value  = codeUnit.Value;
  } else {
    /**
     * Consumed byte must be a valid trailing UTF-8 Code Unit.
     */
    if (codeUnit.L2.Unit2.Magic != P32_UTF8_MAGIC) {
      goto eilseq;
    }

    if (utf8->Info.Bytes == 1) {
      utf8->Unit2.Value = codeUnit.Value;
    } else if (utf8->Info.Bytes == 2) {
      utf8->Unit3.Value = codeUnit.Value;
    } else if (utf8->Info.Bytes == 3) {
      utf8->Unit4.Value = codeUnit.Value;
    }

    utf8->Info.Bytes += 1;

    /**
     * We have to delay validation for UTF-8 Code Unit Sequence with
     * length 3 and 4 until we have consumed two UTF-8 Code Units.
     */
    if (utf8->Info.Length == 3 && utf8->Info.Bytes == 2) {
      if (utf8->Unit1.L3.Unit1.Bits1 == 0 && (utf8->Unit2.L3.Unit2.Bits2 & 0x20) == 0) {
        goto eilseq;
      }
    } else if (utf8->Info.Length == 4 && utf8->Info.Bytes == 2) {
      if (utf8->Unit1.L4.Unit1.Bits1 == 0 && utf8->Unit2.L4.Unit2.Bits2 == 0) {
        goto eilseq;
      }
    }
  }

  *count -= 1;

  /**
   * We consumed enough UTF-8 Code Units to complete conversion.
   */
  if (utf8->Info.Length == utf8->Info.Bytes) {
    utf8->Info.State = P32_MBSTATE_UTF8_COMPLETE;
    return !!str[0];
  }

  /**
   * We need to consume at least one more UTF-8 Code Unit to perform
   * conversion.
   */
  utf8->Info.State = P32_MBSTATE_UTF8_INCOMPLETE;

  return !!str[0];

eilseq:
  return (size_t) -1;
}

#ifdef P32_UCHAR_DBCS
/*******************************************************************************
 * DBCS
 */

#define P32_MBSTATE_DBCS_INITIAL    0x00
#define P32_MBSTATE_DBCS_INCOMPLETE 0x01
#define P32_MBSTATE_DBCS_COMPLETE   0x02
#define P32_MBSTATE_DBCS_RESERVED   0x03

#define P32_MBSTATE_DBCS_GUARD 0x67

/**
 * Internal conversion state for DBCS.
 */
typedef struct {
  ConversionStateInfo Info;
  char                Buffer[2];
} DBCSConversionState;

/**
 * Return `true` if `byte` is a leading byte used by `charset->CodePage`.
 */
static bool P32IsLeadByte (uint8_t byte, Charset *charset) {
  for (size_t i = 0; i < MAX_LEADBYTES; i += 2) {
    /**
     * A pair of zero bytes terminates the list.
     */
    if (charset->Map[i] == 0 && charset->Map[i + 1] == 0) {
      break;
    }

    if (byte >= charset->Map[i] && byte <= charset->Map[i + 1]) {
      return true;
    }
  }

  return false;
}

/**
 * Store DBCS conversion state `dbcs` in `mbstate_t` object `state`.
 */
static void P32SetDBCSConversionState (mbstate_t *state, DBCSConversionState *dbcs, Charset *charset) {
  /**
   * Set `state` to initial conversion state.
   */
  if (dbcs == NULL || dbcs->Info.Length == 0) {
    memset (state, 0, sizeof (mbstate_t));
    return;
  }

  assert (dbcs->Info.State == P32_MBSTATE_DBCS_INCOMPLETE);

  ConversionState conversionState = {0};

  conversionState.DBCS.CodePage = (uint16_t) charset->CodePage;
  conversionState.DBCS.Byte     = dbcs->Buffer[0];

#if P32_CRT >= P32_UCRT
  conversionState.DBCS.Guard = P32_MBSTATE_DBCS_GUARD;

  state->_Byte  = (dbcs->Info.Length << 8) | (dbcs->Info.Bytes);
  state->_State = (dbcs->Info.Magic << 8) | (dbcs->Info.State);
  state->_Wchar = conversionState.Bytes;
#else
  conversionState.DBCS.Info.Magic  = dbcs->Info.Magic;
  conversionState.DBCS.Info.State  = dbcs->Info.State;
  conversionState.DBCS.Info.Length = dbcs->Info.Length;
  conversionState.DBCS.Info.Bytes  = dbcs->Info.Bytes;

  *state = conversionState.Bytes;
#endif
}

/**
 * Get DBCS conversion state `dbcs` from `mbstate_t` object `state`.
 */
static bool P32GetDBCSConversionState (
  DBCSConversionState *dbcs,
  ConversionState     *conversionState,
  mbstate_t           *state,
  Charset             *charset
) {
  /**
   * No need to validate initial conversion state.
   */
  if (p32_mbsinit (state)) {
    return true;
  }

#if P32_CRT >= P32_UCRT
  /**
   * We store `P32_MBSTATE_DBCS_GUARD` in `conversionState.DBCS.Guard`.
   */
  if (conversionState->DBCS.Guard != P32_MBSTATE_DBCS_GUARD) {
    return false;
  }
#endif

  if (dbcs->Info.Magic != P32_MBSTATE_DBCS_MAGIC) {
    return false;
  }

  assert (dbcs->Info.State != P32_MBSTATE_DBCS_COMPLETE);

  if (dbcs->Info.State != P32_MBSTATE_DBCS_INCOMPLETE) {
    return false;
  }

  if (dbcs->Info.Length != 2) {
    return false;
  }

  if (dbcs->Info.Bytes != 1) {
    return false;
  }

  /**
   * Conversion state is reused with different code page.
   */
  if (conversionState->DBCS.CodePage != charset->CodePage) {
    return false;
  }

  /**
   * We store DBCS leading byte in `mbState->Bytes[0]`.
   */
  if (!P32IsLeadByte ((uint8_t) conversionState->DBCS.Byte, charset)) {
    return false;
  }

  dbcs->Buffer[0] = conversionState->DBCS.Byte;

  return true;
}

/**
 * Convert UTF-16 Code Unit Sequence to DBCS character.
 */
static bool P32UTF16ToDBCS (DBCSConversionState *dbcs, UTF16ConversionState *utf16, Charset *charset) {
  BOOL defaultCharacterUsed = FALSE;

  int length = WideCharToMultiByte (
    charset->CodePage, charset->ToMultiByte, utf16->Buffer, utf16->Info.Length, dbcs->Buffer, charset->MaxLength, NULL,
    &defaultCharacterUsed
  );

  assert (length <= (int) charset->MaxLength);

  if (length == 0 || length > (int) charset->MaxLength || defaultCharacterUsed) {
    return false;
  }

  dbcs->Info.Length = (uint8_t) length;
  dbcs->Info.Bytes  = (uint8_t) length;

  return true;
}

/**
 * Convert DBCS character to UTF-16 Code Unit Sequence.
 */
static bool P32DBCSToUTF16 (UTF16ConversionState *utf16, DBCSConversionState *dbcs, Charset *charset) {
  int length =
    MultiByteToWideChar (charset->CodePage, charset->ToWideChar, dbcs->Buffer, dbcs->Info.Bytes, utf16->Buffer, 2);

  assert (length <= (int) charset->MaxLength);

  if (length == 0 || length > 2) {
    return false;
  }

  utf16->Info.Length = (uint8_t) length;
  utf16->Info.Bytes  = (uint8_t) length;

  return true;
}

/**
 * Consume and validate single byte from `str`.
 */
static size_t P32GetDBCSByte (DBCSConversionState *dbcs, const char *str, size_t *count, Charset *charset) {
  /**
   * We need to consume more bytes to perform conversion.
   */
  if (*count == 0) {
    return (size_t) -2;
  }

  /**
   * If `dbcs` in initial conversion state, check if `str` points to a DBCS
   * lead byte as used by `charset`.
   */
  if (dbcs->Info.State == P32_MBSTATE_DBCS_INITIAL) {
    if (P32IsLeadByte ((uint8_t) str[0], charset)) {
      dbcs->Info.Length = 2;
    } else {
      dbcs->Info.Length = 1;
    }
  } else {
    /**
     * NUL byte cannot be trailng byte of a DBCS character.
     */
    if (str[0] == '\0') {
      return (size_t) -1;
    }
  }

  dbcs->Buffer[dbcs->Info.Bytes]  = str[0];
  dbcs->Info.Bytes               += 1;

  *count -= 1;

  /**
   * Check if we have consumed enough bytes to perform conversion.
   */
  if (dbcs->Info.Bytes == dbcs->Info.Length) {
    dbcs->Info.State = P32_MBSTATE_DBCS_COMPLETE;
  } else {
    dbcs->Info.State = P32_MBSTATE_DBCS_INCOMPLETE;
  }

  return !!str[0];
}
#endif

#ifdef P32_UCHAR_SBCS
/*******************************************************************************
 * SBCS
 */

#define P32_MBSTATE_SBCS_INITIAL    0x00
#define P32_MBSTATE_SBCS_INCOMPLETE 0x01
#define P32_MBSTATE_SBCS_COMPLETE   0x02
#define P32_MBSTATE_SBCS_RESERVED   0x03

/**
 * Internal conversion state for SBCS.
 */
typedef struct {
  ConversionStateInfo Info;
  union {
    char Buffer[1];
    char Byte;
  };
} SBCSConversionState;

/**
 * Set `state` to initial conversion state.
 */
static void P32SetSBCSConversionState (mbstate_t *state) {
  memset (state, 0, sizeof (mbstate_t));
}

/**
 * Conversion state is not used with SBCS code pages.
 */
static bool P32GetSBCSConversionState (mbstate_t *state) {
  return p32_mbsinit (state);
}

/**
 * Convert UTF-16 Code Unit Sequence to SBCS character.
 */
static bool P32UTF16ToSBCS (SBCSConversionState *sbcs, UTF16ConversionState *utf16, Charset *charset) {
#if P32_UCHAR_IMPL == P32_UCHAR_IMPL_SBCS
  BOOL defaultCharacterUsed = FALSE;

  int length = WideCharToMultiByte (
    charset->CodePage, charset->ToMultiByte, utf16->Buffer, utf16->Info.Length, sbcs->Buffer, charset->MaxLength, NULL,
    &defaultCharacterUsed
  );

  assert (length <= 1);

  if (length == 0 || length > (int) charset->MaxLength || defaultCharacterUsed) {
    return false;
  }

  sbcs->Info.Length = (uint8_t) length;
  sbcs->Info.Bytes  = (uint8_t) length;
#else
  if (utf16->Info.Length != 1) {
    return false;
  }

#if P32_UCHAR_IMPL == P32_UCHAR_IMPL_ASCII
  if (!iswascii (utf16->Unit1.Value)) {
    return false;
  }
#else
  if (utf16->Unit1.Value > 0xFF) {
    return false;
  }
#endif

  sbcs->Byte        = (char) utf16->Unit1.Value;
  sbcs->Info.Length = 1;
  sbcs->Info.Bytes  = 1;
#endif

  return true;
  UNREFERENCED_PARAMETER (charset);
}

/**
 * Convert SBCS character to UTF-16 Code Unit Sequence.
 */
static bool P32SBCSToUTF16 (UTF16ConversionState *utf16, SBCSConversionState *sbcs, Charset *charset) {
#if P32_UCHAR_IMPL == P32_UCHAR_IMPL_SBCS
  int length = MultiByteToWideChar (charset->CodePage, charset->ToWideChar, sbcs->Buffer, 1, utf16->Buffer, 2);

  assert (length <= 1);

  if (length == 0 || length > 2) {
    return false;
  }

  utf16->Info.Length = (uint8_t) length;
  utf16->Info.Bytes  = (uint8_t) length;
#else
  utf16->Unit1.Value = (uint8_t) sbcs->Byte;
  utf16->Info.Length = 1;
  utf16->Info.Bytes  = 1;
#endif

  return true;
  UNREFERENCED_PARAMETER (charset);
}

/**
 * Consume and validate single byte from `str`.
 */
static size_t P32GetSBCSByte (SBCSConversionState *sbcs, const char *str, size_t *count) {
  /**
   * We need to consume a single byte to perform conversion.
   */
  if (*count == 0) {
    return (size_t) -2;
  }

#if P32_UCHAR_IMPL == P32_UCHAR_IMPL_ASCII
  if (!__isascii (str[0])) {
    return (size_t) -1;
  }
#endif

  sbcs->Byte       = str[0];
  sbcs->Info.State = P32_MBSTATE_SBCS_COMPLETE;

  return !!str[0];
}
#endif

/*******************************************************************************
 * Generic Wrappers
 */

#ifdef P32_UCHAR_UTF8
#define P32_MBSTATE_MAGIC      P32_MBSTATE_UTF8_MAGIC
#define P32_MBSTATE_INITIAL    P32_MBSTATE_UTF8_INITIAL
#define P32_MBSTATE_INCOMPLETE P32_MBSTATE_UTF8_INCOMPLETE
#define P32_MBSTATE_COMPLETE   P32_MBSTATE_UTF8_COMPLETE

typedef union {
  UTF8ConversionState MB;
  UTF8ConversionState UTF8;
} MBConversionState;

static size_t P32GetMB (MBConversionState *mbState, const char *str, size_t *count, Charset *charset) {
  return P32GetUTF8Unit (&mbState->UTF8, str, count);
  UNREFERENCED_PARAMETER (charset);
}

static void P32SetMBConversionState (mbstate_t *state, MBConversionState *mbState, Charset *charset) {
  P32SetUTF8ConversionState (state, &mbState->UTF8);
  return;
  UNREFERENCED_PARAMETER (charset);
}

static bool P32GetMBConversionState (MBConversionState *mbState, mbstate_t *state, Charset *charset) {
  ConversionState conversionState = {0};
  P32GetConversionState (&mbState->MB.Info, &conversionState, state);
  return P32GetUTF8ConversionState (&mbState->UTF8, &conversionState, P32_MBSTATE_MAGIC);
  UNREFERENCED_PARAMETER (charset);
}

static bool P32CodePointToMB (MBConversionState *mbState, CodePoint *codePoint, Charset *charset) {
  return P32CodePointToUTF8 (&mbState->UTF8, codePoint);
  UNREFERENCED_PARAMETER (charset);
}

static bool P32MBToCodePoint (CodePoint *codePoint, MBConversionState *mbState, Charset *charset) {
  return P32UTF8ToCodePoint (codePoint, &mbState->UTF8);
  UNREFERENCED_PARAMETER (charset);
}

static bool P32UTF16ToMB (MBConversionState *mbState, UTF16ConversionState *utf16, Charset *charset) {
  CodePoint codePoint = {0};

  if (!P32UTF16ToCodePoint (&codePoint, utf16)) {
    return false;
  }

  if (!P32CodePointToUTF8 (&mbState->UTF8, &codePoint)) {
    return false;
  }

  return true;
  UNREFERENCED_PARAMETER (charset);
}

static bool P32MBToUTF16 (UTF16ConversionState *utf16, MBConversionState *mbState, Charset *charset) {
  CodePoint codePoint = {0};

  if (!P32UTF8ToCodePoint (&codePoint, &mbState->UTF8)) {
    return false;
  }

  if (!P32CodePointToUTF16 (utf16, &codePoint)) {
    return false;
  }

  return true;
  UNREFERENCED_PARAMETER (charset);
}

static bool P32MBToUTF8 (UTF8ConversionState *utf8, MBConversionState *mbState, Charset *charset) {
  CodePoint codePoint = {0};

  if (!P32UTF8ToCodePoint (&codePoint, &mbState->UTF8)) {
    return false;
  }

  *utf8 = mbState->UTF8;

  return true;
  UNREFERENCED_PARAMETER (charset);
}

static bool P32UTF8ToMB (MBConversionState *mbState, UTF8ConversionState *utf8, Charset *charset) {
  CodePoint codePoint = {0};

  if (!P32UTF8ToCodePoint (&codePoint, utf8)) {
    return false;
  }

  mbState->UTF8 = *utf8;

  return true;
  UNREFERENCED_PARAMETER (charset);
}
#elif P32_UCHAR_IMPL == P32_UCHAR_IMPL_DBCS
#define P32_MBSTATE_MAGIC      P32_MBSTATE_DBCS_MAGIC
#define P32_MBSTATE_INITIAL    P32_MBSTATE_DBCS_INITIAL
#define P32_MBSTATE_INCOMPLETE P32_MBSTATE_DBCS_INCOMPLETE
#define P32_MBSTATE_COMPLETE   P32_MBSTATE_DBCS_COMPLETE

typedef union {
  DBCSConversionState MB;
  UTF8ConversionState UTF8;
} MBConversionState;

static size_t P32GetMB (MBConversionState *mbState, const char *str, size_t *count, Charset *charset) {
  return P32GetDBCSByte (&mbState->MB, str, count, charset);
}

static void P32SetMBConversionState (mbstate_t *state, MBConversionState *mbState, Charset *charset) {
  if (mbState != NULL && mbState->MB.Info.State == P32_MBSTATE_COMPLETE) {
    P32SetUTF8ConversionState (state, &mbState->UTF8);
  } else {
    P32SetDBCSConversionState (state, &mbState->MB, charset);
  }
}

static bool P32GetMBConversionState (MBConversionState *mbState, mbstate_t *state, Charset *charset) {
  ConversionState conversionState = {0};

  P32GetConversionState (&mbState->MB.Info, &conversionState, state);

  if (mbState->MB.Info.State == P32_MBSTATE_COMPLETE) {
    return P32GetUTF8ConversionState (&mbState->UTF8, &conversionState, P32_MBSTATE_MAGIC);
  } else {
    return P32GetDBCSConversionState (&mbState->MB, &conversionState, state, charset);
  }
}

static bool P32CodePointToMB (MBConversionState *mbState, CodePoint *codePoint, Charset *charset) {
  UTF16ConversionState utf16 = {0};

  if (!P32CodePointToUTF16 (&utf16, codePoint)) {
    return false;
  }

  if (!P32UTF16ToDBCS (&mbState->MB, &utf16, charset)) {
    return false;
  }

  return true;
}

static bool P32MBToCodePoint (CodePoint *codePoint, MBConversionState *mbState, Charset *charset) {
  UTF16ConversionState utf16 = {0};

  if (!P32DBCSToUTF16 (&utf16, &mbState->MB, charset)) {
    return false;
  }

  if (!P32UTF16ToCodePoint (codePoint, &utf16)) {
    return false;
  }

  return true;
}

static bool P32UTF16ToMB (MBConversionState *mbState, UTF16ConversionState *utf16, Charset *charset) {
  return P32UTF16ToDBCS (&mbState->MB, utf16, charset);
}

static bool P32MBToUTF16 (UTF16ConversionState *utf16, MBConversionState *mbState, Charset *charset) {
  if (mbState->MB.Info.Length == mbState->MB.Info.Bytes) {
    if (!P32DBCSToUTF16 (utf16, &mbState->MB, charset)) {
      return false;
    }

    if (utf16->Info.Length == 1) {
      return true;
    }

    CodePoint codePoint = {0};

    if (!P32UTF16ToCodePoint (&codePoint, utf16)) {
      return false;
    }

    return P32CodePointToUTF8 (&mbState->UTF8, &codePoint);
  } else {
    CodePoint codePoint = {0};

    if (!P32UTF8ToCodePoint (&codePoint, &mbState->UTF8)) {
      return false;
    }

    return P32CodePointToUTF16 (utf16, &codePoint);
  }
}

static bool P32UTF8ToMB (MBConversionState *mbState, UTF8ConversionState *utf8, Charset *charset) {
  CodePoint codePoint = {0};

  if (!P32UTF8ToCodePoint (&codePoint, utf8)) {
    return false;
  }

  if (!P32CodePointToMB (mbState, &codePoint, charset)) {
    return false;
  }

  return true;
}

static bool P32MBToUTF8 (UTF8ConversionState *utf8, MBConversionState *mbState, Charset *charset) {
  if (mbState->MB.Info.Length == mbState->MB.Info.Bytes) {
    CodePoint codePoint = {0};

    if (!P32MBToCodePoint (&codePoint, mbState, charset)) {
      return false;
    }

    if (!P32CodePointToUTF8 (utf8, &codePoint)) {
      return false;
    }

    utf8->Info.Magic = mbState->MB.Info.Magic;
    utf8->Info.State = mbState->MB.Info.State;
  } else {
    *utf8 = mbState->UTF8;
  }

  return true;
}
#else
#define P32_MBSTATE_MAGIC      P32_MBSTATE_SBCS_MAGIC
#define P32_MBSTATE_INITIAL    P32_MBSTATE_SBCS_INITIAL
#define P32_MBSTATE_INCOMPLETE P32_MBSTATE_SBCS_INCOMPLETE
#define P32_MBSTATE_COMPLETE   P32_MBSTATE_SBCS_COMPLETE

typedef union {
  SBCSConversionState MB;
  UTF8ConversionState UTF8;
} MBConversionState;

static size_t P32GetMB (MBConversionState *mbState, const char *str, size_t *count, Charset *charset) {
  return P32GetSBCSByte (&mbState->MB, str, count);
  UNREFERENCED_PARAMETER (charset);
}

static void P32SetMBConversionState (mbstate_t *state, MBConversionState *mbState, Charset *charset) {
  if (mbState != NULL && mbState->MB.Info.State == P32_MBSTATE_COMPLETE) {
    P32SetUTF8ConversionState (state, &mbState->UTF8);
  } else {
    P32SetSBCSConversionState (state);
  }

  return;
  UNREFERENCED_PARAMETER (charset);
}

static bool P32GetMBConversionState (MBConversionState *mbState, mbstate_t *state, Charset *charset) {
  ConversionState conversionState = {0};

  P32GetConversionState (&mbState->MB.Info, &conversionState, state);

  if (mbState->MB.Info.State == P32_MBSTATE_COMPLETE) {
    return P32GetUTF8ConversionState (&mbState->UTF8, &conversionState, P32_MBSTATE_MAGIC);
  } else {
    return P32GetSBCSConversionState (state);
  }

  UNREFERENCED_PARAMETER (charset);
}

static bool P32CodePointToMB (MBConversionState *mbState, CodePoint *codePoint, Charset *charset) {
  UTF16ConversionState utf16 = {0};

  if (!P32CodePointToUTF16 (&utf16, codePoint)) {
    return false;
  }

  if (!P32UTF16ToSBCS (&mbState->MB, &utf16, charset)) {
    return false;
  }

  return true;
}

static bool P32MBToCodePoint (CodePoint *codePoint, MBConversionState *mbState, Charset *charset) {
  UTF16ConversionState utf16 = {0};

  if (!P32SBCSToUTF16 (&utf16, &mbState->MB, charset)) {
    return false;
  }

  if (!P32UTF16ToCodePoint (codePoint, &utf16)) {
    return false;
  }

  return true;
}

static bool P32UTF16ToMB (MBConversionState *mbState, UTF16ConversionState *utf16, Charset *charset) {
  return P32UTF16ToSBCS (&mbState->MB, utf16, charset);
}

static bool P32MBToUTF16 (UTF16ConversionState *utf16, MBConversionState *mbState, Charset *charset) {
  if (mbState->MB.Info.Length == mbState->MB.Info.Bytes) {
    if (!P32SBCSToUTF16 (utf16, &mbState->MB, charset)) {
      return false;
    }

    if (utf16->Info.Length == 1) {
      return true;
    }

    CodePoint codePoint = {0};

    if (!P32UTF16ToCodePoint (&codePoint, utf16)) {
      return false;
    }

    return P32CodePointToUTF8 (&mbState->UTF8, &codePoint);
  } else {
    CodePoint codePoint = {0};

    if (!P32UTF8ToCodePoint (&codePoint, &mbState->UTF8)) {
      return false;
    }

    return P32CodePointToUTF16 (utf16, &codePoint);
  }
}

static bool P32UTF8ToMB (MBConversionState *mbState, UTF8ConversionState *utf8, Charset *charset) {
  CodePoint codePoint = {0};

  if (!P32UTF8ToCodePoint (&codePoint, utf8)) {
    return false;
  }

  if (!P32CodePointToMB (mbState, &codePoint, charset)) {
    return false;
  }

  return true;
}

static bool P32MBToUTF8 (UTF8ConversionState *utf8, MBConversionState *mbState, Charset *charset) {
  if (mbState->MB.Info.Length == mbState->MB.Info.Bytes) {
    CodePoint codePoint = {0};

    if (!P32MBToCodePoint (&codePoint, mbState, charset)) {
      return false;
    }

    if (!P32CodePointToUTF8 (utf8, &codePoint)) {
      return false;
    }

    utf8->Info.Magic = mbState->MB.Info.Magic;
    utf8->Info.State = mbState->MB.Info.State;
  } else {
    *utf8 = mbState->UTF8;
  }

  return true;
}
#endif

P32_STATIC_ASSERT (offsetof (MBConversionState, MB.Info.Magic) == offsetof (MBConversionState, UTF8.Info.Magic), "");
P32_STATIC_ASSERT (offsetof (MBConversionState, MB.Info.State) == offsetof (MBConversionState, UTF8.Info.State), "");
P32_STATIC_ASSERT (offsetof (MBConversionState, MB.Info.Bytes) == offsetof (MBConversionState, UTF8.Info.Bytes), "");
P32_STATIC_ASSERT (offsetof (MBConversionState, MB.Info.Length) == offsetof (MBConversionState, UTF8.Info.Length), "");

/**
 * Input string to be used when `mbrtoc8`, `mbrtoc16` or `mbrtoc32` is called
 * with NULL as their second argument.
 */
static const char EmptyString[] = {'\0'};

size_t mbrtoc8 (char8_t *u8, const char *mbs, size_t count, mbstate_t *state, Charset *charset) {
  assert (state != NULL);

  /**
   * Internal conversion state.
   */
  MBConversionState conversionState = {0};

  if (!P32GetMBConversionState (&conversionState, state, charset)) {
    goto einval;
  }

  /**
   * If `state` contains complete UTF-8 Code Unit Sequence, we need to verify
   * that is was consumed by `mbrtoc8` and not `mbrtoc16`.
   */
  if (conversionState.MB.Info.State == P32_MBSTATE_COMPLETE) {
    if (conversionState.MB.Info.Bytes == 0) {
      goto einval;
    }
  }

  if (conversionState.MB.Info.Magic == 0) {
    conversionState.MB.Info.Magic = P32_MBSTATE_MAGIC;
  }

  /**
   * Calling `mbrtoc8 (..., NULL, ..., state)` is equivalent to calling
   * `mbrtoc8 (NULL, "", 1, state)`.
   */
  if (mbs == NULL) {
    u8    = NULL;
    mbs   = EmptyString;
    count = 1;
  }

  /**
   * Number of bytes consumed from `mbs`.
   */
  size_t bytesConsumed = 0;

  while (1) {
    /**
     * We have consumed enough bytes to perfrom conversion.
     */
    if (conversionState.MB.Info.State == P32_MBSTATE_COMPLETE) {
      break;
    }

    const size_t ret = P32GetMB (&conversionState, mbs, &count, charset);

    /**
     * `mbs` points to invalid byte.
     */
    if (ret == (size_t) -1) {
      goto eilseq;
    }

    /**
     * Need to consume more bytes to perform conversion.
     */
    if (ret == (size_t) -2) {
      goto incomplete;
    }

    bytesConsumed += ret;
    mbs           += ret;
  }

  /**
   * UTF-8 Code Unit Sequence.
   */
  UTF8ConversionState utf8 = {0};

  if (!P32MBToUTF8 (&utf8, &conversionState, charset)) {
    goto eilseq;
  }

  if (u8 != NULL) {
    if (utf8.Info.Length == 4) {
      if (utf8.Info.Bytes == 4) {
        *u8 = utf8.Unit1.Value;
      } else if (utf8.Info.Bytes == 3) {
        *u8 = utf8.Unit2.Value;
      } else if (utf8.Info.Bytes == 2) {
        *u8 = utf8.Unit3.Value;
      } else {
        *u8 = utf8.Unit4.Value;
      }
    } else if (utf8.Info.Length == 3) {
      if (utf8.Info.Bytes == 3) {
        *u8 = utf8.Unit1.Value;
      } else if (utf8.Info.Bytes == 2) {
        *u8 = utf8.Unit2.Value;
      } else {
        *u8 = utf8.Unit3.Value;
      }
    } else if (utf8.Info.Length == 2) {
      if (utf8.Info.Bytes == 2) {
        *u8 = utf8.Unit1.Value;
      } else {
        *u8 = utf8.Unit2.Value;
      }
    } else {
      *u8 = utf8.Unit1.Value;
    }
  }

  size_t ret = 0;

  if (utf8.Info.Length == utf8.Info.Bytes) {
    ret = bytesConsumed;
  } else {
    ret = (size_t) -3;
  }

  utf8.Info.Bytes -= 1;

  if (utf8.Info.Bytes == 0) {
    P32SetUTF8ConversionState (state, NULL);
  } else {
    P32SetUTF8ConversionState (state, &utf8);
  }

  return ret;

incomplete:
  P32SetMBConversionState (state, &conversionState, charset);
  return (size_t) -2;

eilseq:
  _set_errno (EILSEQ);
  return (size_t) -1;

einval:
  _set_errno (EINVAL);
  return (size_t) -1;
}

size_t c8rtomb (char *str, char8_t u8, mbstate_t *state, Charset *charset) {
  assert (state != NULL);

  /**
   * Set `state` to initial conversion state.
   */
  if (str == NULL) {
    P32SetUTF8ConversionState (state, NULL);
    return 1;
  }

  /**
   * Unpacked `state`
   */
  ConversionState conversionState = {0};

  /**
   * Internal conversion state.
   */
  UTF8ConversionState utf8 = {0};

  P32GetConversionState (&utf8.Info, &conversionState, state);

  if (!P32GetUTF8ConversionState (&utf8, &conversionState, P32_MBSTATE_UTF8_MAGIC)) {
    goto einval;
  }

  if (utf8.Info.Magic == 0) {
    utf8.Info.Magic = P32_MBSTATE_UTF8_MAGIC;
  }

  do {
    size_t       count  = 1;
    const size_t length = P32GetUTF8Unit (&utf8, (char *) &u8, &count);

    assert (length != (size_t) -2);

    /**
     * `u8` is invalid.
     */
    if (length == (size_t) -1) {
      goto eilseq;
    }

    if (utf8.Info.State != P32_MBSTATE_UTF8_COMPLETE) {
      goto incomplete;
    }

    assert (utf8.Info.State == P32_MBSTATE_UTF8_COMPLETE);
  } while (0);

  MBConversionState mbState = {0};

  if (!P32UTF8ToMB (&mbState, &utf8, charset)) {
    goto eilseq;
  }

  if (str != NULL) {
    memcpy (str, mbState.MB.Buffer, mbState.MB.Info.Length);
  }

  P32SetUTF8ConversionState (state, NULL);

  return mbState.MB.Info.Length;

incomplete:
  P32SetUTF8ConversionState (state, &utf8);
  return 0;

eilseq:
  _set_errno (EILSEQ);
  return (size_t) -1;

einval:
  _set_errno (EINVAL);
  return (size_t) -1;
}

size_t mbrtoc16 (char16_t *u16, const char *mbs, size_t count, mbstate_t *state, Charset *charset) {
  assert (state != NULL);

  /**
   * Internal conversion state.
   */
  MBConversionState conversionState = {0};

  if (!P32GetMBConversionState (&conversionState, state, charset)) {
    goto einval;
  }

  /**
   * If `state` contains complete UTF-8 Code Unit Sequence, we need to verify
   * that is was consumed by `mbrtoc16` and not `mbrtoc8`.
   */
  if (conversionState.MB.Info.State == P32_MBSTATE_COMPLETE) {
    if (conversionState.MB.Info.Length != 4 || conversionState.UTF8.Info.Bytes != 0) {
      goto einval;
    }
  }

  if (conversionState.MB.Info.Magic == 0) {
    conversionState.MB.Info.Magic = P32_MBSTATE_MAGIC;
  }

  /**
   * Calling `mbrtoc16 (..., NULL, ..., state)` is equivalent to calling
   * `mbrtoc16 (NULL, "", 1, state)`
   */
  if (mbs == NULL) {
    u16   = NULL;
    mbs   = EmptyString;
    count = 1;
  }

  /**
   * Number of bytes consumed from `mbs`.
   */
  size_t bytesConsumed = 0;

  while (1) {
    /**
     * We have consumed enough bytes to perfrom conversion.
     */
    if (conversionState.MB.Info.State == P32_MBSTATE_COMPLETE) {
      break;
    }

    const size_t ret = P32GetMB (&conversionState, mbs, &count, charset);

    /**
     * `mbs` points to invalid byte.
     */
    if (ret == (size_t) -1) {
      goto eilseq;
    }

    /**
     * Need to consume more bytes to perform conversion.
     */
    if (ret == (size_t) -2) {
      goto incomplete;
    }

    bytesConsumed += ret;
    mbs           += ret;
  }

  /**
   * UTF-16 Code Unit Sequence.
   */
  UTF16ConversionState utf16 = {0};

  if (!P32MBToUTF16 (&utf16, &conversionState, charset)) {
    goto eilseq;
  }

  /**
   * Write single non-surrogate UTF-16 Code Unit.
   */
  if (utf16.Info.Length == 1) {
    P32SetUTF8ConversionState (state, NULL);

    if (u16 != NULL) {
      *u16 = utf16.Unit1.Value;
    }

    return bytesConsumed;
  }

  /**
   * Write high surrogate.
   */
  if (conversionState.MB.Info.Bytes == 4) {
    conversionState.MB.Info.Bytes = 0;
    P32SetUTF8ConversionState (state, &conversionState.UTF8);

    if (u16 != NULL) {
      *u16 = utf16.Unit1.Value;
    }

    return bytesConsumed;

    /**
     * Write low surrogate.
     */
  } else {
    P32SetUTF8ConversionState (state, NULL);

    if (u16 != NULL) {
      *u16 = utf16.Unit2.Value;
    }

    return (size_t) -3;
  }

incomplete:
  P32SetMBConversionState (state, &conversionState, charset);
  return (size_t) -2;

eilseq:
  _set_errno (EILSEQ);
  return (size_t) -1;

einval:
  _set_errno (EINVAL);
  return (size_t) -1;
}

size_t c16rtomb (char *str, char16_t u16, mbstate_t *state, Charset *charset) {
  assert (state != NULL);

  /**
   * Set `state` to initial conversion state.
   */
  if (str == NULL) {
    P32SetUTF16ConversionState (state, NULL);
    return 1;
  }

  /**
   * Internal conversion state.
   */
  UTF16ConversionState utf16 = {0};

  if (!P32GetUTF16ConversionState (&utf16, state)) {
    goto einval;
  }

  /**
   * Identify conversion state.
   */
  if (utf16.Info.Magic == 0) {
    utf16.Info.Magic = P32_MBSTATE_UTF16_MAGIC;
  }

  /**
   * If `utf16` contains initial conversion state, then `u16char`must be
   * either non-surrogate or high surrogate UTF-16 Code Unit.
   */
  if (utf16.Info.State == P32_MBSTATE_UTF16_INITIAL) {
    if (IS_LOW_SURROGATE (u16)) {
      goto eilseq;
    }

    if (IS_HIGH_SURROGATE (u16)) {
      utf16.Info.Length = 2;
      utf16.Info.Bytes  = 1;
      utf16.Unit1.Value = u16;
    } else {
      utf16.Info.Length = 1;
      utf16.Info.Bytes  = 1;
      utf16.Unit1.Value = u16;
    }

    /**
     * We have previously consumed high surrogate, consumed `u16char` must be
     * a low surrogate.
     */
  } else if (utf16.Info.State == P32_MBSTATE_UTF16_INCOMPLETE) {
    if (!IS_LOW_SURROGATE (u16)) {
      goto eilseq;
    }

    utf16.Info.Bytes  += 1;
    utf16.Unit2.Value  = u16;
  }

  /**
   * We have consumed high surrogate, we need to consume low surrogate
   * in order to perform conversion.
   */
  if (utf16.Info.Bytes != utf16.Info.Length) {
    utf16.Info.State = P32_MBSTATE_UTF16_INCOMPLETE;
    goto incomplete;
  }

  MBConversionState conversionState = {0};

  if (!P32UTF16ToMB (&conversionState, &utf16, charset)) {
    goto eilseq;
  }

  if (str != NULL) {
    memcpy (str, conversionState.MB.Buffer, conversionState.MB.Info.Length);
  }

  P32SetUTF16ConversionState (state, NULL);

  return conversionState.MB.Info.Length;

incomplete:
  P32SetUTF16ConversionState (state, &utf16);
  return 0;

eilseq:
  _set_errno (EILSEQ);
  return (size_t) -1;

einval:
  _set_errno (EINVAL);
  return (size_t) -1;
}

size_t mbrtoc32 (char32_t *u32, const char *mbs, size_t count, mbstate_t *state, Charset *charset) {
  assert (state != NULL);

  /**
   * Internal conversion state.
   */
  MBConversionState conversionState = {0};

  if (!P32GetMBConversionState (&conversionState, state, charset)) {
    goto einval;
  }

  /**
   * `mbrtoc32` never stores complete UTF-8 Code Unit Sequence in `state`.
   */
  if (conversionState.MB.Info.State == P32_MBSTATE_COMPLETE) {
    goto einval;
  }

  if (conversionState.MB.Info.Magic == 0) {
    conversionState.MB.Info.Magic = P32_MBSTATE_MAGIC;
  }

  /**
   * Calling `mbrtoc32 (..., NULL, ..., state)` is equivalent to calling
   * `mbrtoc32 (NULL, "", 1, state)`
   */
  if (mbs == NULL) {
    u32   = NULL;
    mbs   = EmptyString;
    count = 1;
  }

  /**
   * Number of bytes consumed from `mbs`.
   */
  size_t bytesConsumed = 0;

  while (1) {
    /**
     * We have consumed enough bytes to perfrom conversion.
     */
    if (conversionState.MB.Info.State == P32_MBSTATE_COMPLETE) {
      break;
    }

    const size_t ret = P32GetMB (&conversionState, mbs, &count, charset);

    /**
     * `mbs` points to invalid byte.
     */
    if (ret == (size_t) -1) {
      goto eilseq;
    }

    /**
     * Need to consume more bytes to perform conversion.
     */
    if (ret == (size_t) -2) {
      goto incomplete;
    }

    bytesConsumed += ret;
    mbs           += ret;
  }

  /**
   * Code Point constructed from Multibyte Sequence.
   */
  CodePoint codePoint = {0};

  if (!P32MBToCodePoint (&codePoint, &conversionState, charset)) {
    goto eilseq;
  }

  P32SetMBConversionState (state, NULL, charset);

  if (u32 != NULL) {
    *u32 = codePoint.Value;
  }

  return bytesConsumed;

incomplete:
  P32SetMBConversionState (state, &conversionState, charset);
  return (size_t) -2;

eilseq:
  _set_errno (EILSEQ);
  return (size_t) -1;

einval:
  _set_errno (EINVAL);
  return (size_t) -1;
}

size_t c32rtomb (char *str, char32_t u32, mbstate_t *state, Charset *charset) {
  assert (state != NULL);

  /**
   * Set `state` to initial conversion state.
   */
  if (str == NULL) {
    memset (state, 0, sizeof (mbstate_t));
    return 1;
  }

  /**
   * `mbrtoc32` does not use conversion state.
   * `state` must always describe initial conversion state.
   */
  if (!p32_mbsinit (state)) {
    goto einval;
  }

  /**
   * `u32char` is a single Unicdeo Code Point.
   */
  CodePoint codePoint = {.Value = u32};

  /**
   * Multibyte Character Sequence constructed from `codePoint`.
   */
  MBConversionState conversionState = {0};

  if (!P32CodePointToMB (&conversionState, &codePoint, charset)) {
    goto eilseq;
  }

  if (str != NULL) {
    memcpy (str, conversionState.MB.Buffer, conversionState.MB.Info.Length);
  }

  return conversionState.MB.Info.Length;

eilseq:
  _set_errno (EILSEQ);
  return (size_t) -1;

einval:
  _set_errno (EINVAL);
  return (size_t) -1;
}
