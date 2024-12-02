#include <kernel/api/calls.h>
#include <kernel/arch/arch.h>

/**
 * @details Iterates through each buffer in the `iovec` array and writes its content to the primary
 * UART (COM1) using `arch_write`. If an invalid buffer is encountered, the function stops and
 * returns -1.
 */
int kernel_writev(struct iovec *iov, int iovcnt)
{
    int total_written = 0;

    for (int i = 0; i < iovcnt; i++) {
        int bytes_written = arch_write(iov[i].buffer, iov[i].len);

        if (bytes_written == -1) { return -1; }

        total_written += bytes_written;

        if ((size_t)bytes_written != iov[i].len) { break; }
    }

    return total_written;
}