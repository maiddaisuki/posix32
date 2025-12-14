
##
# Tests for posix32-crt library
#

tests = [
  'wcscasecmp',
  'wcsncasecmp',
  'wcsxfrm',
]

variants = [
  'locale',
]

foreach test : tests
  foreach variant : variants
    test ('wstring/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format (test, variant), '@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

tests = [
  'wcscasecmp',
  'wcscoll',
]

variants = [
  'compat',
]

foreach test : tests
  foreach variant : variants
    test ('wstring/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format (test, variant), '@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

unset_variable ('variants')
unset_variable ('tests')
