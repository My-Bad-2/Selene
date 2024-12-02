#include "../internal/stdio_impl.h"
#include <klibc/string.h>

size_t __fwrite(const unsigned char *restrict str, size_t length, FILE *restrict file)
{
    size_t i = 0;

    if (!file->write_end && __towrite(file)) { return 0; }

    if (length > (size_t)(file->write_end - file->write_pos)) {
        return file->write(file, str, length);
    }

    if (file->lbf >= 0) {
        for (i = length; i && str[i - 1] != '\n'; i--) {}

        if (i) {
            size_t n = file->write(file, str, i);

            if (n < i) { return n; }

            str += i;
            length -= i;
        }
    }

    memcpy(file->write_pos, str, length);
    file->write_pos += length;

    return length + i;
}

size_t fwrite(const void *restrict ptr, size_t size, size_t nmemb, FILE *restrict stream)
{
    size_t length = size * nmemb;
    size_t k = __fwrite(ptr, length, stream);

    return (k == length) ? nmemb : (k / size);
}