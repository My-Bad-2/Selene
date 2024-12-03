#include <klibc/string.h>
#include <stdint.h>

#include "../internal/memcopy.h"

void *memset(void *dest, int symbol, size_t len)
{
    uintptr_t destp = (uintptr_t)dest;

    if (len >= 8) {
        size_t length = 0;
        op_t word = 0;

        word = (uint8_t)symbol;
        word |= word << 8;
        word |= word << 16;

        if (OPSIZ > 4) {
            // Do the shift in two dteps to avoid warning if long has 32 bits
            word |= (word << 16) << 16;
        }

        while (destp % OPSIZ != 0) {
            ((uint8_t *)destp)[0] = symbol;
            destp++;
            length--;
        }

        // Write 8 `op_t` per iteration until less than 8 `op_t` remain.
        length = length / (OPSIZ * 8);

        while (length > 0) {
            ((op_t *)destp)[0] = word;
            ((op_t *)destp)[1] = word;
            ((op_t *)destp)[2] = word;
            ((op_t *)destp)[3] = word;
            ((op_t *)destp)[4] = word;
            ((op_t *)destp)[5] = word;
            ((op_t *)destp)[6] = word;
            ((op_t *)destp)[7] = word;

            destp += 8 * OPSIZ;
            length--;
        }

        length %= OPSIZ * 8;

        // Write 1 `op_t` per iteration
        length = length / OPSIZ;

        while (length > 0) {
            ((op_t *)destp)[0] = word;
            destp += OPSIZ;
            length--;
        }

        length %= OPSIZ;
    }

    // Write the last few bytes
    while (len > 0) {
        ((uint8_t *)destp)[0] = symbol;
        destp++;
        len--;
    }

    return dest;
}