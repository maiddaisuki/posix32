# POSIX API Library for Win32

The `posix32` is a work-in-progress set of libraries which implement POSIX
functions for Windows NT operating system. It also provides better conforming
replacements for CRT functions.

The libraries can be built using either MSVC or mingw-w64 toolchain.
UCRT is supported with both; in order to build for older CRTs such as
`msvcrt.dll`, use mingw-w64.

Original 32-bit mingw toolchain is not supported (not to be confused with
32-bit mingw-w64 toolchain).

For build instructions and configuration details see [INSTALL.md](/INSTALL.md).

## Using the Library

Once the library is built and installed, you can use `pkgconf` to link against
`posix32`:

```shell
PKG_CONFIG_PATH=PREFIX/lib/pkgconfig
export PKG_CONFIG_PATH
gcc ... `pkgconf --cflags --libs posix32`
```

No changes to source code are required to use the library.

### Autotools

If you want to add dependency on `posix32`, it is recommended to use `PKG_*`
Autotools macros from `pkg.m4` to locale and use the library.

You may also try to transparently use `posix32` with:

```shell
path/to/configure                         \
  CFLAGS="... `pkgconf --cflags posix32`" \
  LIBS="... `pkgconf --libs posix32`"     \
  ...
```

### Meson

```meson
  posix32 = dependency ('posix32')
```

### CMake

Use cmake's `FindPkgConfig` module to locate `posix32` using `pkgconf`.

In future, cmake will be added as a build system for `posix32`, and it will be
possible to use cmake's `find_package` command to locale and use `posix32`.
