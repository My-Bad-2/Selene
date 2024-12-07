#ifndef KLIBC_INTERNAL_STDIO_IMPL_H
#define KLIBC_INTERNAL_STDIO_IMPL_H 1

#include <compiler.h>
#include <klibc/stdio.h>
#include <lock.h>
#include <stddef.h>

#define LOCK_STREAM(stream) (mutex_try_lock(&stream->lock))
#define UNLOCK_STREAM(stream) (mutex_unlock(&stream->lock))

#define NL_ARGMAX 9

#define UNGET 9

#define FILE_PERM 1u
#define FILE_NO_READ 4u
#define FILE_NO_WRITE 8u
#define FILE_EOF 16u
#define FILE_ERROR 32u
#define FILE_SVB 64u
#define FILE_APPEND 128u

struct _IO_FILE {
  unsigned flags;
  unsigned char *read_pos, *read_end;
  unsigned char *write_end, *write_pos;
  unsigned char *write_base;
  size_t (*write)(FILE *, const unsigned char *, size_t);
  unsigned char *buf;
  void *cookie;
  size_t buf_size;
  int offset;
  unsigned int mode;
  int lbf;
  struct mutex lock;
};

__LOCAL int __towrite(FILE *stream);
__LOCAL size_t __stdio_write(FILE *stream, const unsigned char *buf, size_t len);
__LOCAL size_t __fwrite(const unsigned char *restrict str, size_t length, FILE *restrict stream);
__LOCAL int __overflow(FILE *stream, int sym);

#endif  // KLIBC_INTERNAL_STDIO_IMPL_H