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

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef LIBPOSIX32_DLL
#define _DECL_DLLMAIN
#include <process.h>
#endif

#ifdef LIBPOSIX32_DLL
BOOL APIENTRY DllMain (HANDLE Module, DWORD Reason, LPVOID Reserved) {
  switch (Reason) {
    /**
     * DLL is loaded.
     */
    case DLL_PROCESS_ATTACH: {
      break;
    }
    /**
     * New thread is created.
     */
    case DLL_THREAD_ATTACH: {
      break;
    }
    /**
     * Thread terminates
     */
    case DLL_THREAD_DETACH: {
      break;
    }
    /**
     * DLL is unloaded
     */
    case DLL_PROCESS_DETACH: {
      break;
    }
  }

  return TRUE;
  UNREFERENCED_PARAMETER (Module);
  UNREFERENCED_PARAMETER (Reserved);
}
#else
extern int posix32_redirect_library;
int        posix32_redirect_library = 1;
#endif
