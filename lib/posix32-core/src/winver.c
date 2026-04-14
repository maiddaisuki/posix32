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
#include <pthread.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "core-runtime.h"
#include "core-winver.h"

/**
 * File Summary:
 *
 * This file defines functions to obtain information about the platform the
 * code is running on.
 *
 * These functions allow to have conditional code to check Windows version
 * at runtime, compared to conditional code which uses preprocessor conditions
 * involving `_WIN32_WINNT`.
 */

/**
 * Suppress warnings about conversion between data and function pointers with
 * picky compilers.
 */
#define F(ptr) (PVOID) (UINT_PTR) ptr

/**
 * Convenience wrapper for `InterlockedExchangePointer`.
 */
#define P32AtomicExchange(target, source) InterlockedExchangePointer ((void *volatile *) target, F (source))

/**
 * Function type corresponding to `p32_platform`.
 */
typedef WindowsPlatform (*FuncPlatform) (void);

/**
 * Initialization thunk for `p32_platform`.
 */
static WindowsPlatform P32InitPlatform (void);

/**
 * Function type corresponding to `p32_winnt`.
 */
typedef WindowsNtVersion (*FuncVersionNt) (void);

/**
 * Initialization thunk for `p32_winnt`.
 */
static WindowsNtVersion P32InitVersionNt (void);

/**
 * Function type corresponding to `p32_win9x`.
 */
typedef Windows9xVersion (*FuncVersion9x) (void);

/**
 * Initialization thunk for `p32_win9x`.
 */
static Windows9xVersion P32InitVersion9x (void);

/**
 * Structure to store information about the platform.
 */
typedef struct PlatformInfo {
  pthread_once_t Init;
  /**
   * Platform the code is running on:
   *
   * Windows NT: `WindowsPlatformNt`
   * Windows 9x: `WindowsPlatform9x`
   */
  WindowsPlatform Paltform;
  /**
   * Windows version the code is running on:
   *
   * If `Platform` is `WindowsPlatformNt`, then `VersionNt` is set to one of
   * `WindowsNtVersion` values.
   *
   * If `Platform` is `WindowsPlatform9x`, then `Version9x` is set to one of
   * `Windows9xVersion` values.
   */
  union {
    WindowsNtVersion VersionNt;
    Windows9xVersion Version9x;
  };
  /**
   * Implementation for `p32_platform`.
   */
  FuncPlatform PtrPlatform;
  /**
   * Implementation for `p32_winnt`.
   */
  FuncVersionNt PtrVersionNt;
  /**
   * Implementation for `p32_win9x`.
   */
  FuncVersion9x PtrVersion9x;
} PlatformInfo;

/**
 * Information about the platform the code is running on.
 */
static PlatformInfo P32PlatformInfo = {
  .Init         = PTHREAD_ONCE_INIT,
  .Paltform     = WindowsPlatformInvalid,
  .VersionNt    = WindowsNtInvalid,
  .PtrPlatform  = P32InitPlatform,
  .PtrVersionNt = P32InitVersionNt,
  .PtrVersion9x = P32InitVersion9x,
};

/**
 * Implementation for `p32_platform`.
 */
static WindowsPlatform P32Platform (void) {
  assert (P32PlatformInfo.Paltform != WindowsPlatformInvalid);
  return P32PlatformInfo.Paltform;
}

/**
 * Implementation for `p32_winnt`.
 *
 * Returns `WindowsNtVersion` corresponding to Windows NT version the code
 * is running on.
 *
 * This function must only be called when code is running on Windows NT.
 */
static WindowsNtVersion P32VersionNt (void) {
  assert (P32PlatformInfo.Paltform == WindowsPlatformNt);
  return P32PlatformInfo.VersionNt;
}

/**
 * Implementation for `p32_win9x`.
 *
 * Returns `Windows9xVersion` corresponding to Windows 9X version the code
 * is running on.
 *
 * This function must only be called when code is running on Windows 9x.
 */
static Windows9xVersion P32Version9x (void) {
  assert (P32PlatformInfo.Paltform == WindowsPlatform9x);
  return P32PlatformInfo.Version9x;
}

/**
 * Implementation for `p32_winnt`.
 *
 * Always returns `WindowsNtInvalid` to indicate that the code is running on
 * Windows 9x.
 *
 * This function must only be called when code is running on Windows 9x.
 */
static WindowsNtVersion P32InvalidVersionNt (void) {
  assert (P32PlatformInfo.Paltform == WindowsPlatform9x);
  return WindowsNtInvalid;
}

