
##
# Tests for posix32-crt library
#

tests = [
  'locale_map',
  'locale_resolver',
  'user_default_locale',
]

foreach test : tests
  test ('locale/misc/@0@'.format(test),
    executable ('misc.@0@'.format(test), 'misc/@0@.c'.format(test),
      dependencies: [posix32_crt_test_dep],
    ),
  )
endforeach

tests = [
  'posix',
  'qps-ploc',
  'qps-ploca',
  'qps-plocm',
]

foreach test : tests
  test ('locale/locales/@0@'.format(test),
    executable ('locales.@0@'.format(test), 'locales/@0@.c'.format(test),
      dependencies: [posix32_crt_test_dep],
    ),
  )
endforeach

tests = [
  'bad-charset',
  'from-crt.fail',
  'from-crt.pass',
  'from-env.pass',
  'setlocale',
]

foreach test : tests
  test ('locale/setlocale/@0@'.format(test),
    executable ('setlocale.@0@'.format(test), 'setlocale/@0@.c'.format(test),
      dependencies: [posix32_crt_test_dep],
    ),
  )
endforeach

tests = [
  'ansi',
  'ascii',
  'cp',
  'ebcdic',
  'mac',
  'oem',
  'utf-8',
]

foreach test : tests
  test ('locale/newlocale/@0@'.format(test),
    executable ('newlocale.@0@'.format(test), 'newlocale/@0@.c'.format(test),
      dependencies: [posix32_crt_test_dep],
    ),
  )
endforeach

tests = [
  'bad-charset',
  'from-crt.fail',
  'from-crt.pass',
  'uselocale',
]

foreach test : tests
  test ('locale/uselocale/@0@'.format(test),
    executable ('uselocale.@0@'.format(test), 'uselocale/@0@.c'.format(test),
      dependencies: [posix32_crt_test_dep],
    ),
  )
endforeach

tests = [
  'duplocale',
  'lc_global_locale',
  'lc_global_locale.from-crt',
]

foreach test : tests
  test ('locale/duplocale/@0@'.format(test),
    executable ('duplocale.@0@'.format(test), 'duplocale/@0@.c'.format(test),
      dependencies: [posix32_crt_test_dep],
    ),
  )
endforeach

unset_variable ('tests')
