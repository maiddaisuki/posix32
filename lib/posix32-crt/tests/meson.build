
##
# Tests for posix32-crt library
#

posix32_crt_test_includes  = posix32_crt_includes
posix32_crt_test_includes += include_directories ('include')

posix32_crt_test_args  = posix32_crt_args
posix32_crt_test_args += '-DLIBPOSIX32_TEST'

posix32_crt_test = library (
  'posix32-crt_test',
  posix32_crt_sources, 'posix32-crt_test.c',

  version: posix32_crt_version,
  install: false,

  include_directories: [
    posix32_crt_test_includes,
  ],
  c_args: [
    posix32_crt_test_args,
  ],
  c_shared_args: [
    '-DLIBPOSIX32_DLL',
  ],
  vs_module_defs: posix32_crt_def,
  dependencies: [
    posix32_core_test_dep,
  ],
)

# Dependency object to link against `posix32-crt_test`
posix32_crt_test_dep = declare_dependency (
  include_directories: [
    posix32_crt_test_includes,
  ],
  link_with: [
    posix32_crt_test,
  ],
  dependencies: [
    posix32_core_test_dep,
  ],
)

subdir('ctype')
subdir('locale')
subdir('string')
subdir('uchar')
subdir('wchar')
subdir('wctype')
subdir('wstring')

unset_variable ('posix32_crt_test_args')
unset_variable ('posix32_crt_test_includes')
