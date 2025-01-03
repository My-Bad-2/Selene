/**
 * @file
 * @brief Defines CPUID-related macros, structures, and functions for querying CPU features.
 *
 * This header provides definitions and utilities to interact with the CPUID instruction,
 * enabling the identification of supported CPU features, cache details, thermal capabilities,
 * and other processor-specific information.
 */
#ifndef CPU_FEATURES_HPP
#define CPU_FEATURES_HPP 1

#include <cstdint>

/** @defgroup CPUID_Leafs CPUID Leafs
 * @{
 */
#define CPUID_BASE 0                      ///< Base CPUID leaf number.
#define CPUID_MODEL_FEATURES 0x1          ///< CPUID leaf for model features.
#define CPUID_CACHE_V1 0x2                ///< CPUID leaf for cache information (version 1).
#define CPUID_CACHE_V2 0x4                ///< CPUID leaf for cache information (version 2).
#define CPUID_MON 0x5                     ///< CPUID leaf for monitor features.
#define CPUID_THERMAL_AND_POWER 0x6       ///< CPUID leaf for thermal and power management features.
#define CPUID_EXTENDED_FEATURE_FLAGS 0x7  ///< CPUID leaf for extended feature flags.
#define CPUID_PERFORMANCE_MONITORING 0xa  ///< CPUID leaf for performance monitoring.
#define CPUID_TOPOLOGY 0xb                ///< CPUID leaf for processor topology.
#define CPUID_XSAVE 0xd                   ///< CPUID leaf for XSAVE features.
#define CPUID_PT 0x14                     ///< CPUID leaf for Processor Trace (PT) features.
#define CPUID_TSC 0x15                    ///< CPUID leaf for TSC (Time Stamp Counter) information.
#define CPUID_EXT_BASE 0x80000000         ///< Base value for extended CPUID leafs.
#define CPUID_FEATS 0x80000001            ///< Extended CPUID leaf for feature flags.
#define CPUID_BRAND 0x80000002            ///< Extended CPUID leaf for processor brand string.
#define CPUID_ADDR_WIDTH 0x80000008       ///< Extended CPUID leaf for address width information.
#define CPUID_AMD_TOPOLOGY 0x8000001e     ///< Extended CPUID leaf for AMD topology.
/** @} */

/**
 * @brief Defines a structure for representing CPUID leaf values.
 *
 * The CPUID leaf contains the values of the four registers (EAX, EBX, ECX, and EDX) returned by the
 * CPUID instruction.
 */
struct CpuidLeaf {
  uint32_t values[4];  ///< Array holding EAX, EBX, ECX, and EDX values.
};

/**
 * @brief Defines a structure for identifying a specific feature bit in a CPUID leaf.
 */
struct CpuidBit {
  uint32_t leaf;  ///< The CPUID leaf number.
  uint8_t word;   ///< The index of the 32-bit word (0 = EAX, 1 = EBX, etc.).
  uint8_t bit;    ///< The bit index within the word.
};

/**
 * @defgroup cpuid_macros CPUID Macros
 * @brief Utilities for working with CPUID feature bits.
 * @{
 */

/**
 * @brief Creates a `struct cpuid_bit` instance for a specific CPUID feature bit.
 *
 * @param leaf The CPUID leaf number.
 * @param word The register index (EAX, EBX, ECX, or EDX) where the bit resides.
 * @param bit The bit position within the register.
 */
#define CPUID_BIT(leaf, word, bit) ({leaf, word, bit})
/** @} */

/**
 * @defgroup cpuid_features CPUID Feature Macros
 * @brief Macros defining specific CPUID features.
 * @{
 */

// clang-format off

/**
 * @defgroup cpuid_model_features CPUID Model Features
 * @brief Features retrieved from the standard CPUID leaf.
 * @{
 */
