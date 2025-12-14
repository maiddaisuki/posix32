
##
# Tests for posix32-crt library
#

tests = [
  'iswctype',
]

variants = [
  'compat',
  'locale',
]

foreach test : tests
  foreach variant : variants
    test ('wctype/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format (test, variant), '@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

tests = [
  'towctrans',
]

variants = [
  'locale',
  # posix32-specific character mappings
  'to-ja-hiragana',
  'to-ja-katakana',
  'to-zh-simplified',
  'to-zh-traditional',
]

foreach test : tests
  foreach variant : variants
    test ('wctype/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format (test, variant), '@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

tests = [
  'towlower',
  'towupper'
]

variants = [
  'compat',
]

foreach test : tests
  foreach variant : variants
    test ('wctype/@0@/@1@'.format(test, variant),
      executable ('@0@.@1@'.format (test, variant), '@0@/@1@.c'.format(test, variant),
        dependencies: [posix32_crt_test_dep],
      )
    )
  endforeach
endforeach

unset_variable ('variants')
unset_variable ('tests')
