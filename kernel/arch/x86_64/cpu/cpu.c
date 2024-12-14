#include <kernel/arch/x86_64/cpu/cpu.h>

void invalidate_page(uintptr_t address) { asm volatile("invlpg (%0)" ::"r"(address)); }

uint64_t read_cr0() {
  uint64_t value = 0;
  asm volatile("mov %%cr0, %0" : "=r"(value)::"memory");
  return value;
}

uint64_t read_cr2() {
  uint64_t value = 0;
  asm volatile("mov %%cr2, %0" : "=r"(value)::"memory");
  return value;
}

uint64_t read_cr3() {
  uint64_t value = 0;
  asm volatile("mov %%cr3, %0" : "=r"(value)::"memory");
  return value;
}

uint64_t read_cr4() {
  uint64_t value = 0;
  asm volatile("mov %%cr4, %0" : "=r"(value)::"memory");
  return value;
}

void write_cr0(uint64_t value) { asm volatile("mov %0, %%cr0" ::"r"(value) : "memory"); }
void write_cr2(uint64_t value) { asm volatile("mov %0, %%cr2" ::"r"(value) : "memory"); }
void write_cr3(uint64_t value) { asm volatile("mov %0, %%cr3" ::"r"(value) : "memory"); }
void write_cr4(uint64_t value) { asm volatile("mov %0, %%cr4" ::"r"(value) : "memory"); }

uint64_t read_msr(uint32_t msr) {
  uint32_t edx, eax;
  asm volatile("rdmsr" : "=a"(eax), "=d"(edx) : "c"(msr) : "memory");
  return ((uint64_t)edx << 32) | eax;
}

void write_msr(uint32_t msr, uint64_t value) {
  uint32_t edx = value >> 32;
  uint32_t eax = (uint32_t)value;
  asm volatile("wrmsr" ::"a"(eax), "d"(edx), "c"(msr) : "memory");
}

void enable_pat() { write_msr(MSR_PAT, DEFAULT_PAT); }

void fxsave(uint8_t* region) { asm volatile("fxsaveq (%0)" ::"r"(region) : "memory"); }

void xsave(uint8_t* region) {
  asm volatile("xsaveq (%0)" ::"r"(region), "a"(RFBM_LOW), "d"(RFBM_HIGH) : "memory");
}

void xsaveopt(uint8_t* region) {
  asm volatile("xsaveopt64 (%0)" ::"r"(region), "a"(RFBM_LOW), "d"(RFBM_HIGH) : "memory");
}
void xrstor(uint8_t* region) {
  asm volatile("xrstorq (%0)" ::"r"(region), "a"(RFBM_LOW), "d"(RFBM_HIGH) : "memory");
}

void fxrstor(uint8_t* region) {
  asm volatile("fxrstorq (%0)" ::"r"(region), "a"(RFBM_LOW), "d"(RFBM_HIGH) : "memory");
}

uintptr_t kernel_gs_base() { return read_msr(MSR_KERNEL_GS_BASE); }

uintptr_t fs_base() { return read_msr(MSR_FS_BASE); }

uintptr_t gs_base() { return read_msr(MSR_GS_BASE); }

void set_fs_base(uintptr_t addr) { write_msr(MSR_FS_BASE, addr); }

void set_gs_base(uintptr_t addr) { write_msr(MSR_GS_BASE, addr); }

void set_kernel_gs_base(uintptr_t addr) { write_msr(MSR_KERNEL_GS_BASE, addr); }

void swapgs() { asm volatile("swapgs"); }