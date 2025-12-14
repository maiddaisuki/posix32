
##
# posix32-crt library
#

posix32_crt_includes = include_directories('include')
posix32_crt_args = ['-DP32_BUILD', '-DP32_CRT_BUILD']

posix32_crt_sources = files (
  # gperf-generated
  'gperf/country_codes.c',
  'gperf/country_names.c',
  'gperf/charset.c',
  'gperf/modifier.c',
  'gperf/language_codes.c',
  'gperf/language_names.c',
  'gperf/script.c',
  'gperf/sorting.c',
  'gperf/sorting_id.c',
  'gperf/sublanguage.c',
  # Source files
  'src/ctype/ctype.c',
  'src/locale/locale_map/base_language.c',
  'src/locale/locale_map/country.c',
  'src/locale/locale_map/language.c',
  'src/locale/locale_map/modifier.c',
  'src/locale/locale_map/script.c',
  'src/locale/locale_map/sorting.c',
  'src/locale/locale_map/sorting_id.c',
  'src/locale/locale_map/sublanguage.c',
  'src/locale/alt_digits.c',
  'src/locale/langinfo.c',
  'src/locale/lc_collate.c',
  'src/locale/lc_ctype.c',
  'src/locale/lc_messages.c',
  'src/locale/lc_monetary.c',
  'src/locale/lc_numeric.c',
  'src/locale/lc_time.c',
  'src/locale/locale.c',
  'src/locale/locale_charset.c',
  'src/locale/locale_map.c',
  'src/locale/locale_string.c',
  'src/locale/locale_win32.c',
  'src/locale/misc.c',
  'src/string/string.c',
  'src/uchar/ascii.c',
  'src/uchar/dbcs.c',
  'src/uchar/posix.c',
  'src/uchar/sbcs.c',
  'src/uchar/uchar.c',
  'src/uchar/utf8.c',
  'src/wchar/c89.c',
  'src/wchar/c95.c',
  'src/wchar/posix32.c',
  'src/wctype/wctrans.c',
  'src/wctype/wctype.c',
  'src/wstring/wstring.c',
  'src/runtime.c',
  'src/tls.c',
)

##
# Generate posix32-crt.def
#

posix32_crt_def_in = files('posix32-crt.def.in')[0]
posix32_crt_def = configure_file (
  input: posix32_crt_def_in,
  output: 'posix32-crt.def',
  copy: true
)

##
# Define `posix32-crt` target
#

posix32_crt_version = fs.read('VERSION').strip()

posix32_crt = library (
  'posix32-crt',
  posix32_crt_sources,

  version: posix32_crt_version,
  install: true,

  include_directories: [
    p32_includes,
    p32_includes_private,
    posix32_crt_includes,
  ],
  c_args: [
    p32_args,
    p32_args_private,
    posix32_crt_args,
  ],
  c_shared_args: [
    '-DLIBPOSIX32_DLL',
  ],
  dependencies: [
    p32_deps,
  ],
  vs_module_defs: posix32_crt_def,
)

posix32_crt_libname = fs.name(posix32_crt.full_path())

# Dependency object to link against `posix32-crt`
posix32_crt_dep = declare_dependency (
  link_with: [
    posix32_crt
  ],
  include_directories: [
    p32_includes
  ]
)

if get_option('tests')
  subdir('tests')
endif

unset_variable ('posix32_crt_sources')
unset_variable ('posix32_crt_args')
unset_variable ('posix32_crt_includes')
