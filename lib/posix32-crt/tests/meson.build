
##
# Tests for posix32-crt library
#

posix32_crt_test = library (
  'posix32-crt_test',
  posix32_crt_sources, 'posix32-crt_test.c',

  version: posix32_crt_version,
  install: false,

  include_directories: [
    p32_includes,
    p32_includes_private,
    posix32_crt_includes,
  ],
  c_args: [
    p32_args,
    p32_args_private,
    posix32_crt_args,
    '-DLIBPOSIX32_TEST',
  ],
  c_shared_args: [
    '-DLIBPOSIX32_DLL',
  ],
  dependencies: [
    p32_deps,
  ],
  vs_module_defs: posix32_crt_def,
)

# Dependency object to link against `posix32-crt_test`
posix32_crt_test_dep = declare_dependency (
  link_with: [
    posix32_crt_test,
  ],
  include_directories: [
    p32_includes,
    p32_includes_private,
    posix32_crt_includes,
    include_directories ('include'),
  ],
  compile_args: [
    p32_args,
    p32_args_private,
    p32_args_test,
    building_shared ? [] : ['-DLIBPOSIX32_STATIC'],
  ]
)

subdir('ctype')
subdir('locale')
subdir('string')
subdir('uchar')
subdir('wchar')
subdir('wctype')
subdir('wstring')
