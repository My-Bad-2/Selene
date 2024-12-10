#include <klibc/assert.h>
#include <klibc/stdio.h>

__NO_RETURN void __assert_fail(const char *expr, const char *file, int line, const char *func) {
  fprintf(stderr, "Assertion failed: %s (%s: %s: %d)\n", expr, file, func, line);
  __UNREACHABLE();
}