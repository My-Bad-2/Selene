#include "../internal/stdio_impl.h"

int feof(FILE *stream)
{
    int ret = !!(stream->flags & FILE_EOF);

    return ret;
}