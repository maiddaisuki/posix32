
##
# meson.build for posix32
#

p32_includes_private += include_directories('.')

##
# Generate posix32.pc
#

posix32_Cflags         = []
posix32_Cflags_private = []
posix32_Libs           = []
posix32_Libs_private   = []
posix32_Requires       = []

posix32_Cflags += [p32_args]

if building_static
  posix32_Cflags_private += ['-DLIBPOSIX32_STATIC']
else
  posix32_Cflags_private += ['-DLIBPOSIX32_NO_STATIC']
endif

posix32_Libs         += ['-lposix32']
posix32_Libs_private += ['-lposix32-crt']

if opt_winpthreads.enabled()
  posix32_Requires     += ['winpthreads']
else
  posix32_Libs_private += ['-lpthread']
endif

posix32_Cflags         = ' '.join(posix32_Cflags)
posix32_Cflags_private = ' '.join(posix32_Cflags_private)
posix32_Libs           = ' '.join(posix32_Libs)
posix32_Libs_private   = ' '.join(posix32_Libs_private)
posix32_Requires       = ' '.join(posix32_Requires)

posix32_pc_in = files ('posix32.pc.in')
posix32_pc_data = configuration_data (
  {
    'prefix': get_option ('prefix'),
    'includedir': get_option ('prefix') / get_option ('includedir'),
    'libdir': get_option ('prefix') / get_option ('libdir'),
    'version': meson.project_version(),
    'posix32_Cflags': posix32_Cflags,
    'posix32_Cflags_private': posix32_Cflags_private,
    'posix32_Libs': posix32_Libs,
    'posix32_Libs_private': posix32_Libs_private,
    'posix32_Requires': posix32_Requires,
  }
)
posix32_pc = configure_file (
  input: posix32_pc_in,
  output: 'posix32.pc',
  configuration: posix32_pc_data,
  install: true,
  install_dir: get_option ('prefix') / get_option ('libdir') / 'pkgconfig'
)

unset_variable ('posix32_Cflags')
unset_variable ('posix32_Cflags_private')
unset_variable ('posix32_Libs')
unset_variable ('posix32_Libs_private')
unset_variable ('posix32_Requires')

##
# Generate config.h
#

config_h_in = files('config-cmake.h.in')[0]
config_h_data = configuration_data (
  {
    'HAVE_CRTDBG_H': have_crtdbg_h
  }
)
config_h = configure_file (
  input: config_h_in,
  output: 'config.h',
  format: 'cmake',
  configuration: config_h_data
)

##
# Modules
#

subdir('posix32-crt')
subdir('posix32')

##
# Declare `posix32_dep` dependency object
#

posix32_dep = declare_dependency (
  version: meson.project_version(),

  include_directories: p32_includes,
  compile_args: p32_args,
  link_with: [
    posix32,
  ],
)
