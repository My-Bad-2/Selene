#include "../internal/stdio_impl.h"
#include <kernel/api/calls.h>

size_t __stdio_write(FILE *stream, const unsigned char *buf, size_t len)
{
    struct iovec iovs[2] = {
        { .buffer = stream->write_base, .len = stream->write_pos - stream->write_base },
        { .buffer = (void *)buf, .len = len },
    };

    struct iovec *iov = iovs;

    size_t rem = iovs[0].len + iovs[1].len;
    int iov_cnt = 2;
    size_t cnt = 0;

    while (true) {
        cnt = kernel_writev(iov, iov_cnt);

        if (cnt == rem) {
            stream->write_end = stream->buf + stream->buf_size;
            stream->write_pos = stream->write_base = stream->buf;

            return len;
        }

        if (cnt < 0) {
            stream->write_pos = stream->write_base = stream->write_end = NULL;
            stream->flags |= FILE_ERROR;

            return (iov_cnt == 2) ? 0 : len - iov[0].len;
        }

        rem -= cnt;

        if (cnt > iov[0].len) {
            cnt -= iov[0].len;
            iov++;
            iov_cnt--;
        }

        iov[0].buffer = (char *)iov[0].buffer + cnt;
        iov[0].len -= cnt;
    }

    return 0;
}