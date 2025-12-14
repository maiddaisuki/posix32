
##
# posix32 redirection library
#

##
# Generate posix32.def
#

posix32_def_in = files('posix32.def.in')[0]
posix32_def_data = configuration_data (
  {
    'posix32_crt': posix32_crt_libname,
  }
)
posix32_def = configure_file (
  input: posix32_def_in,
  output: 'posix32.def',
  configuration: posix32_def_data,
)

##
# Define posix32 target
#

posix32 = library (
  'posix32', 'posix32.c',
  install: true,

  include_directories: [
    p32_includes,
    p32_includes_private,
  ],
  c_args: [
    p32_args,
    p32_args_private,
  ],
  c_shared_args: [
    '-DLIBPOSIX32_DLL',
  ],

  vs_module_defs: posix32_def,
  version: meson.project_version(),

  dependencies: [
    posix32_crt_dep,
  ],
)
