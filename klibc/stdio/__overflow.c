#include "../internal/stdio_impl.h"

int __overflow(FILE *file, int ch)
{
    unsigned char c = ch;

    if (!file->write_end && __towrite(file)) { return EOF; }

    if ((file->write_pos != file->write_end) && (c != file->lbf)) { return *file->write_pos++ = c; }

    if (file->write(file, &c, 1) != 1) { return EOF; }

    return c;
}