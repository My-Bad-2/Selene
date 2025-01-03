#include <compiler.h>

typedef void (*func_ptr)(void);

extern func_ptr __init_array_start[0], __init_array_end[0];
extern func_ptr __fini_array_start[0], __fini_array_end[0];

void _init() {
  for (func_ptr* func = __init_array_start; func != __init_array_end; func++) {
    (*func)();
  }
}

void _fini() {
  for (func_ptr* func = __fini_array_start; func != __fini_array_end; func++) {
    (*func)();
  }
}

/* crti.c for ARM - BPABI - use -std=c99 */

func_ptr __init_array_start[0] __USED __SECTION(".init_array") __ALIGNED(sizeof(func_ptr)) = { };
func_ptr __fini_array_start[0] __USED __SECTION(".fini_array") __ALIGNED(sizeof(func_ptr)) = { };
