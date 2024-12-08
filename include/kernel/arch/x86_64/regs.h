/**
 * @file
 * @brief Definitions and masks for CPU control and status registers.
 *
 * This header provides definitions for EFLAGS/RFLAGS, DR6, and DR7 registers,
 * as well as related masks and macros. These are commonly used for debugging,
 * exception handling, and system control in low-level programming.
 *
 * It includes:
 * - Bitwise definitions for EFLAGS/RFLAGS.
 * - Debug register masks (DR6 and DR7) for hardware debugging control.
 * - Relevant notes and usage guidelines for interacting with these registers.
 */
#ifndef CPU_REGISTERS_H
#define CPU_REGISTERS_H 1

/**
 * @defgroup interrupt_stack Interrupt Stack
 * @brief Macros related to interrupt stack configuration.
 * @{
 */
#define INTERRUPT_STACK_SIZE (4096)                ///< Size of the interrupt stack in bytes.
#define PERCPU_INTERRUPT_STACKS_NMI_OFFSET 0x20e0  ///< Offset for per-CPU NMI stacks.
/** @} */

/**
 * @defgroup iframe_offsets Iframe Register Offsets
 * @brief Macros defining offsets within the interrupt frame (iframe).
 * @{
 */
#define IFRAME_OFFSET_RDI (0 * 8)       ///< Offset for the RDI register in iframe.
#define IFRAME_OFFSET_RSI (1 * 8)       ///< Offset for the RSI register in iframe.
#define IFRAME_OFFSET_RBP (2 * 8)       ///< Offset for the RBP register in iframe.
#define IFRAME_OFFSET_RBX (3 * 8)       ///< Offset for the RBX register in iframe.
#define IFRAME_OFFSET_RDX (4 * 8)       ///< Offset for the RDX register in iframe.
#define IFRAME_OFFSET_RCX (5 * 8)       ///< Offset for the RCX register in iframe.
#define IFRAME_OFFSET_RAX (6 * 8)       ///< Offset for the RAX register in iframe.
#define IFRAME_OFFSET_VECTOR (7 * 8)    ///< Offset for the interrupt vector in iframe.
#define IFRAME_OFFSET_ERR_CODE (8 * 8)  ///< Offset for the error code in iframe.
#define IFRAME_OFFSET_RIP (9 * 8)       ///< Offset for the RIP register in iframe.
#define IFRAME_OFFSET_CS (10 * 8)       ///< Offset for the code segment in iframe.
#define IFRAME_OFFSET_RFLAGS (11 * 8)   ///< Offset for the RFLAGS register in iframe.
#define IFRAME_OFFSET_RSP (12 * 8)      ///< Offset for the RSP register in iframe.
#define IFRAME_OFFSET_USER_SS (13 * 8)  ///< Offset for the user segment selector in iframe.
#define IFRAME_SIZE (14 * 8)            ///< Total size of the iframe in bytes.
/** @} */

/**
 * @defgroup control_register_flags Control Register Flags
 * @brief Macros representing control register (CR) flags.
 * @{
 */
