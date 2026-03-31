# Installing

This document describes how to build and install `posix32`.

Currently, `posix32` uses `meson` as its build system.  
In future, support for `cmake` and `autotools` will be added.

## Dependencies

The only dependency is [mingw-w64](https://www.mingw-w64.org/)'s `winpthreads`
library.

If you're building with MSVC, you also need native `tclsh` which is used during
configuration. You can install it using [Msys2](https://www.msys2.org/).

### MSVC

You need to build `winpthreads` library and create `winpthreads.pc` so it can
be found by the build system.

You can use
[libs-with-msvc.sh](https://github.com/maiddaisuki/gnu-libs-with-msvc)
to build `winpthreads` using either MSVC or `clang-cl.exe`.

Creating simple `winpthreads.pc` like this will suffice:

```txt
# --prefix used when building winpthreads, as native Windows path
prefix=PREFIX
includedir=${prefix}/include
libdir=${prefix}/lib

Name: winpthreads
Description: The Winpthreads Library
Version: 1.0
URL: https://www.mingw-w64.org/

Cflags: -I${includedir}
Cflags.private:
Libs: -L${libdir} -lwinpthread
Libs.private:
```

Make sure that `pkgconf` is able find it.

## Meson

Using `meson` is easy:

```shell
mkdir build
cd build
meson setup ../ --prefix PREFIX [OPTIONS]
meson compile
meson test
meson install [--strip]
```

## Configuration

The following table lists all supported options:

| Option      | Description                                               |
| ----------- | --------------------------------------------------------- |
| winpthreads | Use `pkgconf` to locate `winpthreads` even with mingw-w64 |
| winnt       | Minimal Windows NT version to support                     |
| ntddi       | Minimal `NTDDI` version to support (Windows 10 or later)  |
| uwp         | Configure for UWP                                         |
| lfs         | Default value of `_FILE_OFFSET_BITS`                      |
| y2k         | Default value of `_TIME_BITS`                             |
| tests       | Whether to compile and run tests                          |

### Testsuite

You can disable `tests` option if you do not plan to run `posix32`'s testsuite.

This will prevent all libraries from being built twice: once for regular version
and once for _test_ version used by testsuite.

| Build System | Syntax                |
| ------------ | --------------------- |
| Meson        | -Dtests={false\|true} |

### Using winpthreads

The build system uses `pkgconf` to locate `winpthreads` when building with MSVC.

You can enable this option if you want to locate `winpthreads` using `pkgconf`
even when using mingw-w64. This may be useful if want to use your own build of
`winpthreads` library.

| Build System | Syntax                                |
| ------------ | ------------------------------------- |
| Meson        | -Dwinpthreads={auto\|enable\|disable} |

### Windows Version

The library can be configured for Windows XP and any later version.

The `winnt` option accepts the following values:

| Value      | Description                        |
| ---------- | ---------------------------------- |
| `winxp`    | Configure for Windows XP           |
| `winvista` | Configure for Windows Vista        |
| `win7`     | Configure for Windows 7            |
| `win8`     | Configure for Windows 8            |
| `win8.1`   | Configure for Windows 8.1          |
| `win10`    | Configure for Windows 10 and later |

Default value is `win10`.

When this option is set to `win10`, you can use `ntddi` option to set minimal
`NTDDI` version to support.

Note that any combination of CRT and `winnt` is valid, although some
combination make little practical sense, such as `UCRT and winxp` or
`crtdll.dll and win10`.

| Build System | Syntax        |
| ------------ | ------------- |
| Meson        | -Dwinnt=VALUE |

#### NTDDI Version

When configuring for Windows 10 and later, the `ntddi` option can be used to
further customize minimum Windows version to support.

Default value is `0x0A000000` which corresponds to `NTDDI_WIN10`; that is,
the library will be usable on any device with Windows 10 and later.

This option can be set to any valid value that can be set for `NTDDI_VERSION`:

- You can set it to one of `NTDDI_WIN10_*` and `NTDDI_WIN11_*` constants defined
  in your toolchain's `sdkddkver.h`
- You can set it to `WDK_NTDDI_VERSION` to use the latest `NTDDI` version
  supported by the SDK

If this option is set to any value less than `NTDDI_WIN10`, it will result in
behavior as if it was set to `NTDDI_WIN10`. _Do not do this on purpose._

### \_FILE_OFFSET_BITS and \_TIME_BITS

The `lfs` and `y2k` options control default value for `_FILE_OFFSET_BITS` and
`_TIME_BITS` macros respectively.

When enabled, corresponding macro will be defined to `64` in `posix32.pc`.
When disabled, they will be defined to `32`.

These options are only meaningful for 32-bit builds. They cannot be disabled
for 64-bit builds.

Default value is `auto`.

| Build System | Syntax                               |
| ------------ | ------------------------------------ |
| Meson        | -D{lfs\|y2k}={auto\|enable\|disable} |

### UWP

This options allows to configure `posix32` for use with UWP Apps.

| Build System | Syntax              |
| ------------ | ------------------- |
| Meson        | -Duwp={true\|false} |

When you configure `posix32` for UWP, the follwing conditions must be met:

- CRT must be UCRT
- value of `winnt` option must be `win10`
- Building against static CRT is not allowed (as with MSVC's `/MT` switch)
- Building `posix32` as static library is not allowed

Building with debug version of CRT (`/MDd`) is allowed, but only for debugging
and testing reasons.

For details on how to compile code for UWP, see
[mingw-w64-libraries/winstorecompat/README.md](https://github.com/mingw-w64/mingw-w64/tree/master/mingw-w64-libraries/winstorecompat)
in mingw-w64's repository.
