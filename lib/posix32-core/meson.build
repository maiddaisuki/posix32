
##
# posix32-core library
#

p32_includes_private += include_directories('include')
posix32_core_args     = ['-DP32_BUILD', '-DP32_CORE_BUILD']

posix32_core_sources = files (
  'src/crt.c',
  'src/heap.c',
  'src/runtime.c',
  'src/winver.c',
)

##
# Define `posix32-core` target
#

posix32_core_version = fs.read('VERSION').strip()

posix32_core = library (
  'posix32-core',
  posix32_core_sources,

  version: posix32_core_version,
  install: true,

  include_directories: [
    p32_includes_private,
  ],
  c_args: [
    p32_args,
    p32_args_private,
    posix32_core_args,
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

posix32_core_libname = fs.name(posix32_core.full_path())

# Dependency object to link against `posix32-core`
posix32_core_dep = declare_dependency (
  link_with: [
    posix32_core
  ],
  dependencies: [
    p32_deps,
  ],
)

if get_option('tests')
  subdir('tests')
endif

unset_variable ('posix32_core_sources')
unset_variable ('posix32_core_args')