#define CR0_PE 0x00000001          ///< Protected mode enable.
#define CR0_MP 0x00000002          ///< Monitor coprocessor.
#define CR0_EM 0x00000004          ///< Emulation mode.
#define CR0_TS 0x00000008          ///< Task-switched flag.
#define CR0_ET 0x00000010          ///< Extension type.
#define CR0_NE 0x00000020          ///< Numeric error.
#define CR0_WP 0x00010000          ///< Write protect.
#define CR0_AM 0x00040000          ///< Alignment mask.
#define CR0_NW 0x20000000          ///< Not writable.
#define CR0_CD 0x40000000          ///< Cache disable.
#define CR0_PG 0x80000000          ///< Paging enabled.
#define CR4_VME 0x00000001         ///< Virtual-8086 mode extensions.
#define CR4_PVI 0x00000002         ///< Protected mode virtual interrupts.
#define CR4_TSD 0x00000004         ///< Time stamp disable.
#define CR4_DE 0x00000008          ///< Debugging extensions.
#define CR4_PSE 0x00000010         ///< Page size extension.
#define CR4_PAE 0x00000020         ///< Physical address extension.
#define CR4_MCE 0x00000040         ///< Machine check enable.
#define CR4_PGE 0x00000080         ///< Page global enable.
#define CR4_PCE 0x00000100         ///< Performance monitoring counter enable.
#define CR4_OSFXSR 0x00000200      ///< Operating system support for FXSAVE and FXRSTOR.
#define CR4_OSXMMEXCPT 0x00000400  ///< OS support for unmasked SIMD floating-point exceptions.
#define CR4_UMIP 0x00000800        ///< User-mode instruction prevention.
#define CR4_LA57 0x00001000        ///< 5-level paging.
#define CR4_VMXE 0x00002000        ///< Virtual machine extensions enable.
#define CR4_SMXE 0x00004000        ///< Safer mode extensions enable.
#define CR4_FSGSBASE 0x00010000    ///< Enables FS and GS base instructions.
#define CR4_PCIDE 0x00020000       ///< PCID enable.
#define CR4_OSXSAVE 0x00040000     ///< XSAVE and processor extended states enable.
#define CR4_SMEP 0x00100000        ///< Supervisor mode execution prevention enable.
#define CR4_SMAP 0x00200000        ///< Supervisor mode access prevention enable.
#define CR4_PKE 0x00400000         ///< Protection key enable.
/** @} */

/**
 * @defgroup efer_registers EFER Registers
 * @brief Extended Feature Enable Register (EFER) flags.
 * @{
 */
#define EFER_SCE 0x00000001  ///< Enable SYSCALL instruction.
#define EFER_LME 0x00000100  ///< Long Mode Enable.
#define EFER_LMA 0x00000400  ///< Long Mode Active.
#define EFER_NXE 0x00000800  ///< Enable Execute-Disable bit.
/** @} */

/**
 * @defgroup msr_registers Model-Specific Registers (MSRs)
 * @brief Definitions and flags for MSRs.
 * @{
 */
