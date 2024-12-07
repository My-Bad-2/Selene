#include "../internal/stdio_impl.h"

int feof(FILE *stream) {
  LOCK_STREAM(stream);
  int ret = !!(stream->flags & FILE_EOF);
  UNLOCK_STREAM(stream);

  return ret;
}