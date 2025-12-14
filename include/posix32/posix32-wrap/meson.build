
##
# meson.build for posix32
#

find_header = meson.project_source_root() / 'scripts/find-header.tcl'

headers = [
  'ctype',
  'locale',
  'stdlib',
  'string',
  'uchar',
  'wchar',
  'wctype'
]

extra_headers = [
  'langinfo',
  'strings',
]

headers += extra_headers

foreach header : headers
  if is_mingw
    spec = ['#include_next <@0@.h>', header]
  elif header in extra_headers
    spec = ['#include "p32_@0@.h"', header]
  else
    res = run_command (
      tclsh, find_header, '@0@.h'.format(header),
      check:   true,
      capture: true,
    )

    spec = ['#include "@0@"', res.stdout().strip()]
    unset_variable ('res')
  endif

  data = configuration_data (
    {
      'include': spec[0].format(spec[1])
    }
  )

  configure_file (
    input:  '@0@.in.h'.format(header),
    output: '@0@.h'.format(header),
    configuration: data,
    install: true,
    install_dir: get_option('includedir') / 'posix32' / 'posix32-wrap'
  )

  unset_variable ('data')
  unset_variable ('spec')
endforeach

unset_variable('extra_headers')
unset_variable('headers')
unset_variable('find_header')
