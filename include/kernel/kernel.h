#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H 1

#include <limine.h>
#include <compiler.h>

#define PAGING_MODE_MAX (paging_mode_request.response->mode != LIMINE_PAGING_MODE_MIN)

__CDECLS_BEGIN

extern volatile struct limine_memmap_request memmap_request;
extern volatile struct limine_hhdm_request hhdm_request;
extern volatile struct limine_paging_mode_request paging_mode_request;
extern volatile struct limine_executable_address_request kernel_address_request;
extern volatile struct limine_executable_file_request kernel_file_request;

__CDECLS_END

#endif  // KERNEL_KERNEL_H