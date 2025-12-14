
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

# Import required modules
fs = import ('fs')

tclsh = find_program ('tclsh', native: true)
gperf = find_program ('gperf', native: true)

# Following include directories are common for all libraries
p32_includes = []
p32_includes_private = []

# Following arguments are common for all libraries
p32_args = []
p32_args_private = []

# Extra arguments to use with unit tests
p32_args_test = ['-DLIBPOSIX32_TEST']

# Dependencies common for all libraries
p32_deps = []

# Macros to define based on value of `winver` option
windows_versions = {
  # _WIN32_WINNT_WINXP
  'winxp': 	  ['-D_WIN32_WINNT=0x0501'],
  # _WIN32_WINNT_VISTA
  'winvista': ['-D_WIN32_WINNT=0x0600'],
  # _WIN32_WINNT_WIN7
  'win7':     ['-D_WIN32_WINNT=0x0601'],
  # _WIN32_WINNT_WIN8
  'win8':     ['-D_WIN32_WINNT=0x0602'],
  # _WIN32_WINNT_WINBLUE
  'win8.1':   ['-D_WIN32_WINNT=0x0603'],
  # _WIN32_WINNT_WIN10
  'win10':    ['-D_WIN32_WINNT=0x0A00'],
  # _WIN32_WINNT_WIN10 + LIBPOSIX32_WIN10PLUS
  'win10+':   ['-D_WIN32_WINNT=0x0A00', '-DLIBPOSIX32_WIN10PLUS'],
}

cc = meson.get_compiler('c')

building_shared = get_option('default_library') in ['shared', 'both']
building_static = get_option('default_library') in ['static', 'both']

opt_uwp    = get_option('uwp')
opt_winver = get_option('winver')

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

  if opt_winver not in ['win10', 'win10+']
    error ('UWP: value of -Dwinver option must be win10 or win10+')
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
p32_args_private += windows_versions[opt_winver]

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

  p32_args_test += cc.get_supported_arguments (
    '/Oi-', # Disable built-in functions
  )
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

  p32_args_test += cc.get_supported_arguments (
    '-fno-builtin',
  )
else
  error ('Unsupported compiler. Please open an issue and provide information about you compiler.')
endif

# Find POSIX threads
opt_winpthreads = get_option('winpthreads').enable_if(not is_mingw)

if opt_winpthreads.enabled()
  threads = dependency('winpthreads', required: true)
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
    set_variable ('time_bits32_args', '-D_TIME_BITS')

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
