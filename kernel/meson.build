kernel_sources = files(
  'kernel.c',
  'main.cpp',
)

subdir('arch' / host_machine.cpu_family())
subdir('api')
subdir('memory')

kernel = executable(
  'kernel',
  kernel_sources,
  dependencies: [
    limine_dep,
    libstdcxx_dep,
  ],
  include_directories: [
    include_directories('../include'),
    include_directories('../include/klibc'),
  ],
  c_args: '-DLIMINE_API_REVISION=2',
  cpp_args: '-DLIMINE_API_REVISION=2',
  link_args: ['-L' + libgcc_path, '-lgcc'],
  link_with: [klibc, misc],
  install: false,
)