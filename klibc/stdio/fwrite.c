#include <klibc/string.h>

#include "../internal/stdio_impl.h"

size_t __fwrite(const unsigned char *restrict str, size_t length, FILE *restrict stream) {
  size_t i = 0;

  if (!stream->write_end && __towrite(stream)) {
    return 0;
  }

  if (length > (size_t)(stream->write_end - stream->write_pos)) {
    return stream->write(stream, str, length);
  }

  if (stream->lbf >= 0) {
    for (i = length; i && str[i - 1] != '\n'; i--) {
    }

    if (i) {
      size_t n = stream->write(stream, str, i);

      if (n < i) {
        return n;
      }

      str += i;
      length -= i;
    }
  }

  memcpy(stream->write_pos, str, length);
  stream->write_pos += length;

  return length + i;
}

size_t fwrite(const void *restrict ptr, size_t size, size_t nmemb, FILE *restrict stream) {
  size_t length = size * nmemb;
  if (!size) {
    nmemb = 0;
  }

  LOCK_STREAM(stream);
  size_t k = __fwrite(ptr, length, stream);
  UNLOCK_STREAM(stream);

  return (k == length) ? nmemb : (k / size);
}