#define MSR_PLATFORM_ID 0x00000017              ///< Platform ID register.
#define MSR_APIC_BASE 0x0000001b                ///< APIC base physical address register.
#define MSR_TSC_ADJUST 0x0000003b               ///< TSC Adjust register.
#define MSR_SPEC_CTRL 0x00000048                ///< Speculative Execution Controls.
#define SPEC_CTRL_IBRS (1ull << 0)              ///< Indirect Branch Restricted Speculation.
#define SPEC_CTRL_STIBP (1ull << 1)             ///< Single Thread Indirect Branch Predictors.
#define SPEC_CTRL_SSBD (1ull << 2)              ///< Speculative Store Bypass Disable.
#define MSR_SMI_COUNT 0x00000034                ///< Count of SMI interrupts since boot.
#define MSR_PRED_CMD 0x00000049                 ///< Indirect Branch Prediction Command register.
#define MSR_BIOS_UPDT_TRIG 0x00000079u          ///< Microcode Patch Loader trigger.
#define MSR_BIOS_SIGN_ID 0x0000008b             ///< BIOS update signature register.
#define MSR_MTRRCAP 0x000000fe                  ///< MTRR capability register.
#define MSR_ARCH_CAPABILITIES 0x0000010a        ///< Architectural Capabilities register.
#define ARCH_CAPABILITIES_RDCL_NO (1ull << 0)   ///< No mitigation required for RDCL.
#define ARCH_CAPABILITIES_IBRS_ALL (1ull << 1)  ///< IBRS applies to all code.
#define ARCH_CAPABILITIES_RSBA (1ull << 2)      ///< Robust speculative branch behavior.
#define ARCH_CAPABILITIES_SSB_NO (1ull << 4)    ///< No mitigation required for SSB.
#define ARCH_CAPABILITIES_MDS_NO (1ull << 5)    ///< No mitigation required for MDS.
#define ARCH_CAPABILITIES_TSX_CTRL (1ull << 7)  ///< TSX control available.
#define ARCH_CAPABILITIES_TAA_NO (1ull << 8)    ///< No mitigation required for TAA.
#define MSR_FLUSH_CMD 0x0000010b                ///< L1D cache flush control register.
#define MSR_TSX_CTRL 0x00000122                 ///< TSX instruction enable/disable control.
#define TSX_CTRL_RTM_DISABLE (1ull << 0)        ///< Force all RTM instructions to abort.
#define TSX_CTRL_CPUID_DISABLE (1ull << 1)      ///< Mask RTM and HLE in CPUID.
#define MSR_SYSENTER_CS 0x00000174              ///< SYSENTER CS register.
#define MSR_SYSENTER_ESP 0x00000175             ///< SYSENTER ESP register.
#define MSR_SYSENTER_EIP 0x00000176             ///< SYSENTER EIP register.
#define MSR_MCG_CAP 0x00000179                  ///< Global machine check capability register.
#define MSR_MCG_STATUS 0x0000017a               ///< Global machine check status register.
#define MSR_MISC_ENABLE 0x000001a0  ///< Miscellaneous processor feature enable/disable register.
#define MSR_MISC_ENABLE_TURBO_DISABLE (1ull << 38)  ///< Disable Turbo mode.
#define MSR_TEMPERATURE_TARGET 0x000001a2           ///< Temperature target register.
#define MSR_ENERGY_PERF_BIAS 0x000001b0             ///< Energy/Performance Bias register.
#define MSR_MTRR_PHYSBASE0 0x00000200               ///< MTRR PhysBase0 register.
#define MSR_MTRR_PHYSMASK0 0x00000201               ///< MTRR PhysMask0 register.
#define MSR_MTRR_PHYSMASK9 0x00000213               ///< MTRR PhysMask9 register.
#define MSR_MTRR_DEF_TYPE 0x000002ff                ///< MTRR Default Type register.
#define MSR_MTRR_FIX64K_00000 0x00000250            ///< MTRR FIX64K_00000 register.
#define MSR_MTRR_FIX16K_80000 0x00000258            ///< MTRR FIX16K_80000 register.
#define MSR_MTRR_FIX16K_A0000 0x00000259            ///< MTRR FIX16K_A0000 register.
#define MSR_MTRR_FIX4K_C0000 0x00000268             ///< MTRR FIX4K_C0000 register.
#define MSR_MTRR_FIX4K_F8000 0x0000026f             ///< MTRR FIX4K_F8000 register.
#define MSR_PAT 0x00000277                          ///< PAT register.
#define MSR_TSC_DEADLINE 0x000006e0                 ///< TSC deadline register.
/** @} */

/**
 * @defgroup x2apic_registers x2APIC Registers
 * @brief x2APIC Model-Specific Registers (MSRs) and related functionality.
 * @{
 */

/** @name Identification Registers */
/**@{*/
#define MSR_X2APIC_APICID 0x00000802   ///< x2APIC ID Register (Read-Only).
#define MSR_X2APIC_VERSION 0x00000803  ///< x2APIC Version Register (Read-Only).
/**@}*/

/** @name Task and Priority Registers */
/**@{*/
#define MSR_X2APIC_TPR 0x00000808   ///< x2APIC Task Priority Register (Read/Write).
#define MSR_X2APIC_PPR 0x0000080A   ///< x2APIC Processor Priority Register (Read-Only).
#define MSR_X2APIC_EOI 0x0000080B   ///< x2APIC End-of-Interrupt Register (Write-Only).
#define MSR_X2APIC_LDR 0x0000080D   ///< x2APIC Logical Destination Register (Read-Only).
#define MSR_X2APIC_SIVR 0x0000080F  ///< x2APIC Spurious Interrupt Vector Register (Read/Write).
/**@}*/

