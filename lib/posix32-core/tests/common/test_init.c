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

#if P32_CRT >= P32_MSVCR80
/**
 * Invalid parameter handler.
 *
 * Write a `_CRT_ERROR` debug message and terminate the process.
 */
static void __cdecl P32InvalidParameterHandler (
  const wchar_t *expression,
  const wchar_t *function,
  const wchar_t *filename,
  unsigned int   line,
  uintptr_t      reserved
) {
  /**
   * The arguments are only valid when using debug version of CRT.
   */
#ifdef _DEBUG
  p32_dbg_error (L"%s:%u: invalid parameter has been passed to %s: %s\n", filename, line, function, expression);
#endif

  p32_terminate (L"The invalid parameter handler has been invoked.");

  UNREFERENCED_PARAMETER (expression);
  UNREFERENCED_PARAMETER (function);
  UNREFERENCED_PARAMETER (filename);
  UNREFERENCED_PARAMETER (line);
  UNREFERENCED_PARAMETER (reserved);
}
#endif

static int P32TestFini (void) {
#ifdef _DEBUG
  _CrtCheckMemory ();
  _CrtDumpMemoryLeaks ();
#endif

  return EXIT_SUCCESS;
}

void p32_test_init (void) {
  UINT errorMode = 0;

#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
  errorMode |= SEM_FAILCRITICALERRORS;
  errorMode |= SEM_NOGPFAULTERRORBOX;
  errorMode |= SEM_NOOPENFILEERRORBOX;
#endif

  SetErrorMode (errorMode);

#if P32_CRT >= P32_MSVCR80
  /**
   * Set invalid parameter handler which prints a debug message and then
   * terminates the process.
   */
  _set_invalid_parameter_handler (P32InvalidParameterHandler);
#endif

  /**
   * msvcr80.dll and later attempt to set multibyte code page to active
   * ANSI code page during initialization.
   *
   * If active ANSI code page is 65001 (UTF-8), _setmbcp will fail
   * and set `errno` to EINVAL.
   *
   * Some of our tests assume `errno` to be set to zero when entering main.
   */
  _set_errno (0);

  /**
   * Disable buffering. This ensures that all output is written to log file in
   * case an assertion fails.
   */
  setvbuf (stdout, NULL, _IONBF, 0);
  setvbuf (stderr, NULL, _IONBF, 0);

#if P32_CRT >= P32_MSVCR80
  /**
   * Make `abort` as silent as possible.
   */
  _set_abort_behavior (0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif

#if P32_CRT >= P32_MSVCRT40
#if WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP
  /**
   * We don't want failed assertions to pop-up a message box.
   */
  assert (_set_error_mode (_OUT_TO_STDERR) != -1);
#endif
#endif

#if P32_CRT >= P32_MSVCR80
  assert (_setmode (_fileno (stdout), _O_U8TEXT) != -1);
  assert (_setmode (_fileno (stderr), _O_U8TEXT) != -1);
#endif

#ifdef _DEBUG
  /**
   * Usually, we want `_CrtDbgReport` functions to write debug messages
   * to the log file, or `stderr` if running tests directly.
   *
   * However, if the process was started under debugger, we will send these
   * messages to the debugger instead; this should provide better debugging
   * experience.
   *
   * We do not send them to both; otherwise, all messages would be repeated
   * twice when using debuggers such `gdb` or `lldb`.
   */
  int reportMode = _CRTDBG_MODE_FILE;

  if (p32_dbg_is_debugger_present ()) {
    reportMode = _CRTDBG_MODE_DEBUG;
  }

  assert (_CrtSetReportMode (_CRT_WARN, reportMode) != -1);
  assert (_CrtSetReportFile (_CRT_WARN, _CRTDBG_FILE_STDOUT) != _CRTDBG_HFILE_ERROR);

  assert (_CrtSetReportMode (_CRT_ERROR, reportMode) != -1);
  assert (_CrtSetReportFile (_CRT_ERROR, _CRTDBG_FILE_STDERR) != _CRTDBG_HFILE_ERROR);

  assert (_CrtSetReportMode (_CRT_ASSERT, reportMode) != -1);
  assert (_CrtSetReportFile (_CRT_ASSERT, _CRTDBG_FILE_STDERR) != _CRTDBG_HFILE_ERROR);
#endif

  _onexit (P32TestFini);
}
