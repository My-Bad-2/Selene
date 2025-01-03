#include <compiler.h>
#include <stddef.h>

#include <kernel/kernel.h>

__SECTION(".limine_requests") static volatile LIMINE_BASE_REVISION(3);

__SECTION(".limine_requests_start_marker") __USED static volatile LIMINE_REQUESTS_START_MARKER;

__SECTION(".limine_requests")
volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr,
};

__SECTION(".limine_requests")
volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr,
};

__SECTION(".limine_requests")
volatile struct limine_paging_mode_request paging_mode_request = {
    .id = LIMINE_PAGING_MODE_REQUEST,
    .revision = 0,
    .response = nullptr,
#ifdef __x86_64__
    .mode = LIMINE_PAGING_MODE_X86_64_5LVL,
    .max_mode = LIMINE_PAGING_MODE_X86_64_5LVL,
    .min_mode = LIMINE_PAGING_MODE_X86_64_4LVL,
#endif
};

__SECTION(".limine_requests")
volatile struct limine_executable_address_request kernel_address_request = {
  .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST,
  .revision = 0,
  .response = nullptr,
};

__SECTION(".limine_requests")
volatile struct limine_executable_file_request kernel_file_request = {
  .id = LIMINE_EXECUTABLE_FILE_REQUEST,
  .revision = 2,
  .response = nullptr,
};

__SECTION(".limine_requests_end_marker") __USED static volatile LIMINE_REQUESTS_END_MARKER;