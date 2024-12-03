#include "../internal/stdio_impl.h"

static unsigned char buffer[BUFSIZ + UNGET] = {};

static size_t __stdout_write(FILE *stream, const unsigned char *str, size_t len)
{
    stream->write = __stdio_write;

    if (!(stream->flags & FILE_SVB)) { stream->lbf = -1; }

    return __stdio_write(stream, str, len);
}

__LOCAL FILE __stdout_FILE = {
    .buf = buffer + UNGET,
    .buf_size = sizeof(buffer) - UNGET,
    .flags = FILE_PERM | FILE_NO_READ,
    .lbf = '\n',
    .write = __stdout_write,
    .lock = INITIALIZE_MUTEX,
};

FILE *const stdout = &__stdout_FILE;