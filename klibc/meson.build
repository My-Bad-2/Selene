klibc_sources = files('ctype.c')

subdir('string')
subdir('internal')
subdir('stdio')
subdir('stdlib')

clangtidy_files += klibc_sources

klibc = static_library(
  'klibc',
  klibc_sources,
  build_by_default: true,
  include_directories: [
    include_directories('../include/klibc'),
    include_directories('../include'),
  ],
  install: false,
)