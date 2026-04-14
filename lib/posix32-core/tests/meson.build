
##
# Tests for posix32-core library
#

posix32_core_test_args  = posix32_core_args
posix32_core_test_args += '-DLIBPOSIX32_TEST'

posix32_core_test = library (
  'posix32-core_test',
  posix32_core_sources, 'posix32-core_test.c',

  version: posix32_core_version,
  install: false,

  include_directories: [
    p32_includes_private,
  ],
  c_args: [
    p32_args,
    p32_args_private,
    posix32_core_test_args,
  ],
  c_shared_args: [
    '-DLIBPOSIX32_DLL',
  ],
  link_args: [
    p32_link_args,
    p32_link_args_private,
  ],
  dependencies: [
    p32_deps,
  ],
)

# Dependency object to link against `posix32-core_test`
posix32_core_test_dep = declare_dependency (
  include_directories: [
    p32_includes_private,
  ],
  compile_args: [
    p32_args,
    p32_args_private,
  ],
  link_args: [
    p32_link_args,
    p32_link_args_private,
  ],
  link_with: [
    posix32_core_test,
  ],
  dependencies: [
    p32_deps,
  ],
)

unset_variable ('posix32_core_test_args')
