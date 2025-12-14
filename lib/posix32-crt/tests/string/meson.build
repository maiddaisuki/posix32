
##
# Tests for posix32-crt library
#

tests = [
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
    test ('string/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format(test, variant), '@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

tests = [
  'strcasecmp',
  'strncasecmp',
  'strxfrm',
]

variants = [
  'locale',
]

foreach test : tests
  foreach variant : variants
    test ('string/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format(test, variant), '@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

tests = [
  'strcasecmp',
  'strcoll',
]

variants = [
  'compat',
]

foreach test : tests
  foreach variant : variants
    test ('string/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format(test, variant), '@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

unset_variable ('variants')
unset_variable ('tests')
