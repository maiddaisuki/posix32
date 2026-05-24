
##
# Tests for posix32-core library
#

tests = [
  'form-c',
  'form-d',
  'form-kc',
  'form-kd',
]

foreach test : tests
  test ('norm/@0@'.format(test),
    executable ('@0@'.format (test), '@0@.c'.format(test),
      c_args:       [p32_test_args],
      link_args:    [p32_test_link_args],
      dependencies: [posix32_core_test_dep],
    )
  )
endforeach

unset_variable ('tests')