#define FEATURE_SSE3 CPUID_BIT(CPUID_MODEL_FEATURES, 2, 0)           ///< SSE3 (Streaming SIMD Extensions 3) support.
#define FEATURE_MON CPUID_BIT(CPUID_MODEL_FEATURES, 2, 3)            ///< MONITOR/MWAIT instructions support.
#define FEATURE_VMX CPUID_BIT(CPUID_MODEL_FEATURES, 2, 5)            ///< Virtual Machine Extensions (VMX) support.
#define FEATURE_TM2 CPUID_BIT(CPUID_MODEL_FEATURES, 2, 8)            ///< Enhanced Thermal Monitoring 2 (TM2) support.
#define FEATURE_SSSE3 CPUID_BIT(CPUID_MODEL_FEATURES, 2, 9)          ///< Supplemental SSE3 (SSSE3) support.
#define FEATURE_PDCM CPUID_BIT(CPUID_MODEL_FEATURES, 2, 15)          ///< Performance and Debug Capability MSR (PDCM) support.
#define FEATURE_PCID CPUID_BIT(CPUID_MODEL_FEATURES, 2, 17)          ///< Process-context identifiers (PCID) support.
#define FEATURE_SSE4_1 CPUID_BIT(CPUID_MODEL_FEATURES, 2, 19)        ///< SSE4.1 support.
#define FEATURE_SSE4_2 CPUID_BIT(CPUID_MODEL_FEATURES, 2, 20)        ///< SSE4.2 support.
#define FEATURE_X2APIC CPUID_BIT(CPUID_MODEL_FEATURES, 2, 21)        ///< x2APIC support.
#define FEATURE_TSC_DEADLINE CPUID_BIT(CPUID_MODEL_FEATURES, 2, 24)  ///< TSC-Deadline Timer support.
#define FEATURE_AESNI CPUID_BIT(CPUID_MODEL_FEATURES, 2, 25)         ///< AES New Instructions (AES-NI) support.
#define FEATURE_XSAVE CPUID_BIT(CPUID_MODEL_FEATURES, 2, 26)         ///< XSAVE and XRSTOR instructions support.
#define FEATURE_OXSAVE CPUID_BIT(CPUID_MODEL_FEATURES, 2, 27)        ///< OS XSAVE support.
#define FEATURE_AVX CPUID_BIT(CPUID_MODEL_FEATURES, 2, 28)           ///< AVX (Advanced Vector Extensions) support.
#define FEATURE_RDRAND CPUID_BIT(CPUID_MODEL_FEATURES, 2, 30)        ///< Random number generator support (RDRAND).
#define FEATURE_HYPERVISOR CPUID_BIT(CPUID_MODEL_FEATURES, 2, 31)    ///< Hypervisor presence indication.
#define FEATURE_FPU CPUID_BIT(CPUID_MODEL_FEATURES, 3, 0)            ///< FPU (Floating Point Unit) presence.
#define FEATURE_SEP CPUID_BIT(CPUID_MODEL_FEATURES, 3, 11)           ///< Fast system call support (SYSENTER/SYSEXIT).
#define FEATURE_CLFLUSH CPUID_BIT(CPUID_MODEL_FEATURES, 3, 19)       ///< Cache line flush instruction (CLFLUSH).
#define FEATURE_ACPI CPUID_BIT(CPUID_MODEL_FEATURES, 3, 22)          ///< Advanced Configuration and Power Interface (ACPI) support.
#define FEATURE_MMX CPUID_BIT(CPUID_MODEL_FEATURES, 3, 23)           ///< MMX technology support.
#define FEATURE_FXSR CPUID_BIT(CPUID_MODEL_FEATURES, 3, 24)          ///< FXSAVE/FXRSTOR instructions support.
#define FEATURE_SSE CPUID_BIT(CPUID_MODEL_FEATURES, 3, 25)           ///< SSE (Streaming SIMD Extensions) support.
#define FEATURE_SSE2 CPUID_BIT(CPUID_MODEL_FEATURES, 3, 26)          ///< SSE2 support.
#define FEATURE_TM CPUID_BIT(CPUID_MODEL_FEATURES, 3, 29)            ///< Thermal Monitor (TM) support.
/** @} */

/**
 * @defgroup cpuid_thermal_features CPUID Thermal and Power Management Features
 * @brief Features related to CPU thermal and power management.
 * @{
 */
