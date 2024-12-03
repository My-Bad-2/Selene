/**
 * @file calls.h
 * @brief Kernel API calls for klibc.
 */
#ifndef KERNEL_API_CALLS_H
#define KERNEL_API_CALLS_H 1

#include <stddef.h>
#include <stdint.h>

/**
 * @struct iovec
 * @brief Represents a buffer and its length for vectorized I/O operations.
 */
struct iovec
{
    void *buffer;///< Starting address of the buffer.
    size_t len;  ///< Length of the buffer in bytes.
};

/**
 * @brief Writes data from multiple buffers to the output device.
 * @param buffer Array of `iovec` structures containing the buffers to write.
 * @param count Number of buffers in the array.
 * @return The total number of bytes written, or -1 on error.
 */
int kernel_writev(struct iovec *buffer, int count);

#endif// KERNEL_API_CALLS_H