#include "../internal/stdio_impl.h"
#include <kernel/api/calls.h>

size_t __stdio_write(FILE *file, const unsigned char *buf, size_t len)
{
    struct iovec iovs[2] = {
        { .buffer = file->write_base, .len = file->write_pos - file->write_base },
        { .buffer = buf, .len = len },
    };

    struct iovec *iov = iovs;

    size_t rem = iovs[0].len + iovs[1].len;
    int iov_cnt = 2;
    size_t cnt = 0;

    while (true) {
        cnt = kernel_writev(iov, iov_cnt);

        if (cnt == rem) {
            file->write_end = file->buf + file->buf_size;
            file->write_pos = file->write_base = file->buf;

            return len;
        }

        if (cnt < 0) {
            file->write_pos = file->write_base = file->write_end = 0;
            file->flags |= FILE_ERROR;

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
}