#include <klibc/string.h>
#include <stdint.h>

#include "../internal/memcopy.h"

void *memmove(void *dest, const void *src, size_t length)
{
    uintptr_t destp = (uintptr_t)dest;
    uintptr_t srcp = (uintptr_t)src;

    if (destp - srcp >= length) {
        dest = memcpy(dest, src, length);
    } else {
        srcp += length;
        destp += length;

        if (length >= OP_T_THRES) {
            length -= destp % OPSIZ;

            // NOLINTNEXTLINE
            BYTE_COPY_BWD(destp, srcp, destp % OPSIZ);
            WORD_COPY_BWD(destp, srcp, length, length);
        }

        // NOLINTNEXTLINE
        BYTE_COPY_BWD(destp, srcp, length);
    }

    return dest;
}