#define FEATURE_DTS CPUID_BIT(CPUID_THERMAL_AND_POWER, 0, 0)          ///< Digital Thermal Sensor (DTS) support.
#define FEATURE_TURBO CPUID_BIT(CPUID_THERMAL_AND_POWER, 0, 1)        ///< Turbo Mode support.
#define FEATURE_PLN CPUID_BIT(CPUID_THERMAL_AND_POWER, 0, 4)          ///< Power Limit Notification (PLN) support.
#define FEATURE_PTM CPUID_BIT(CPUID_THERMAL_AND_POWER, 0, 6)          ///< Power Throttle Management (PTM) support.
#define FEATURE_HWP CPUID_BIT(CPUID_THERMAL_AND_POWER, 0, 7)          ///< Hardware-Controlled Performance States (HWP) support.
#define FEATURE_HWP_NOT CPUID_BIT(CPUID_THERMAL_AND_POWER, 0, 8)      ///< HWP Notification support.
#define FEATURE_HWP_ACT CPUID_BIT(CPUID_THERMAL_AND_POWER, 0, 9)      ///< HWP Activity Window support.
#define FEATURE_HWP_PREF CPUID_BIT(CPUID_THERMAL_AND_POWER, 0, 10)    ///< HWP Performance Preference support
#define FEATURE_TURBO_MAX CPUID_BIT(CPUID_THERMAL_AND_POWER, 0, 14)   ///< Turbo Boost Max Technology support.
#define FEATURE_HW_FEEDBACK CPUID_BIT(CPUID_THERMAL_AND_POWER, 2, 0)  ///< Hardware Feedback Interface support.
#define FEATURE_PERF_BIAS CPUID_BIT(CPUID_THERMAL_AND_POWER, 2, 3)    ///< Performance Energy Bias Hint support.
#define FEATURE_AVX512F CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 16)    ///< AVX-512 Foundation support.
#define FEATURE_AVX512DQ CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 17)   ///< AVX-512 Doubleword and Quadword Instructions support.
#define FEATURE_RDSEED CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 18)     ///< RDSEED instruction support.
#define FEATURE_SMAP CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 20)       ///< Supervisor Mode Access Prevention (SMAP).
#define FEATURE_AVX512IFMA CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 21) ///< AVX-512 Integer Fused Multiply-Add support.
#define FEATURE_CLFLUSHOPT CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 23) ///< Cache Line Flush Optimization support.
#define FEATURE_CLWB CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 24) ///< Cache Line Write Back support.
#define FEATURE_PT CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 25)   ///< Processor Trace support.
#define FEATURE_AVX512PF CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 26)   ///< AVX-512 Prefetch support.
#define FEATURE_AVX512ER CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 27)   ///< AVX-512 Exponential and Reciprocal Instructions support.
#define FEATURE_AVX512CD CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 28)   ///< AVX-512 Conflict Detection support.
#define FEATURE_AVX512BW CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 30)   ///< AVX-512 Byte and Word Instructions support.
#define FEATURE_AVX512VL CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 31)   ///< AVX-512 Vector Length Extensions support.
#define FEATURE_AVX512VBMI CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 2, 1)  ///< AVX-512 Vector Byte Manipulation Instructions support.
#define FEATURE_UMIP CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 2, 2)  ///< User-Mode Instruction Prevention support.
#define FEATURE_PKU CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 2, 3)   ///< Protection Keys for User-mode pages (PKU).
#define FEATURE_AVX512VBMI2 CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 2, 6) ///< AVX-512 VBMI2 support.
#define FEATURE_AVX512VNNI CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 2, 11) ///< AVX-512 Vector Neural Network Instructions.
#define FEATURE_AVX512BITALG CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 2, 12) ///< AVX-512 BITALG support.
#define FEATURE_AVX512VPDQ CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 2, 14)   ///< AVX-512 VPDPWSSD and VPDPWSSDS support.
#define FEATURE_AVX512QVNNIW CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 3, 2)  ///< AVX-512 QVNNIW support.
#define FEATURE_AVX512QFMA CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 3, 3)    ///< AVX-512 QFMA support.
#define FEATURE_MD_CLEAR CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 3, 10)     ///< Memory Disambiguation Clear support.
#define FEATURE_IBRS_IBPB CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 3, 26)    ///< Indirect Branch Restriction Speculation (IBRS) and Indirect Branch Prediction Barrier (IBPB) support.
#define FEATURE_STIBP CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 3, 27)        ///< Single Thread Indirect Branch Predictors.
#define FEATURE_L1D_FLUSH CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 3, 28)    ///< L1 Data Cache Flush.
#define FEATURE_ARCH_CAPABILITIES CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 3, 29) ///< Architectural Capabilities MSR support.
#define FEATURE_SSBD CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 3, 31)              ///< Speculative Store Bypass Disable support.
/** @} */

