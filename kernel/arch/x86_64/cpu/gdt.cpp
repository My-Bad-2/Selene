#include <klibc/string.h>
#include <log.hpp>

#include <kernel/arch/x86_64/cpu/gdt.hpp>

#define DEFAULT_LIMIT 0xffff

#define GDT_KERNEL_CODE (1)
#define GDT_KERNEL_DATA (2)
#define GDT_USER_DATA (3)
#define GDT_USER_CODE (4)

enum GdtAccess : uint8_t {
  GDT_ACCESSED = (1 << 0),
  GDT_READWRITE = (1 << 1),
  GDT_CONFORMING = (1 << 2),
  GDT_EXECUTABLE = (1 << 3),
  GDT_SEGMENT = (1 << 4),
  GDT_USER = (3 << 5),
  GDT_PRESENT = (1 << 7),

  GDT_DATA_SEGMENT = (GDT_PRESENT | GDT_SEGMENT | GDT_READWRITE),
  GDT_CODE_SEGMENT = (GDT_DATA_SEGMENT | GDT_EXECUTABLE),
};

enum GdtGranularity : uint8_t {
  GDT_LONG_MODE_GRANULARITY = (1 << 1),
  GDT_DB = (1 << 2),
  GDT_GRANULARITY = (1 << 3),
};

void GdtSegment::set(uint32_t base, uint32_t limit, uint8_t granularity, uint8_t access) {
  this->limit = limit & 0xffff;
  this->base_low = base & 0xffff;
  this->base_mid = (base >> 16) & 0xff;
  this->access = access;
  this->limit_high = (limit >> 16) & 0x0f;
  this->granularity = granularity;
  this->base_high = (base >> 24) & 0xff;
}

void TssSegment::set(Tss *tss) {
  const uintptr_t base = reinterpret_cast<uintptr_t>(tss);

  this->limit = sizeof(Tss) - 1;
  this->base_low = base & 0xffff;
  this->base_mid = (base >> 16) & 0xff;
  this->flags_low = 0x89;
  this->flags_high = 0;
  this->base_high = (base >> 24) & 0xff;
  this->base_upper = (base >> 32) & 0xffffffff;
  this->reserved = 0;
}

void Gdt::initialize() {
  memset(&this->m_table, 0, sizeof(GdtTable));
  memset(&this->m_tss, 0, sizeof(Tss));

  this->m_table.segments[0].set(0, 0, 0, 0);

  this->m_table.segments[GDT_KERNEL_CODE].set(
      0, DEFAULT_LIMIT, GDT_LONG_MODE_GRANULARITY | GDT_GRANULARITY, GDT_CODE_SEGMENT);

  this->m_table.segments[GDT_KERNEL_DATA].set(0, DEFAULT_LIMIT, GDT_DB | GDT_GRANULARITY,
                                              GDT_DATA_SEGMENT);

  this->m_table.segments[GDT_USER_DATA].set(0, DEFAULT_LIMIT, GDT_DB | GDT_GRANULARITY,
                                            GDT_DATA_SEGMENT | GDT_USER);

  this->m_table.segments[GDT_USER_CODE].set(
      0, DEFAULT_LIMIT, GDT_LONG_MODE_GRANULARITY | GDT_GRANULARITY, GDT_CODE_SEGMENT | GDT_USER);

  this->m_table.tss_segment.set(&this->m_tss);

  this->load();

  log_info("Initialized Global Description Table!");
  log_debug("Global Description Table located @ %p", &this->m_table);
}

void Gdt::load() {
  GdtRegister gdtr = {
      .limit = sizeof(GdtTable) - 1,
      .base = reinterpret_cast<uintptr_t>(&this->m_table),
  };

  load_gdt(&gdtr);
  load_tss();
}