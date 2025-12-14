#/bin/env tclsh

if "$argc < 1" then {
  puts stderr "Usage: $0 header-file"
  exit 1
}

set header [lindex $argv 0]

if "[lsearch [array names env] INCLUDE] != -1" then {
  foreach dir [split $env(INCLUDE) ";"] {
    if [file exists $dir/$header] then {
      puts stdout [file normalize $dir/$header]
      exit 0
    }
  }
}

exit 1
