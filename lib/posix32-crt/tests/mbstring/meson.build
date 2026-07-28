
##
# Tests for posix32-crt library
#

tests = [
  'strlen', 'strnlen',
  'strcpy', 'strncpy',
  'strchr',
  'strcspn',
  'strrchr',
  'strpbrk',
  'strspn',
  'strstr',
  'strtok_r',
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
    test ('mbstring/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format(test, variant), '@0@/@1@.c'.format(test, variant),
        c_args:       [p32_test_args],
        link_args:    [p32_test_link_args],
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

unset_variable ('variants')
unset_variable ('tests')
