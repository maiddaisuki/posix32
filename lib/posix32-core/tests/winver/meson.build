
##
# Tests for posix32-core library
#

tests = [
  'winver',
]

foreach test : tests
  test ('winver/@0@'.format(test),
    executable ('@0@'.format (test), '@0@.c'.format(test),
      c_args:       [p32_test_args],
      link_with:    [p32_test_link_args],
      dependencies: [posix32_core_test_dep],
    )
  )
endforeach

unset_variable ('tests')
