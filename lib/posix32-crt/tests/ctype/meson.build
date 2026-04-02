
##
# Tests for posix32-crt library
#

tests = [
  'isctype',
  'tolower',
  'toupper'
]

variants = [
  'compat',
  'posix',
]

foreach test : tests
  foreach variant : variants
    test ('ctype/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format (test, variant), '@0@/@1@.c'.format(test, variant),
        c_args:       [p32_test_args],
        link_with:    [p32_test_link_args],
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

unset_variable ('variants')
unset_variable ('tests')
