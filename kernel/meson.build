kernel_sources = files('main.c')

kernel = executable(
    'kernel',
    kernel_sources,
    dependencies: [limine_dep],
    include_directories: [
        include_directories('../include'),
        include_directories('../include/klibc'),
    ],
    link_with: klibc,
    install: false,
)