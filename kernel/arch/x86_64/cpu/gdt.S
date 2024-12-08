#include <asm.h>

#define KERNEL_CODE_SELECTOR (1 * 8)
#define KERNEL_DATA_SELECTOR (2 * 8)
#define USER_DATA_SELECTOR (3 * 8)
#define USER_CODE_SELECTOR (4 * 8)
#define TSS_SELECTOR (5 * 8)

.function load_gdt, scope=global, align=64
  // Load GDT Table
  lgdtq (%rdi)

  // Reload Selector Segments
  movw $KERNEL_DATA_SELECTOR, %ax
  movl %eax, %ds
  movl %eax, %es
  movl %eax, %fs
  movl %eax, %gs
  movl %eax, %ss

  // Load the return address
  lea 1f(%rip), %rax

  // Push Kernel code selector value & return address
  push_value $KERNEL_CODE_SELECTOR
  push_reg %rax

  // Long Return
  lretq
  int3
1:
  RET_AND_SPECULATION_POSTFENCE
.end_function

.function load_tss, scope=global
  movw $TSS_SELECTOR, %ax
  ltrw %ax
  RET_AND_SPECULATION_POSTFENCE
.end_function