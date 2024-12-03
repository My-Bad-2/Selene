#include "../internal/stdio_impl.h"

#include <klibc/string.h>
#include <limits.h>

struct cookie
{
    char *src;
    size_t len;
};

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static size_t sn_write(FILE *stream, const unsigned char *str, size_t length)
{
    struct cookie *cookie = stream->cookie;
    size_t k = MIN(cookie->len, (size_t)(stream->write_pos - stream->write_base));

    if (k) {
        memcpy(cookie->src, stream->write_base, k);
        cookie->src += k;
        cookie->len -= k;
    }

    k = MIN(cookie->len, 1);

    if (k) {
        memcpy(cookie->src, str, k);
        cookie->src += k;
        cookie->len -= k;
    }

    *cookie->src = 0;
    stream->write_pos = stream->write_base = stream->buf;

    return length;
}

int vsnprintf(char *restrict buffer, size_t maxlen, const char *restrict format, va_list arg)
{
    unsigned char buf[1];
    char dummy[1];

    struct cookie cookie = {
        .src = maxlen ? buffer : dummy,
        .len = maxlen ? (maxlen - 1) : 0,
    };

    FILE stream = {
        .lbf = EOF,
        .write = sn_write,
        .buf = buf,
        .cookie = &cookie,
    };

    *cookie.src = 0;

    return vfprintf(&stream, format, arg);
}

int vsprintf(char *restrict buffer, const char *restrict format, va_list arg)
{
    return vsnprintf(buffer, INT_MAX, format, arg);
}

int vprintf(const char *restrict format, va_list arg) { return vfprintf(stdout, format, arg); }