misc_sources = files('lock.c', 'log.c')

clangtidy_files += misc_sources

misc = static_library(
    'misc',
    misc_sources,
    build_by_default: true,
    include_directories: [
        include_directories('../include/klibc'),
        include_directories('../include'),
    ],
    install: false,
)