misc_sources = files('log.cpp')

clangtidy_files += misc_sources

misc = static_library(
  'misc',
  misc_sources,
  build_by_default: true,
  include_directories: [
    include_directories('../include/klibc'),
    include_directories('../include'),
  ],
  dependencies: libstdcxx_dep,
  install: false,
)