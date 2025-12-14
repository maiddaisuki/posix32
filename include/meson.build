
##
# meson.build for posix32
#

p32_includes += include_directories('posix32')

p32_headers = files (
  'posix32/posix32-ext/p32_wchar-ext.h',
  'posix32/p32_ctype.h',
  'posix32/p32_langinfo.h',
  'posix32/p32_locale.h',
  'posix32/p32_stdlib.h',
  'posix32/p32_string.h',
  'posix32/p32_strings.h',
  'posix32/p32_uchar.h',
  'posix32/p32_wchar.h',
  'posix32/p32_wctype.h',
  'posix32/posix32.h',
  'posix32/posix32-decl.h',
)

install_headers (
  p32_headers,
  preserve_path: true,
)

subdir ('posix32/posix32-wrap')

unset_variable ('p32_headers')
