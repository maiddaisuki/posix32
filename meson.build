
##
# meson.build for posix32
#

project (
  'posix32', 'c',

  version: files('VERSION')[0],
  meson_version: '>=1.3.0',

  default_options: {
    # Produce release builds by default
    'buildtype': 'release',
    # Disable assertions for release builds
    'b_ndebug':  'if-release',
    # We need C11 support, prefer GNU C if supported by the compiler
    'c_std':     'gnu11,c11',
  }
)

if host_machine.system() != 'windows'
  error ('posix32 is a library for native Windows platforms')
endif

cpu = host_machine.cpu_family()

# We could support Itanium (`ia64`) if there is a need; we would have to add
# corresponding `P32_ARCH_*` constant in `posix32.h` and modify arch-specific
# code.
if cpu not in ['x86', 'x86_64', 'arm', 'aarch64']
  error ('Arch `@0@` is not supported,'.format(cpu))
endif

# Import required modules
fs = import ('fs')

tclsh = find_program ('tclsh', native: true)
gperf = find_program ('gperf', native: true)

# Include directories
p32_includes         = [] # for external usage only
p32_includes_private = [] # for internal usage

# Compiler arguments
p32_args         = [] # for external and internal usage
p32_args_private = [] # for internal usage

# How to disable use of built-in functions
p32_args_no_builtin = []

# Linker arguments
p32_link_args         = [] # for external and internal usage
p32_link_args_private = [] # for internal usage

# Extra compiler and linker arguments to use with unit tests
p32_test_args      = ['-DLIBPOSIX32_TEST']
p32_test_link_args = []

# Dependencies common for all libraries
p32_deps = []

# Supported Windows NT versions
#
# For each supported `winnt` value it contains:
#
# - `P32_WINNT` value to use during compilation
# - `cpu`-specific major and minor subsystem versions
#
# If `windows_versions[winnt]` does not contain subsystem versions for `cpu`,
# then `winnt` does not support `cpu`.
#
# For example, this prevents configuring with `-Dwinnt=winxp` for arm;
# this makes no sense since arm processors are only supported since Windows 8.
#
windows_versions = {
  # Windows XP
  'winxp': {
    'args': [
      '-DP32_WINNT=0x0501', # _WIN32_WINNT_WINXP
    ],
    'x86': {
      'major': 5,
      'minor': 1,
    },
    'x86_64': {
      'major': 5,
      'minor': 2,
    },
  },
  # Windows Vista
  'winvista': {
    'args': [
      '-DP32_WINNT=0x0600', # _WIN32_WINNT_VISTA
    ],
    'x86': {
      'major': 6,
      'minor': 0,
    },
    'x86_64': {
      'major': 6,
      'minor': 0,
    },
  },
  # Windows 7
  'win7': {
    'args': [
      '-DP32_WINNT=0x0601', # _WIN32_WINNT_WIN7
    ],
    'x86': {
      'major': 6,
      'minor': 1,
    },
    'x86_64': {
      'major': 6,
      'minor': 1,
    },
  },
  # Windows 8
  'win8': {
    'args': [
      '-DP32_WINNT=0x0602', # _WIN32_WINNT_WIN8
    ],
    'x86': {
      'major': 6,
      'minor': 2,
    },
    'x86_64': {
      'major': 6,
      'minor': 2,
    },
    'arm': {
      'major': 6,
      'minor': 2,
    },
  },
  # Windows 8.1
  'win8.1': {
    'args': [
      '-DP32_WINNT=0x0603', # _WIN32_WINNT_WINBLUE
    ],
    'x86': {
      'major': 6,
      'minor': 3,
    },
    'x86_64': {
      'major': 6,
      'minor': 3,
    },
    'arm': {
      'major': 6,
      'minor': 3,
    },
  },
  # Windows 10
  'win10': {
    'args': [
      '-DP32_WINNT=0x0A00', # _WIN32_WINNT_WIN10
    ],
    'x86': {
      'major': 10,
      'minor': 0,
    },
    'x86_64': {
      'major': 10,
      'minor': 0,
    },
    'arm': {
      'major': 10,
      'minor': 0,
    },
    'aarch64': {
      'major': 10,
      'minor': 0,
    },
  },
}

