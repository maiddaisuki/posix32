
##
# Tests for posix32-crt library
#

tests = [
  'c8rtomb',  'mbrtoc8',
  'c16rtomb', 'mbrtoc16',
  'c32rtomb', 'mbrtoc32',
]

variants = [
  'ascii',
  'dbcs',
  'posix',
  'sbcs',
  'utf-8'
]

foreach test : tests
  foreach variant : variants
    test ('uchar/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format (test, variant), '@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

tests = [
  'correct1',
  'correct2'
]

foreach test : tests
  test ('uchar/@0@'.format(test),
    executable ('@0@'.format (test), '@0@.c'.format(test),
      dependencies: [posix32_crt_test_dep],
    )
  )
endforeach

unset_variable ('variants')
unset_variable ('tests')