/**
 * Implementation for `p32_win9x`.
 *
 * Always returns `Windows9xInvalid` to indicate that the code is running on
 * Windows NT.
 *
 * This function must only be called when code is running on Windows NT.
 */
static Windows9xVersion P32InvalidVersion9x (void) {
  assert (P32PlatformInfo.Paltform == WindowsPlatformNt);
  return Windows9xInvalid;
}

/**
 * Initialalize `P32PlatformInfo`.
 */
static void P32Init (void) {
  /**
   * Function `GetVersion` is available on all Windows NT and Windows 9x
   * systems, and provides all information that we need.
   *
   * However, `GetVersion` is not available to UWP Application;
   * for this reason we use `GetVersionEx` in UWP builds.
   */
#ifdef LIBPOSIX32_UWP
  /**
   * There is no extra information in `OSVERSIONINFOEXW` that we need,
   * so simply use `OSVERSIONINFOW` which is slightly smaller.
   */
  OSVERSIONINFOW osVersionInfo      = {0};
  osVersionInfo.dwOSVersionInfoSize = sizeof (osVersionInfo);

  /**
   * Microsoft headers deprecate `GetVersionExW` function suggesting to use
   * helper functions declared in `VersionHelpers.h`; those function are not
   * available to UWP Applications, so we cannot use them here.
   */
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996) // Use of deprecated APIs
#endif

  if (!GetVersionExW (&osVersionInfo)) {
    goto fail;
  }

#ifdef _MSC_VER
#pragma warning(pop)
#endif

  assert (osVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT);

  if (osVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) {
    P32PlatformInfo.Paltform = WindowsPlatformNt;
  } else {
    goto unsupported_platform;
  }

  if (osVersionInfo.dwMajorVersion > 10) {
    P32PlatformInfo.VersionNt = WindowsNtLatest;
  } else if (osVersionInfo.dwMajorVersion == 10) {
    if (osVersionInfo.dwMinorVersion > 0) {
      P32PlatformInfo.VersionNt = WindowsNtLatest;
    } else {
      P32PlatformInfo.VersionNt = WindowsNt10;
    }
#ifdef LIBPOSIX32_TEST
    /**
     * Applications which do not explicitly target Windows 8.1 or later in their
     * manifest return version information as if code is running on Windows 8.
     *
     * See <https://learn.microsoft.com/en-us/windows/win32/sysinfo/targeting-your-application-at-windows-8-1>
     *
     * UWP Applications can run only on Windows 10 and later, so assume
     * Windows 10 for testing reasons.
     */
  } else if (osVersionInfo.dwMajorVersion == 6 && osVersionInfo.dwMinorVersion >= 2) {
    P32PlatformInfo.VersionNt = WindowsNt10;
#endif
  } else {
    goto fail;
  }
