kernel_sources = files('main.c')

subdir('arch' / host_machine.cpu_family())
subdir('api')

kernel = executable(
  'kernel',
  kernel_sources,
  dependencies: [limine_dep],
  include_directories: [
    include_directories('../include'),
    include_directories('../include/klibc'),
  ],
  c_args: '-DLIMINE_API_REVISION=2',
  cpp_args: '-DLIMINE_API_REVISION=2',
  link_with: [klibc, misc],
  install: false,
)