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

#ifndef LIBPOSIX32_CTYPE_TEST_H_INCLUDED
#define LIBPOSIX32_CTYPE_TEST_H_INCLUDED

#include "tests-internal.h"

/**
 * "digit" implies "alnum"
 */
#define CHARTYPE_DIGIT (P32_CHARTYPE_ALNUM | P32_CHARTYPE_DIGIT)

/**
 * "graph" implies "print"
 */
#define CHARTYPE_GRAPH (P32_CHARTYPE_GRAPH | P32_CHARTYPE_PRINT)

/**
 * "lower" implies "alnum" and "alpha"
 */
#define CHARTYPE_LOWER (P32_CHARTYPE_ALNUM | P32_CHARTYPE_ALPHA | P32_CHARTYPE_LOWER)

/**
 * "upper" implies "alnum" and "alpha"
 */
#define CHARTYPE_UPPER (P32_CHARTYPE_ALNUM | P32_CHARTYPE_ALPHA | P32_CHARTYPE_UPPER)

struct {
  uint16_t CharType;
  int      Lower;
  int      Upper;
} CharTypeTable[] = {
  // NUL
  {(P32_CHARTYPE_CONTROL),                                           0x00, 0x00},
  // SOH
  {(P32_CHARTYPE_CONTROL),                                           0x01, 0x01},
  // STX
  {(P32_CHARTYPE_CONTROL),                                           0x02, 0x02},
  // ETX
  {(P32_CHARTYPE_CONTROL),                                           0x03, 0x03},
  // EOT
  {(P32_CHARTYPE_CONTROL),                                           0x04, 0x04},
  // ENQ
  {(P32_CHARTYPE_CONTROL),                                           0x05, 0x05},
  // ACK
  {(P32_CHARTYPE_CONTROL),                                           0x06, 0x06},
  // bell
  {(P32_CHARTYPE_CONTROL),                                           0x07, 0x07},
  // backspace
  {(P32_CHARTYPE_CONTROL),                                           0x08, 0x08},
  // TAB
  {(P32_CHARTYPE_BLANK | P32_CHARTYPE_CONTROL | P32_CHARTYPE_SPACE), 0x09, 0x09},
  // LF
  {(P32_CHARTYPE_CONTROL | P32_CHARTYPE_SPACE),                      0x0A, 0x0A},
  // vertical tab
  {(P32_CHARTYPE_CONTROL | P32_CHARTYPE_SPACE),                      0x0B, 0x0B},
  // form feed
  {(P32_CHARTYPE_CONTROL | P32_CHARTYPE_SPACE),                      0x0C, 0x0C},
  // CR
  {(P32_CHARTYPE_CONTROL | P32_CHARTYPE_SPACE),                      0x0D, 0x0D},
  // SO
  {(P32_CHARTYPE_CONTROL),                                           0x0E, 0x0E},
  // SI
  {(P32_CHARTYPE_CONTROL),                                           0x0F, 0x0F},
  // DLE
  {(P32_CHARTYPE_CONTROL),                                           0x10, 0x10},
  // DC1
  {(P32_CHARTYPE_CONTROL),                                           0x11, 0x11},
  // DC2
  {(P32_CHARTYPE_CONTROL),                                           0x12, 0x12},
  // DC3
  {(P32_CHARTYPE_CONTROL),                                           0x13, 0x13},
  // DC4
  {(P32_CHARTYPE_CONTROL),                                           0x14, 0x14},
  // NAK
  {(P32_CHARTYPE_CONTROL),                                           0x15, 0x15},
  // SYN
  {(P32_CHARTYPE_CONTROL),                                           0x16, 0x16},
  // ETB
  {(P32_CHARTYPE_CONTROL),                                           0x17, 0x17},
  // CAN
  {(P32_CHARTYPE_CONTROL),                                           0x18, 0x18},
  // EM
  {(P32_CHARTYPE_CONTROL),                                           0x19, 0x19},
  // SUB
  {(P32_CHARTYPE_CONTROL),                                           0x1A, 0x1A},
  // ESC
  {(P32_CHARTYPE_CONTROL),                                           0x1B, 0x1B},
  // IS4
  {(P32_CHARTYPE_CONTROL),                                           0x1C, 0x1C},
  // IS3
  {(P32_CHARTYPE_CONTROL),                                           0x1D, 0x1D},
  // IS2
  {(P32_CHARTYPE_CONTROL),                                           0x1E, 0x1E},
  // IS1
  {(P32_CHARTYPE_CONTROL),                                           0x1F, 0x1F},
  // space
  {(P32_CHARTYPE_BLANK | P32_CHARTYPE_PRINT | P32_CHARTYPE_SPACE),   0x20, 0x20},
  // !
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x21, 0x21},
  // "
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x22, 0x22},
  // #
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x23, 0x23},
  // $
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x24, 0x24},
  // %
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x25, 0x25},
  // &
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x26, 0x26},
  // '
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x27, 0x27},
  // (
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x28, 0x28},
  // )
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x29, 0x29},
  // *
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x2A, 0x2A},
  // +
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x2B, 0x2B},
  // ,
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x2C, 0x2C},
  // -
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x2D, 0x2D},
  // .
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x2E, 0x2E},
  // /
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x2F, 0x2F},
  // 0
  {(CHARTYPE_DIGIT | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x30, 0x30},
  // 1
  {(CHARTYPE_DIGIT | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x31, 0x31},
  // 2
  {(CHARTYPE_DIGIT | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x32, 0x32},
  // 3
  {(CHARTYPE_DIGIT | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x33, 0x33},
  // 4
  {(CHARTYPE_DIGIT | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x34, 0x34},
  // 5
  {(CHARTYPE_DIGIT | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x35, 0x35},
  // 6
  {(CHARTYPE_DIGIT | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x36, 0x36},
  // 7
  {(CHARTYPE_DIGIT | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x37, 0x37},
  // 8
  {(CHARTYPE_DIGIT | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x38, 0x38},
  // 9
  {(CHARTYPE_DIGIT | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x39, 0x39},
  // :
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x3A, 0x3A},
  // ;
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x3B, 0x3B},
  // <
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x3C, 0x3C},
  // =
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x3D, 0x3D},
  // >
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x3E, 0x3E},
  // ?
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x3F, 0x3F},
  // @
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x40, 0x40},
  // A
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x61, 0x41},
  // B
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x62, 0x42},
  // C
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x63, 0x43},
  // D
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x64, 0x44},
  // E
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x65, 0x45},
  // F
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x66, 0x46},
  // G
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x67, 0x47},
  // H
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x68, 0x48},
  // I
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x69, 0x49},
  // J
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x6A, 0x4A},
  // K
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x6B, 0x4B},
  // L
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x6C, 0x4C},
  // M
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x6D, 0x4D},
  // N
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x6E, 0x4E},
  // O
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x6F, 0x4F},
  // P
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x70, 0x50},
  // Q
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x71, 0x51},
  // R
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x72, 0x52},
  // S
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x73, 0x53},
  // T
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x74, 0x54},
  // U
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x75, 0x55},
  // V
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x76, 0x56},
  // W
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x77, 0x57},
  // X
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x78, 0x58},
  // Y
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x79, 0x59},
  // Z
  {(CHARTYPE_UPPER | CHARTYPE_GRAPH),                                0x7A, 0x5A},
  // [
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x5B, 0x5B},
  // '\'
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x5C, 0x5C},
  // ]
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x5D, 0x5D},
  // ^
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x5E, 0x5E},
  // _
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x5F, 0x5F},
  // `
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x60, 0x60},
  // a
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x61, 0x41},
  // b
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x62, 0x42},
  // c
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x63, 0x43},
  // d
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x64, 0x44},
  // e
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x65, 0x45},
  // f
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH | P32_CHARTYPE_XDIGIT),          0x66, 0x46},
  // g
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x67, 0x47},
  // h
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x68, 0x48},
  // i
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x69, 0x49},
  // j
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x6A, 0x4A},
  // k
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x6B, 0x4B},
  // l
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x6C, 0x4C},
  // m
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x6D, 0x4D},
  // n
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x6E, 0x4E},
  // o
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x6F, 0x4F},
  // p
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x70, 0x50},
  // q
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x71, 0x51},
  // r
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x72, 0x52},
  // s
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x73, 0x53},
  // t
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x74, 0x54},
  // u
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x75, 0x55},
  // v
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x76, 0x56},
  // w
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x77, 0x57},
  // x
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x78, 0x58},
  // y
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x79, 0x59},
  // z
  {(CHARTYPE_LOWER | CHARTYPE_GRAPH),                                0x7A, 0x5A},
  // {
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x7B, 0x7B},
  // |
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x7C, 0x7C},
  // }
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x7D, 0x7D},
  // ~
  {(CHARTYPE_GRAPH | P32_CHARTYPE_PUNCT),                            0x7E, 0x7E},
  // DEL
  {(P32_CHARTYPE_CONTROL),                                           0x7F, 0x7F},
};

#endif /* LIBPOSIX32_CTYPE_TEST_H_INCLUDED */
