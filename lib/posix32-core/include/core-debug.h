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

#ifndef LIBPOSIX32_CORE_DEBUG_H_INCLUDED
#define LIBPOSIX32_CORE_DEBUG_H_INCLUDED

#include <stdbool.h>
#include <wchar.h>

#include "posix32.h"

/**
 * Returns `true` if process is running under debugger.
 *
 * This function is a portability wrapper for `IsDebuggerPresent`.
 */
P32_CORE_DECL bool p32_dbg_is_debugger_present (void);

/**
 * Send `string` to the debugger.
 *
 * This function is a portability wrapper for `OutputDebugString`.
 */
P32_CORE_DECL void p32_dbg_output_string (const wchar_t *string);

#ifdef LIBPOSIX32_TEST
/**
 * Message types supported by `p32_dbg_write_message`.
 */
typedef enum DbgMessageType {
  DbgMessageInfo,
  DbgMessageWarning,
  DbgMessageError,
} DbgMessageType;

/**
 * Write debug message of `type`.
 */
P32_CORE_DECL void p32_dbg_write_message (
  DbgMessageType type,
  const wchar_t *filename,
  int            line,
  const wchar_t *format,
  ...
);

#ifdef _DEBUG
#define p32_dbg_break_impl()                      _CrtDbgBreak ()
#define p32_dbg_message_impl(filename, line, ...) p32_dbg_write_message (DbgMessageInfo, filename, line, __VA_ARGS__)
#define p32_dbg_warning_impl(filename, line, ...) _CrtDbgReportW (_CRT_WARN, filename, line, NULL, __VA_ARGS__)
#define p32_dbg_error_impl(filename, line, ...)   _CrtDbgReportW (_CRT_ERROR, filename, line, NULL, __VA_ARGS__)
#else
#define p32_dbg_break_impl()                      DebugBreak ()
#define p32_dbg_message_impl(filename, line, ...) p32_dbg_write_message (DbgMessageInfo, filename, line, __VA_ARGS__)
#define p32_dbg_warning_impl(filename, line, ...) p32_dbg_write_message (DbgMessageWarning, filename, line, __VA_ARGS__)
#define p32_dbg_error_impl(filename, line, ...)   p32_dbg_write_message (DbgMessageError, filename, line, __VA_ARGS__)
#endif

/**
 * Trigger breakpoint exception to transfer control to the debugger.
 */
#define p32_dbg_break()                   \
  do {                                    \
    if (p32_dbg_is_debugger_present ()) { \
      p32_dbg_break_impl ();              \
    }                                     \
  } while (0)

/**
 * Write message to `stdout`.
 */
#define p32_dbg_message(format, ...) p32_dbg_message_impl (NULL, 0, format __VA_OPT__ (, ) __VA_ARGS__)

/**
 * Write warning message to `stderr`.
 */
#define p32_dbg_warning(format, ...) p32_dbg_warning_impl (NULL, 0, format __VA_OPT__ (, ) __VA_ARGS__)

/**
 * Write error message to `stderr`.
 *
 * If process is running under debugger, trigger a breakpoint exception to
 * transfer control to the debugger.
 */
#define p32_dbg_error(format, ...)                                    \
  do {                                                                \
    p32_dbg_error_impl (NULL, 0, format __VA_OPT__ (, ) __VA_ARGS__); \
    p32_dbg_break ();                                                 \
  } while (0)

#else /* Normal build */

#define p32_dbg_break()              (void) 0
#define p32_dbg_message(format, ...) (void) 0
#define p32_dbg_warning(format, ...) (void) 0
#define p32_dbg_error(format, ...)   (void) 0

#endif /* Normal build */

#endif /* LIBPOSIX32_CORE_DEBUG_H_INCLUDED */