/** @name Interrupt Handling Registers */
/**@{*/
/** @brief x2APIC In-Service Registers (ISR) (Read-Only). */
#define MSR_X2APIC_ISR0 0x00000810  ///< ISR Bits 31:0.
#define MSR_X2APIC_ISR1 0x00000811  ///< ISR Bits 63:32.
#define MSR_X2APIC_ISR2 0x00000812  ///< ISR Bits 95:64.
#define MSR_X2APIC_ISR3 0x00000813  ///< ISR Bits 127:96.
#define MSR_X2APIC_ISR4 0x00000814  ///< ISR Bits 159:128.
#define MSR_X2APIC_ISR5 0x00000815  ///< ISR Bits 191:160.
#define MSR_X2APIC_ISR6 0x00000816  ///< ISR Bits 223:192.
#define MSR_X2APIC_ISR7 0x00000817  ///< ISR Bits 255:224.

/** @brief x2APIC Trigger Mode Registers (TMR) (Read-Only). */
#define MSR_X2APIC_TMR0 0x00000818  ///< TMR Bits 31:0.
#define MSR_X2APIC_TMR1 0x00000819  ///< TMR Bits 63:32.
#define MSR_X2APIC_TMR2 0x0000081A  ///< TMR Bits 95:64.
#define MSR_X2APIC_TMR3 0x0000081B  ///< TMR Bits 127:96.
#define MSR_X2APIC_TMR4 0x0000081C  ///< TMR Bits 159:128.
#define MSR_X2APIC_TMR5 0x0000081D  ///< TMR Bits 191:160.
#define MSR_X2APIC_TMR6 0x0000081E  ///< TMR Bits 223:192.
#define MSR_X2APIC_TMR7 0x0000081F  ///< TMR Bits 255:224.

/** @brief x2APIC Interrupt Request Registers (IRR) (Read-Only). */
#define MSR_X2APIC_IRR0 0x00000820  ///< IRR Bits 31:0.
#define MSR_X2APIC_IRR1 0x00000821  ///< IRR Bits 63:32.
#define MSR_X2APIC_IRR2 0x00000822  ///< IRR Bits 95:64.
#define MSR_X2APIC_IRR3 0x00000823  ///< IRR Bits 127:96.
#define MSR_X2APIC_IRR4 0x00000824  ///< IRR Bits 159:128.
#define MSR_X2APIC_IRR5 0x00000825  ///< IRR Bits 191:160.
#define MSR_X2APIC_IRR6 0x00000826  ///< IRR Bits 223:192.
#define MSR_X2APIC_IRR7 0x00000827  ///< IRR Bits 255:224.
/**@}*/

/** @name Error and Command Registers */
/**@{*/
#define MSR_X2APIC_ESR 0x00000828  ///< x2APIC Error Status Register (Read/Write).
#define MSR_X2APIC_ICR 0x00000830  ///< x2APIC Interrupt Command Register (Read/Write).
/**@}*/

/** @name Local Vector Table (LVT) Registers */
/**@{*/
#define MSR_X2APIC_LVT_CMCI 0x0000082F     ///< LVT Corrected Machine Check Interrupt (Read/Write).
#define MSR_X2APIC_LVT_TIMER 0x00000832    ///< LVT Timer Interrupt (Read/Write).
#define MSR_X2APIC_LVT_THERMAL 0x00000833  ///< LVT Thermal Sensor Interrupt (Read/Write).
#define MSR_X2APIC_LVT_PMI 0x00000834      ///< LVT Performance Monitor Interrupt (Read/Write).
#define MSR_X2APIC_LVT_LINT0 0x00000835    ///< LVT LINT0 Interrupt (Read/Write).
#define MSR_X2APIC_LVT_LINT1 0x00000836    ///< LVT LINT1 Interrupt (Read/Write).
#define MSR_X2APIC_LVT_ERROR 0x00000837    ///< LVT Error Interrupt (Read/Write).
/**@}*/

