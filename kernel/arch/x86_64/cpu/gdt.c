#include <klibc/string.h>
#include <lock.h>
#include <log.h>

#include <kernel/arch/x86_64/cpu/gdt.h>

#define DEFAULT_LIMIT 0xffff

#define GDT_ACCESSED (1 << 0)
#define GDT_READWRITE (1 << 1)
#define GDT_CONFORMING (1 << 2)
#define GDT_EXECUTABLE (1 << 3)
#define GDT_SEGMENT (1 << 4)
#define GDT_USER (3 << 5)
#define GDT_PRESENT (1 << 7)

#define GDT_LONG_MODE_GRANULARITY (1 << 1)
#define GDT_DB (1 << 2)
#define GDT_GRANULARITY (1 << 3)

#define GDT_KERNEL_CODE (1)
#define GDT_KERNEL_DATA (2)
#define GDT_USER_DATA (3)
#define GDT_USER_CODE (4)

#define GDT_RING_3 (3)

#define GDT_DATA_SEGMENT (GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE)
#define GDT_CODE_SEGMENT (GDT_DATA_SEGMENT | GDT_EXECUTABLE)

struct gdt gdt = {};
struct tss global_tss = {};
struct mutex gdt_mutex = INITIALIZE_MUTEX;

static void set_gdt_segment(struct gdt_segment *segment, uint32_t base, uint32_t limit,
                            uint8_t granularity, uint8_t access) {
  segment->limit = limit & 0xffff;
  segment->base_low = base & 0xffff;
  segment->base_mid = (base >> 16) & 0xff;
  segment->access = access;
  segment->limit_high = (limit >> 16) & 0x0f;
  segment->granularity = granularity;
  segment->base_high = (base >> 24) & 0xff;
}

static void set_tss_segment(struct tss_segment *segment, struct tss *tss) {
  const uintptr_t base = (uintptr_t)tss;

  segment->limit = sizeof(struct tss) - 1;
  segment->base_low = base & 0xffff;
  segment->base_mid = (base >> 16) & 0xff;
  segment->flags_low = 0x89;
  segment->flags_high = 0;
  segment->base_high = (base >> 24) & 0xff;
  segment->base_upper = (base >> 32) & 0xffffffff;
  segment->reserved = 0;
}

void gdt_initialize(void) {
  mutex_lock(&gdt_mutex);

  memset(&gdt, 0, sizeof(struct gdt));
  memset(&global_tss, 0, sizeof(struct tss));

  set_gdt_segment(&gdt.entries[0], 0, 0, 0, 0);

  set_gdt_segment(&gdt.entries[GDT_KERNEL_CODE], 0, DEFAULT_LIMIT,
                  GDT_LONG_MODE_GRANULARITY | GDT_GRANULARITY, GDT_CODE_SEGMENT);

  set_gdt_segment(&gdt.entries[GDT_KERNEL_DATA], 0, DEFAULT_LIMIT, GDT_DB | GDT_GRANULARITY,
                  GDT_DATA_SEGMENT);

  set_gdt_segment(&gdt.entries[GDT_USER_DATA], 0, DEFAULT_LIMIT, GDT_DB | GDT_GRANULARITY,
                  GDT_DATA_SEGMENT | GDT_USER);

  set_gdt_segment(&gdt.entries[GDT_USER_CODE], 0, DEFAULT_LIMIT,
                  GDT_LONG_MODE_GRANULARITY | GDT_GRANULARITY, GDT_CODE_SEGMENT | GDT_USER);

  set_tss_segment(&gdt.tss_segment, &global_tss);

  struct gdt_register gdtr = {
      .limit = sizeof(struct gdt) - 1,
      .base = (uintptr_t)&gdt,
  };

  load_gdt(&gdtr);
  load_tss();

  log_info("Initialized Global Description Table!");
  log_debug("Global Description Table located @ %p", &gdt);

  mutex_unlock(&gdt_mutex);
}