cc = meson.get_compiler('c')

building_shared = get_option('default_library') in ['shared', 'both']
building_static = get_option('default_library') in ['static', 'both']

opt_uwp   = get_option('uwp')
opt_winnt = get_option('winnt')
opt_ntddi = get_option('ntddi')

# Check if `cpu` is supported with `opt_winnt`
if not windows_versions[opt_winnt].has_key(cpu)
  error ('`@0@` images cannot run on `@1@`; configure for higher Windows NT version.'.format(cpu, opt_winnt))
endif

is_mingw  = cc.has_define('__MINGW32__')
is_64bit  = cc.has_define('_WIN64')
is_ucrt   = cc.has_define('_UCRT', prefix: '#include <stdlib.h>')

if is_mingw
  # While mingw-w64 does not support static CRT, we need _DLL macro to be
  # properly defined
  is_dll = cc.has_define('_DLL',  prefix: '#include <stdlib.h>')
else
  is_dll = get_option('b_vscrt') in ['md', 'mdd', 'from_buildtype']
endif

if not is_dll and building_shared
  error ('Buidling posix32 as DLL with static CRT is not allowed')
endif

# Checks for UWP builds
if opt_uwp
  if not is_ucrt
    error('UWP: this configuraion is only supported when targeting UCRT')
  endif

  if not is_dll
    error('UWP: this configuraion is only supported with shared CRT')
  endif

  if building_static
    error('UWP: this configuration supports only shared libraries')
  endif

  if opt_winnt not in ['win10']
    error ('UWP: value of `winnt` option must be `win10`')
  endif

  # Find windowsapp library
  p32_deps += cc.find_library ('windowsapp', required: true)

  # With mingw-w64, we must link with libucrtapp.a instead of libucrt.a
  if is_mingw
    # gcc uses -mcrtdll= to link against specific CRT
    if cc.has_link_argument ('-mcrtdll=ucrtapp')
      ucrtapp_link_arg = '-mcrtdll=ucrtapp'
    # clang handles -l{crtdll|msvcr*|ucrt*} the same way as gcc's -mcrtdll=
    elif cc.has_link_argument ('-lucrtapp')
      ucrtapp_link_arg = '-lucrtapp'
    else
      error ('UWP: cannot link against libucrtapp.a')
    endif

    add_global_link_arguments (ucrtapp_link_arg, language: ['c'], native: false)
    unset_variable ('ucrtapp_link_arg')
  endif
endif

p32_args_private += ['-DHAVE_CONFIG_H', '-D_XOPEN_SOURCE=800',]
p32_args_private += windows_versions[opt_winnt]['args']

if opt_winnt in ['win10']
  p32_args_private += ['-DP32_NTDDI=@0@'.format(opt_ntddi)]
endif

if opt_uwp
  # FIXME: replace with WINAPI_FAMILY_PC_APP once fixed in mingw-w64
  p32_args_private += ['-DWINAPI_FAMILY=WINAPI_FAMILY_APP', '-DLIBPOSIX32_UWP']
else
  p32_args_private += ['-DWINAPI_FAMILY=WINAPI_FAMILY_DESKTOP_APP']
endif

if is_mingw
  p32_args_private += ['-D__USE_MINGW_ANSI_STDIO=0']
endif