/** @name Timer Registers */
/**@{*/
#define MSR_X2APIC_INIT_COUNT 0x00000838  ///< Initial Timer Count (Read/Write).
#define MSR_X2APIC_CUR_COUNT 0x00000839   ///< Current Timer Count (Read-Only).
#define MSR_X2APIC_DIV_CONF 0x0000083E    ///< Timer Divide Configuration (Read/Write).
/**@}*/

/** @name Self IPI Register */
/**@{*/
#define MSR_X2APIC_SELF_IPI 0x0000083F  ///< Self IPI Register (Write-Only).
/**@}*/

/** @} */  // End of x2apic_registers group

/**
 * @defgroup msr_registers Model-Specific Registers (MSRs)
 * @brief Definitions and descriptions of various MSRs used for system and CPU control.
 * @{
 */

/** @name Extended Feature Enable Registers (EFER) */
/**@{*/
#define MSR_EFER 0xc0000080  ///< Extended Feature Enable Register (EFER).
/**@}*/

/** @name System Call MSRs */
/**@{*/
#define MSR_STAR 0xc0000081   ///< System call address.
#define MSR_LSTAR 0xc0000082  ///< Long mode system call target address.
#define MSR_CSTAR 0xc0000083  ///< IA32 compatibility mode system call target.
#define MSR_FMASK 0xc0000084  ///< System call flag mask.
/**@}*/

/** @name Segment Base MSRs */
/**@{*/
#define MSR_FS_BASE 0xc0000100         ///< Base address for the FS segment.
#define MSR_GS_BASE 0xc0000101         ///< Base address for the GS segment.
#define MSR_KERNEL_GS_BASE 0xc0000102  ///< Kernel GS base address.
/**@}*/

/** @name Time Stamp Counter (TSC) MSRs */
/**@{*/
#define MSR_TSC_AUX 0xc0000103  ///< Auxiliary TSC information.
/**@}*/

/** @name Power Management MSRs */
/**@{*/
#define MSR_PM_ENABLE 0x00000770                ///< HWP enable/disable.
#define MSR_HWP_CAPABILITIES 0x00000771         ///< HWP performance range enumeration.
#define MSR_HWP_REQUEST 0x00000774              ///< HWP power management control hints.
#define MSR_POWER_CTL 0x000001fc                ///< Power Control Register.
#define MSR_RAPL_POWER_UNIT 0x00000606          ///< RAPL unit multipliers.
#define MSR_PKG_POWER_LIMIT 0x00000610          ///< Package power limits.
#define MSR_PKG_ENERGY_STATUS 0x00000611        ///< Package energy consumption.
#define MSR_PKG_POWER_INFO 0x00000614           ///< Package power range information.
#define MSR_DRAM_POWER_LIMIT 0x00000618         ///< DRAM power limit control.
#define MSR_DRAM_ENERGY_STATUS 0x00000619       ///< DRAM energy consumption.
#define MSR_PP0_POWER_LIMIT 0x00000638          ///< PP0 (core) power limit control.
#define MSR_PP0_ENERGY_STATUS 0x00000639        ///< PP0 energy consumption.
#define MSR_PP1_POWER_LIMIT 0x00000640          ///< PP1 (uncore) power limit control.
#define MSR_PP1_ENERGY_STATUS 0x00000641        ///< PP1 energy consumption.
#define MSR_PLATFORM_ENERGY_COUNTER 0x0000064d  ///< Total platform energy counter.
#define MSR_PPERF 0x0000064e                    ///< Productive performance count.
#define MSR_PERF_LIMIT_REASONS 0x0000064f       ///< Performance clipping cause register.
#define MSR_GFX_PERF_LIMIT_REASONS 0x000006b0   ///< Graphics clipping cause register.
#define MSR_PLATFORM_POWER_LIMIT 0x0000065c     ///< Platform-wide power limit control.
/**@}*/

