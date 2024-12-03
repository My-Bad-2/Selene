#include "../internal/stdio_impl.h"

int putc_unlocked(int ch, FILE *stream)
{
    if (((unsigned char)ch != stream->lbf) && (stream->write_pos != stream->write_end)) {
        return *stream->write_pos++ = (unsigned char)ch;
    } else {
        return __overflow(stream, (unsigned char)ch);
    }
}

int putc(int ch, FILE *stream)
{
    LOCK_STREAM(stream);
    int ret = putc_unlocked(ch, stream);
    UNLOCK_STREAM(stream);

    return ret;
}