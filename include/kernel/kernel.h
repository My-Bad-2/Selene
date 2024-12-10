#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H 1

#include <limine.h>

extern volatile struct limine_memmap_request memmap_request;
extern volatile struct limine_hhdm_request hhdm_request;

#endif  // KERNEL_KERNEL_H