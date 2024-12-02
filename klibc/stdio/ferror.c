#include "../internal/stdio_impl.h"

int ferror(FILE *file)
{
    int ret = !!(file->flags & FILE_ERROR);
    return ret;
}