/** @name AMD-Specific MSRs */
/**@{*/
#define MSR_AMD_VIRT_SPEC_CTRL 0xc001011f  ///< AMD speculative execution control.
#define MSR_AMD_F10_DE_CFG 0xc0011029      ///< AMD Family 10h decode config.
#define MSR_AMD_LS_CFG 0xc0011020          ///< Load/store unit configuration.
#define MSR_K7_HWCR 0xc0010015             ///< AMD K7 Hardware Configuration Register.
/**@}*/

/** @name AMD-Specific Feature Flags */
/**@{*/
#define MSR_AMD_F10_DE_CFG_LFENCE_SERIALIZE (1 << 1)  ///< Force LFENCE serialization.
#define AMD_LS_CFG_F15H_SSBD (1ull << 54)     ///< Speculative Store Bypass Disable for Family 15h.
#define AMD_LS_CFG_F16H_SSBD (1ull << 33)     ///< Speculative Store Bypass Disable for Family 16h.
#define AMD_LS_CFG_F17H_SSBD (1ull << 10)     ///< Speculative Store Bypass Disable for Family 17h.
#define MSR_K7_HWCR_CPB_DISABLE (1ull << 25)  ///< Disable turbo (boost).
/**@}*/

/** @name Kernel Virtual Machine (KVM) MSRs */
/**@{*/
#define MSR_KVM_PV_EOI_EN 0x4b564d04          ///< Enable paravirtualized APIC EOI.
#define MSR_KVM_PV_EOI_EN_ENABLE (1ull << 0)  ///< Enable paravirtual fast EOI.
/**@}*/

/** @} */  // End of msr_registers group

/**
 * @defgroup cpu_flags_registers CPU Flags and Debug Registers
 * @brief Definitions for EFLAGS/RFLAGS, DR6, and DR7 registers and their bit masks.
 * @{
 */

/** @name EFLAGS/RFLAGS Bit Definitions */
/**@{*/
#define FLAGS_CF (1 << 0)          ///< Carry Flag.
#define FLAGS_PF (1 << 2)          ///< Parity Flag.
#define FLAGS_AF (1 << 4)          ///< Auxiliary Carry Flag.
#define FLAGS_ZF (1 << 6)          ///< Zero Flag.
#define FLAGS_SF (1 << 7)          ///< Sign Flag.
#define FLAGS_TF (1 << 8)          ///< Trap Flag (enables single-step debugging).
#define FLAGS_IF (1 << 9)          ///< Interrupt Enable Flag.
#define FLAGS_DF (1 << 10)         ///< Direction Flag.
#define FLAGS_OF (1 << 11)         ///< Overflow Flag.
#define FLAGS_STATUS_MASK (0xfff)  ///< Mask for status flags.
#define FLAGS_IOPL_MASK (3 << 12)  ///< I/O Privilege Level Mask.
#define FLAGS_IOPL_SHIFT (12)      ///< I/O Privilege Level Shift.
#define FLAGS_NT (1 << 14)         ///< Nested Task Flag.
#define FLAGS_RF (1 << 16)         ///< Resume Flag.
#define FLAGS_VM (1 << 17)         ///< Virtual-8086 Mode Flag.
#define FLAGS_AC (1 << 18)         ///< Alignment Check Flag.
#define FLAGS_VIF (1 << 19)        ///< Virtual Interrupt Flag.
#define FLAGS_VIP (1 << 20)        ///< Virtual Interrupt Pending.
#define FLAGS_ID (1 << 21)         ///< ID Flag (indicates CPUID instruction support).
#define FLAGS_RESERVED_ONES 0x2    ///< Reserved bits always set to 1.
#define FLAGS_RESERVED 0xffc0802a  ///< Reserved bits mask.
#define FLAGS_USER                                                                         \
  (FLAGS_CF | FLAGS_PF | FLAGS_AF | FLAGS_ZF | FLAGS_SF | FLAGS_TF | FLAGS_DF | FLAGS_OF | \
   FLAGS_NT | FLAGS_AC | FLAGS_ID)  ///< Flags available to user processes.
/**@}*/

