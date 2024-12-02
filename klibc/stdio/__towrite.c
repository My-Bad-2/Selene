#include "../internal/stdio_impl.h"

int __towrite(FILE *file)
{
    file->mode |= file->mode - 1;

    if (file->flags & FILE_NO_WRITE) {
        file->flags |= FILE_ERROR;
        return EOF;
    }

    // Clear read buffer
    file->read_pos = file->read_end = 0;

    // Activate write through the buffer
    file->write_pos = file->write_base = file->buf;
    file->write_end = file->buf + file->buf_size;

    return 0;
}