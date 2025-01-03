#include <compiler.h>

typedef void (*func_ptr)(void);

func_ptr __init_array_end[0] __USED __SECTION(".init_array") __ALIGNED(sizeof(func_ptr)) = { };
func_ptr __fini_array_end[0] __USED __SECTION(".fini_array") __ALIGNED(sizeof(func_ptr)) = { };