/** @name DR6 (Debug Status Register) Bit Definitions */
/**@{*/
#define DR6_B0 (1ul << 0)   ///< Breakpoint condition for debug register 0.
#define DR6_B1 (1ul << 1)   ///< Breakpoint condition for debug register 1.
#define DR6_B2 (1ul << 2)   ///< Breakpoint condition for debug register 2.
#define DR6_B3 (1ul << 3)   ///< Breakpoint condition for debug register 3.
#define DR6_BD (1ul << 13)  ///< Debug register access detected.
#define DR6_BS (1ul << 14)  ///< Single step condition.
#define DR6_BT (1ul << 15)  ///< Task switch condition.

#define DR6_USER_MASK \
  (DR6_B0 | DR6_B1 | DR6_B2 | DR6_B3 | DR6_BD | DR6_BS | DR6_BT)  ///< Writeable bits in DR6.
#define DR6_MASK (0xffff0ff0ul)                                   ///< Mask for valid DR6 bits.

/**
 * @note DR6 is a read-only register used for reporting debug conditions. Writing is ignored.
 * Only bits in @ref DR6_USER_MASK are writeable.
 */
/**@}*/

/** @name DR7 (Debug Control Register) Bit Definitions */
/**@{*/
#define DR7_L0 (1ul << 0)     ///< Local breakpoint enable for register 0.
#define DR7_G0 (1ul << 1)     ///< Global breakpoint enable for register 0.
#define DR7_L1 (1ul << 2)     ///< Local breakpoint enable for register 1.
#define DR7_G1 (1ul << 3)     ///< Global breakpoint enable for register 1.
#define DR7_L2 (1ul << 4)     ///< Local breakpoint enable for register 2.
#define DR7_G2 (1ul << 5)     ///< Global breakpoint enable for register 2.
#define DR7_L3 (1ul << 6)     ///< Local breakpoint enable for register 3.
#define DR7_G3 (1ul << 7)     ///< Global breakpoint enable for register 3.
#define DR7_LE (1ul << 8)     ///< Local exact breakpoint enable.
#define DR7_GE (1ul << 9)     ///< Global exact breakpoint enable.
#define DR7_GD (1ul << 13)    ///< General detect enable.
#define DR7_RW0 (3ul << 16)   ///< Read/write mask for breakpoint 0.
#define DR7_LEN0 (3ul << 18)  ///< Length mask for breakpoint 0.
#define DR7_RW1 (3ul << 20)   ///< Read/write mask for breakpoint 1.
#define DR7_LEN1 (3ul << 22)  ///< Length mask for breakpoint 1.
#define DR7_RW2 (3ul << 24)   ///< Read/write mask for breakpoint 2.
#define DR7_LEN2 (3ul << 26)  ///< Length mask for breakpoint 2.
#define DR7_RW3 (3ul << 28)   ///< Read/write mask for breakpoint 3.
#define DR7_LEN3 (3ul << 30)  ///< Length mask for breakpoint 3.

#define DR7_USER_MASK                                                                           \
  (DR7_L0 | DR7_G0 | DR7_L1 | DR7_G1 | DR7_L2 | DR7_G2 | DR7_L3 | DR7_G3 | DR7_RW0 | DR7_LEN0 | \
   DR7_RW1 | DR7_LEN1 | DR7_RW2 | DR7_LEN2 | DR7_RW3 | DR7_LEN3)  ///< Writeable bits in DR7.

/** @note Even though the GD bit is writable, it is disabled in the write_state syscall
 * for safety and to prevent unintended exceptions. LE and GE bits are typically ignored
 * but recommended to be set for backward compatibility.
 */
#define DR7_MASK ((1ul << 10) | DR7_LE | DR7_GE)  ///< Mask for valid DR7 bits.
/**@}*/

/** @name Hardware Debug Registers */
/**@{*/
#define HW_DEBUG_REGISTERS_COUNT 4  ///< Number of hardware debug registers.
/**@}*/

/** @} */  // End of cpu_flags_registers group

#endif  // CPU_REGISTERS_H