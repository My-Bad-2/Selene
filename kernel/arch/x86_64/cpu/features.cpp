#include <cpuid.h>

#include <kernel/arch/x86_64/cpu/features.hpp>

bool read_cpuid(CpuidLeaf *leaf, uint32_t leaf_num, uint32_t subleaf_num) {
  return __get_cpuid_count(leaf_num, subleaf_num, &leaf->values[0], &leaf->values[1],
                           &leaf->values[2], &leaf->values[3]);
}

bool test_feature(CpuidBit bit) {
  if ((bit.word > 3) || (bit.bit > 31)) {
    return false;
  }

  CpuidLeaf leaf = {};

  if (!read_cpuid(&leaf, bit.leaf, 0)) {
    return false;
  }

  return !!((1u << bit.bit) & leaf.values[bit.word]);
}