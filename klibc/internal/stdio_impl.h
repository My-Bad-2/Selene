#ifndef KLIBC_INTERNAL_STDIO_IMPL_H
#define KLIBC_INTERNAL_STDIO_IMPL_H 1

#include <compiler.h>
#include <stddef.h>
#include <klibc/stdio.h>

#define NL_ARGMAX 9

#define UNGET 9

#define FILE_PERM 1
#define FILE_NO_READ 4
#define FILE_NO_WRITE 8
#define FILE_EOF 16
#define FILE_ERROR 32
#define FILE_SVB 64
#define FILE_APPEND 128

struct _IO_FILE
{
    unsigned flags;
    unsigned char *read_pos, *read_end;
    unsigned char *write_end, *write_pos;
    unsigned char *write_base;
    size_t (*write)(FILE *, const unsigned char *, size_t);
    unsigned char *buf;
    void *cookie;
    size_t buf_size;
    int offset;
    int mode;
    int lbf;
};

__LOCAL int __towrite(FILE *file);
__LOCAL size_t __stdio_write(FILE *file, const unsigned char *buf, size_t len);
__LOCAL size_t __fwrite(const unsigned char *restrict str, size_t length, FILE *restrict file);
__LOCAL int __overflow(FILE *file, int ch);

#endif// KLIBC_INTERNAL_STDIO_IMPL_H