#else
  /**
   * Returned `DWORD` value contains information as described below.
   *
   * High order word:
   *
   *  Contains information about operating system platform.
   *
   *  High order byte:
   *
   *    The high bit is set for DOS-based Windows platforms, that is,
   *    Win9x and Win32s. If this bit is clean, then the code is running
   *    on Windows NT platform.
   *
   *    Remaining 7 bits have different meaning depending on platform:
   *
   *      Windows NT: build version
   *      Windows 9x: reserved
   *
   *  Low order byte:
   *
   *    Micorosft documentation is unclean about contents of this byte;
   *    recent versions of documentation claim it to be set to value 5.
   *
   *    This is not true for recent Windows NT versions.
   *
   *    Tests have shown that on recent Windows NT versions,
   *    high word as whole contains the build number.
   *
   *    The contents of this byte on Windows 9x remain unclean.
   *
   * Low order word:
   *
   *  Contains information about operating system version.
   *
   *  High order byte: Minor version number.
   *
   *  Low order byte:  Major version number.
   */
  DWORD version = GetVersion ();

  BYTE majorVersion = LOBYTE (LOWORD (version));
  BYTE minorVersion = HIBYTE (LOWORD (version));
  WORD buildNumber  = HIWORD (version);

  if (buildNumber & 0x8000) {
    P32PlatformInfo.Paltform = WindowsPlatform9x;
  } else {
    P32PlatformInfo.Paltform = WindowsPlatformNt;
  }

  if (P32PlatformInfo.Paltform == WindowsPlatformNt) {
    assert (majorVersion >= 3);

    if (majorVersion > 10) {
      P32PlatformInfo.VersionNt = WindowsNtLatest;
    } else if (majorVersion == 10) {
      if (minorVersion > 0) {
        P32PlatformInfo.VersionNt = WindowsNtLatest;
      } else {
        P32PlatformInfo.VersionNt = WindowsNt10;
      }
    } else if (majorVersion == 6) {
      assert (minorVersion <= 3);

      if (minorVersion == 3) {
        P32PlatformInfo.VersionNt = WindowsNt8_1;
      } else if (minorVersion == 2) {
        P32PlatformInfo.VersionNt = WindowsNt8;
      } else if (minorVersion == 1) {
        P32PlatformInfo.VersionNt = WindowsNt7;
      } else if (minorVersion == 0) {
        P32PlatformInfo.VersionNt = WindowsNtVista;
      } else {
        goto fail;
      }
    } else if (majorVersion == 5) {
      assert (minorVersion <= 2);

      if (minorVersion == 2 || minorVersion == 1) {
        P32PlatformInfo.VersionNt = WindowsNtXP;
      } else if (minorVersion == 0) {
        P32PlatformInfo.VersionNt = WindowsNt2000;
      } else {
        goto fail;
      }
    } else if (majorVersion == 4) {
      assert (minorVersion == 0);

      if (minorVersion == 0) {
        P32PlatformInfo.VersionNt = WindowsNt4;
      } else {
        goto fail;
      }
    } else if (majorVersion == 3) {
      if (minorVersion == 51) {
        P32PlatformInfo.VersionNt = WindowsNt3_51;
      } else if (minorVersion == 50) {
        P32PlatformInfo.VersionNt = WindowsNt3_5;
      } else if (minorVersion == 10) {
        P32PlatformInfo.VersionNt = WindowsNt3_1;
      } else {
        goto fail;
      }
    } else {
      goto fail;
    }
  } else if (P32_WIN9X && P32PlatformInfo.Paltform == WindowsPlatform9x) {
    assert (majorVersion >= 3 && majorVersion <= 4);

    /**
     * All Windows 9x systems have major version 4.
     */
    if (majorVersion == 4) {
      if (minorVersion == 90) {
        P32PlatformInfo.Version9x = WindowsMe;
      } else if (minorVersion == 10) {
        P32PlatformInfo.Version9x = Windows98;
      } else if (minorVersion == 0) {
        P32PlatformInfo.Version9x = Windows95;
      } else {
        goto fail;
      }
    } else if (majorVersion == 3) {
      /**
       * Windows version 3.11 indicates Win32s.
       */
      if (minorVersion == 11) {
        goto win32s;
      } else {
        goto fail;
      }
    } else {
      goto fail;
    }
  } else {
    goto unsupported_platform;
  }
#endif

  P32AtomicExchange (&P32PlatformInfo.PtrPlatform, P32Platform);

  if (P32PlatformInfo.Paltform == WindowsPlatformNt) {
    P32AtomicExchange (&P32PlatformInfo.PtrVersionNt, P32VersionNt);
    P32AtomicExchange (&P32PlatformInfo.PtrVersion9x, P32InvalidVersion9x);
  } else {
    P32AtomicExchange (&P32PlatformInfo.PtrVersionNt, P32InvalidVersionNt);
    P32AtomicExchange (&P32PlatformInfo.PtrVersion9x, P32Version9x);
  }

  return;

fail:
  p32_terminate (L"Failed to obtain information about Windows version.");

unsupported_platform:
  p32_terminate (L"Unsupported platform.");

win32s:
  p32_terminate (L"Win32s is not supported.");
}

static WindowsPlatform P32InitPlatform (void) {
  pthread_once (&P32PlatformInfo.Init, P32Init);
  return P32PlatformInfo.PtrPlatform ();
}

static WindowsNtVersion P32InitVersionNt (void) {
  pthread_once (&P32PlatformInfo.Init, P32Init);
  return P32PlatformInfo.PtrVersionNt ();
}

static Windows9xVersion P32InitVersion9x (void) {
  pthread_once (&P32PlatformInfo.Init, P32Init);
  return P32PlatformInfo.PtrVersion9x ();
}

WindowsPlatform p32_platform (void) {
  return P32PlatformInfo.PtrPlatform ();
}

WindowsNtVersion p32_winnt (void) {
  return P32PlatformInfo.PtrVersionNt ();
}

Windows9xVersion p32_win9x (void) {
  return P32PlatformInfo.PtrVersion9x ();
}
