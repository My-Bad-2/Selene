#include "../internal/stdio_impl.h"

static unsigned char buffer[BUFSIZ + UNGET] = {};

static size_t __stdout_write(FILE *file, const unsigned char *str, size_t len)
{
    file->write = __stdio_write;

    if (!(file->flags & FILE_SVB)) { file->lbf = -1; }

    return __stdio_write(file, str, len);
}

__LOCAL FILE __stdout_FILE = {
    .buf = buffer + UNGET,
    .buf_size = sizeof(buffer) - UNGET,
    .flags = FILE_PERM | FILE_NO_READ,
    .lbf = '\n',
    .write = __stdout_write,
};

FILE *const stdout = &__stdout_FILE;