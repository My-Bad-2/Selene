#include "../internal/stdio_impl.h"

int __overflow(FILE *stream, int sym)
{
    unsigned char symbol = sym;

    if (!stream->write_end && __towrite(stream)) { return EOF; }

    if ((stream->write_pos != stream->write_end) && (symbol != stream->lbf)) {
        return *stream->write_pos++ = symbol;
    }

    if (stream->write(stream, &symbol, 1) != 1) { return EOF; }

    return symbol;
}