if cc.get_argument_syntax() == 'msvc'
  # Avoid annoying warnings about *insecure* functions
  p32_args_private += ['-D_CRT_SECURE_NO_WARNINGS']

  # Tell compiler that source and execution charset is UTF-8
  if cc.has_argument ('/utf-8')
    p32_args_private += ['/utf-8']
  elif cc.has_multi_arguments('/execution-charset:.65001', '/source-charset:.65001')
    p32_args_private += ['/execution-charset:.65001', '/source-charset:.65001']
  else
    error ('No way to tell compiler that source ans execution charset is UTF-8')
  endif

  p32_args_private += cc.get_supported_arguments (
    '-Zc:__STDC__', # Request ISO C conformance
  )

  # Treat some warnings as errors
  p32_args_private += cc.get_supported_arguments (
    '/we4013', # Diagnose use of undeclared functions
  )

  p32_args_no_builtin += cc.get_supported_arguments (
    '/Oi-', # Disable built-in functions
  )

  # How to set subsystem version
  subsystem_link_args = '/SUBSYSTEM:CONSOLE,@0@.@1@'.format (
    windows_versions[opt_winnt][cpu]['major'],
    windows_versions[opt_winnt][cpu]['minor']
  )

  if cc.has_link_argument (subsystem_link_args)
    p32_link_args_private += subsystem_link_args
    unset_variable ('subsystem_link_args')
  else
    error ('Toolchain cannot create images for `@0@`.'.format(opt_winnt))
  endif
elif cc.get_argument_syntax() == 'gcc'
  # Tell compiler that source and execution charset is UTF-8
  if cc.has_multi_arguments('-finput-charset=utf-8', '-fexec-charset=utf-8')
    p32_args_private += ['-finput-charset=utf-8', '-fexec-charset=utf-8']
  else
    error ('No way to tell compiler that source ans execution charset is UTF-8')
  endif

  # Treat some warnings as errors
  p32_args_private += cc.get_supported_arguments (
    '-Werror=invalid-utf8',
  )

  p32_args_no_builtin += cc.get_supported_arguments (
    '-fno-builtin',
  )

  # FIXME: mingw-w64 toolchains produce broken executables when subsystem
  #   version is set to 6.3 (Windows 8.1) or later. Until it is fixed, we set
  #   subsystem version to 6.2 when configuting for Windows 8.1 or later.
  #
  # See <https://sourceforge.net/p/mingw-w64/mailman/message/59315202/>
  #
  major = windows_versions[opt_winnt][cpu]['major']
  minor = windows_versions[opt_winnt][cpu]['minor']

  if major > 6 or (major == 6 and minor > 2)
    major = 6
    minor = 2
  endif

  # How to set subsystem version
  subsystem_link_args = '-Wl,--subsystem,console:@0@.@1@'.format (
    major,
    minor
  )

  unset_variable ('minor')
  unset_variable ('major')

  if cc.has_link_argument (subsystem_link_args)
    p32_link_args_private += subsystem_link_args
    unset_variable ('subsystem_link_args')
  else
    error ('Toolchain cannot create images for `@0@`.'.format(opt_winnt))
  endif
else
  error ('Unsupported compiler. Please open an issue and provide information about you compiler.')
endif

# When building only static libraries, we need to ensure that when we are
# compiling unit tests, functions are not declared with
# `__declspec (dllimport)`, which is default.
#
# `declare_dependency` does not have equivalents of `*_shared_args` and
# `*_static_args`, so we have to pass `-DLIBPOSIX32_STATIC` in `compile_args`.
#
if not building_shared
  p32_test_args += '-DLIBPOSIX32_STATIC'
endif

# Disable use of built-in functions when compiling unit tests
p32_test_args += p32_args_no_builtin

# Find POSIX threads
opt_winpthreads = get_option('winpthreads').enable_if(not is_mingw)

if opt_winpthreads.enabled()
  threads = dependency('winpthreads', required: true, static: not building_shared)
else
  threads = dependency('threads', required: true)
endif

p32_deps += threads

# Figure out values for _FILE_OFFSET_BITS and _TIME_BITS
opt_lfs = get_option('lfs').enable_if(is_64bit)
opt_y2k = get_option('y2k').enable_if(is_64bit)

sizeof_time_t = cc.sizeof('time_t', prefix: '#include <time.h>')

