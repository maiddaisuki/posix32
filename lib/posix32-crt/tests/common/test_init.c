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

static int P32TestFini (void) {
#ifdef _DEBUG
  _CrtCheckMemory ();
  _CrtDumpMemoryLeaks ();
#endif

  return EXIT_SUCCESS;
}

void p32_test_init (void) {
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
#ifndef LIBPOSIX32_UWP
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
  assert (_CrtSetReportMode (_CRT_WARN, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE) != -1);
  assert (_CrtSetReportFile (_CRT_WARN, _CRTDBG_FILE_STDOUT) != _CRTDBG_HFILE_ERROR);

  assert (_CrtSetReportMode (_CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE) != -1);
  assert (_CrtSetReportFile (_CRT_ERROR, _CRTDBG_FILE_STDERR) != _CRTDBG_HFILE_ERROR);

  assert (_CrtSetReportMode (_CRT_ASSERT, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_FILE) != -1);
  assert (_CrtSetReportFile (_CRT_ASSERT, _CRTDBG_FILE_STDERR) != _CRTDBG_HFILE_ERROR);
#endif

  _onexit (P32TestFini);
}
