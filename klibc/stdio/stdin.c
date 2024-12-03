#include "../internal/stdio_impl.h"

static unsigned char buffer[BUFSIZ + UNGET] = {};

__LOCAL FILE __stdin_FILE = {
    .buf = buffer + UNGET,
    .buf_size = sizeof(buffer) - UNGET,
    .flags = FILE_PERM | FILE_NO_WRITE,
    .lock = INITIALIZE_MUTEX,
};

FILE *const stdin = &__stdin_FILE;