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

__SECTION(".limine_requests_end_marker") __USED static volatile LIMINE_REQUESTS_END_MARKER;