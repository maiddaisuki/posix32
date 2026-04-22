
##
# posix32 redirection library
#

##
# Generate posix32.def
#

posix32_def_in = files('posix32.def.in')[0]
posix32_def_data = configuration_data (
  {
    'posix32_crt': posix32_crt_libname.replace('.dll', ''),
  }
)
posix32_def_in = configure_file (
  input: posix32_def_in,
  # FIXME: clang-cl refuses to preprocess file named `posix32.def.in` so use
  #   `posix32.def.c` instead.
  output: 'posix32.def.c',
  configuration: posix32_def_data,
)
posix32_def = cc.preprocess (
  posix32_def_in,
  output: 'posix32.def',

  compile_args: [
    p32_args,
    p32_args_private,
    (is_mingw and is_ucrt) ? ['-D_UCRT'] : [],
  ],
  include_directories: [
    p32_includes_private,
  ],
)[0]

##
# Define posix32 target
#

posix32 = library (
  'posix32', 'posix32.c',

  version: meson.project_version(),
  install: true,

  include_directories: [
    p32_includes_private,
  ],
  c_args: [
    p32_args,
    p32_args_private,
  ],
  c_shared_args: [
    '-DLIBPOSIX32_DLL',
  ],
  link_args: [
    p32_link_args,
    p32_link_args_private,
  ],
  vs_module_defs: posix32_def,
  dependencies: [
    posix32_crt_dep,
  ],
)

##
# Declare `posix32_dep` dependency object
#

posix32_dep = declare_dependency (
  version: meson.project_version(),

  include_directories: [
    p32_includes,
  ],
  compile_args: [
    p32_args,
  ],
  link_with: [
    posix32,
  ],
  dependencies: [
    posix32_crt_dep,
  ],
)

unset_variable ('posix32_def')
unset_variable ('posix32_def_data')
unset_variable ('posix32_def_in')