# Figure out how to use 64-bit time_t
if sizeof_time_t == 4
  if is_mingw
    # Try using _TIME_BITS=64
    set_variable ('time_bits64_args', '-D_TIME_BITS=64')

    _sizeof_time_t = cc.sizeof (
      'time_t',
      prefix: ['#include <time.h>'],
      args:   [time_bits64_args],
    )

    # No need to set `time_bits64_args`
    if _sizeof_time_t == 8
      unset_variable ('time_bits64_args')
    endif

    if _sizeof_time_t != 8
      # Try using __MINGW_USE_VC2005_COMPAT
      set_variable ('time_bits64_args', '-D__MINGW_USE_VC2005_COMPAT')

      _sizeof_time_t = cc.sizeof (
        'time_t',
        prefix: ['#include <time.h>'],
        args:   [time_bits64_args]
      )
    endif

    if _sizeof_time_t != 8
      error('Do not know how to use 64-bit time_t')
    endif

    unset_variable ('_sizeof_time_t')
  endif
endif

# Figure out how to use 32-bit time_t
if not is_64bit and sizeof_time_t == 8
  set_variable ('_sizeof_time_t', sizeof_time_t)

  if is_mingw
    # Try using _TIME_BITS=32
    set_variable ('time_bits32_args', '-D_TIME_BITS=32')

    _sizeof_time_t = cc.sizeof (
      'time_t',
      prefix: ['#include <time.h>'],
      args:   [time_bits32_args]
    )

    # No need to set `time_bits32_args`
    if _sizeof_time_t == 4
      unset_variable ('time_bits32_args')
    endif
  endif

  if _sizeof_time_t != 4
    # Try using _USE_32BIT_TIME_T
    set_variable ('time_bits32_args', '-D_USE_32BIT_TIME_T')

    _sizeof_time_t = cc.sizeof (
      'time_t',
      prefix: ['#include <time.h>'],
      args:   [time_bits32_args]
    )
  endif

  if _sizeof_time_t != 4
    error('Do not know how to use 32-bit time_t')
  endif

  unset_variable ('_sizeof_time_t')
endif

if opt_lfs.enable_auto_if(sizeof_time_t == 8).enabled()
  p32_args += ['-D_FILE_OFFSET_BITS=64']
else
  p32_args += ['-U_FILE_OFFSET_BITS', '-D_FILE_OFFSET_BITS=32']
endif

if opt_y2k.enable_auto_if(sizeof_time_t == 8).enabled()
  p32_args += ['-D_TIME_BITS=64']

  if is_variable ('time_bits64_args')
    p32_args += time_bits64_args
    unset_variable ('time_bits64_args')
  endif
else
  p32_args += ['-D_TIME_BITS=32']

  if is_variable ('time_bits32_args')
    p32_args += time_bits32_args
    unset_variable ('time_bits32_args')
  endif
endif

# Check for working crtdbg.h
# We require that crtdbg.h is present, but we do not need it to be functional
cc.has_header('crtdbg.h', required: true)

crtdbg_testrun = cc.run (
  '''
    #ifndef _DEBUG
    #define _DEBUG
    #endif

    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>

    static int ReportHook (int reportType, char *msg, int *returnValue) {
      _exit (0);
    }

    int main (void) {
      _CrtSetReportMode (_CRT_ERROR, _CRTDBG_MODE_FILE);
      _CrtSetReportFile (_CRT_ERROR, _CRTDBG_FILE_STDERR);
      _CrtSetReportHook (ReportHook);
      void *mem = malloc (1);
      free (mem);
      _RPTW0 (_CRT_ERROR, L"");
      return 1;
    }
  ''',
  name: 'crtdbg.h with _DEBUG'
)

have_crtdbg_h = crtdbg_testrun.compiled() and crtdbg_testrun.returncode() == 0
unset_variable ('crtdbg_testrun')

# Subdirectories

subdir('include')
subdir('lib')

# Custom Targets

run_target (
  'gperf',
  command: [
    tclsh, '@SOURCE_ROOT@/scripts/gperf.tcl', gperf.full_path(), '@SOURCE_ROOT@'
  ],
  env: {
    'LC_ALL': 'en_US.UTF-8'
  }
)
