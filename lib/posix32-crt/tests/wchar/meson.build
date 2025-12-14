
##
# Tests for posix32-crt library
#

tests = [
  'mblen',
  'mbtowc',
  'mbstowcs',
  'wctomb',
  'wcstombs',
]

variants = [
  'ascii',
  'dbcs',
  'posix',
  'sbcs',
  'utf-8',
]

foreach test : tests
  foreach variant : variants
    test ('wchar/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format (test, variant), 'c89/@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

tests = [
  'mbrlen',
  'mbrtowc',
  'mbsrtowcs',
  'mbsnrtowcs',
  'wcrtomb',
  'wcsrtombs',
  'wcsnrtombs',
]

foreach test : tests
  foreach variant : variants
    test ('wchar/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format (test, variant), 'c95/@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

tests = [
	'btowc',
	'wctob',
]

variants = [
  'ascii',
  'compat',
  'dbcs',
  'posix',
  'sbcs',
  'utf-8',
]

foreach test : tests
  foreach variant : variants
    test ('wchar/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format (test, variant), 'c95/@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

unset_variable ('variants')
unset_variable ('tests')
