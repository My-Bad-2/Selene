#include "../internal/stdio_impl.h"

int ferror(FILE *stream)
{
    LOCK_STREAM(stream);
    int ret = !!(stream->flags & FILE_ERROR);
    UNLOCK_STREAM(stream);

    return ret;
}