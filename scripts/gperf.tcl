#/bin/env tclsh

if "$argc < 2" then {
  puts stderr "Usage: $0 path-to-gperf srcdir"
  exit 1
}

set gperf  [lindex $argv 0]
set srcdir [lindex $argv 1]

set sources [list \
  charset         \
  country_codes   \
  country_names   \
  modifier        \
  language_codes  \
  language_names  \
  script          \
  sorting         \
  sorting_id      \
  sublanguage     \
]

foreach src $sources {
  try {
    exec $gperf \
      $srcdir/lib/posix32-crt/gperf/${src}.gperf \
      --output-file=$srcdir/lib/posix32-crt/gperf/${src}.c
  } on error {msg} {
    puts stderr $msg
    exit 1
  }
}

exit 0
