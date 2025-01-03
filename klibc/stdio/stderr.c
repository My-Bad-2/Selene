#include "../internal/stdio_impl.h"

static unsigned char buffer[UNGET];

__LOCAL FILE __stderr_FILE = {
    .buf = buffer + UNGET,
    .buf_size = 0,
    .flags = FILE_PERM | FILE_NO_READ | FILE_SVB,
    .lbf = -1,
    .write = __stdio_write,
};

FILE *const stderr = &__stderr_FILE;