/**
 * @defgroup cpuid_extended_features CPUID Extended Features
 * @brief Extended feature flags retrieved from specific CPUID leaves.
 * @{
 */
#define FEATURE_FSGSBASE CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 0)    ///< FSGSBASE instructions support.
#define FEATURE_TSC_ADJUST CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 1)  ///< TSC Offset Adjust MSR support.
#define FEATURE_AVX2 CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 5)        ///< AVX2 (Advanced Vector Extensions 2) support.
#define FEATURE_SMEP CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 7)        ///< SMEP (Supervisor Mode Execution Protection) support.
#define FEATURE_ERMS CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 9)        ///< Enhanced REP MOVSB/STOSB support.
#define FEATURE_INVPCID CPUID_BIT(CPUID_EXTENDED_FEATURE_FLAGS, 1, 10)    ///< INVPCID instruction support.
#define FEATURE_AMD_TOPO CPUID_BIT(CPUID_FEATS, 2, 22)    ///< AMD topology extensions.
#define FEATURE_SYSCALL CPUID_BIT(CPUID_FEATS, 3, 11)     ///< SYSCALL/SYSRET instructions support.
#define FEATURE_NX CPUID_BIT(CPUID_FEATS, 3, 20)          ///< No-eXecute bit support.
#define FEATURE_HUGE_PAGE CPUID_BIT(CPUID_FEATS, 3, 26)   ///< Huge Pages support.
#define FEATURE_RDTSCP CPUID_BIT(CPUID_FEATS, 3, 27)      ///< RDTSCP instruction support.
#define FEATURE_INVLPGB CPUID_BIT(CPUID_ADDR_WIDTH, 1, 3) ///< INVLPGB instruction support.
/** @} */

// clang-format on

/**
 * @defgroup cpuid_xsave_features XSAVE Features
 * @brief Features related to XSAVE instruction set.
 * @{
 */
#define FEATURE_XSAVEOPT CPUID_BIT(CPUID_XSAVE, 0, 0)  ///< XSAVEOPT instruction support.
#define FEATURE_XSAVEC CPUID_BIT(CPUID_XSAVE, 0, 1)    ///< XSAVEC instruction support.
#define FEATURE_XSAVES CPUID_BIT(CPUID_XSAVE, 0, 3)    ///< XSAVES instruction support.
/** @} */

/** @} */

/**
 * @defgroup cpuid_limits CPUID Limits
 * @brief Defines maximum supported CPUID leaves.
 * @{
 */
#define MAX_SUPPORTED_CPUID (0x17)            ///< Maximum supported standard CPUID leaf.
#define MAX_SUPPORTED_CPUID_EXT (0x8000001e)  ///< Maximum supported extended CPUID leaf.
/** @} */

/**
 * @brief Reads the CPUID leaf and sub-leaf values.
 *
 * This function queries the CPUID instruction for a specific leaf and sub-leaf,
 * and stores the resulting register values in the given `cpuid_leaf` structure.
 *
 * @param[out] leaf Pointer to a `cpuid_leaf` structure to store the result.
 * @param[in] leaf_num The CPUID leaf number to query.
 * @param[in] subleaf_num The CPUID sub-leaf number to query (if applicable).
 * @return `true` if the CPUID query was successful, `false` otherwise.
 */
bool read_cpuid(CpuidLeaf* leaf, uint32_t leaf_num, uint32_t subleaf_num);

/**
 * @brief Tests if a specific CPU feature is supported.
 *
 * This function checks if the CPU supports a particular feature by examining
 * the specified bit in the CPUID leaf.
 *
 * @param bit The `cpuid_bit` structure representing the feature to test.
 * @return `true` if the feature is supported, `false` otherwise.
 */
bool test_feature(CpuidBit bit);

#endif  // CPU_FEATURES_H