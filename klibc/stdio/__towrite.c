#include "../internal/stdio_impl.h"

int __towrite(FILE *stream) {
  stream->mode |= stream->mode - 1;

  if (stream->flags & FILE_NO_WRITE) {
    stream->flags |= FILE_ERROR;
    return EOF;
  }

  stream->read_pos = stream->read_end = nullptr;

  // Activate write through the buffer
  stream->write_pos = stream->write_base = stream->buf;
  stream->write_end = stream->buf + stream->buf_size;

  return 0;
}