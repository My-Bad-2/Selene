#include <log.h>

#include <kernel/arch/x86_64/cpu/exceptions.h>

static void dump_interrupt_frame(struct iframe* iframe) {
  log_trace("CS : 0x%.16lx RIP: 0x%.16lx EFL: 0x%.16lx", iframe->cs, iframe->ip, iframe->flags);
  log_trace("RAX: 0x%.16lx RBX: 0x%.16lx RCX: 0x%.16lx", iframe->rax, iframe->rbx, iframe->rcx);
  log_trace("RDX: 0x%.16lx RSI: 0x%.16lx RBP: 0x%.16lx", iframe->rdx, iframe->rsi, iframe->rbp);
  log_trace("RSP: 0x%.16lx R8 : 0x%.16lx R9 : 0x%.16lx", iframe->rsi, iframe->r8, iframe->r9);
  log_trace("R10: 0x%.16lx R11: 0x%.16lx R12: 0x%.16lx", iframe->r10, iframe->r11, iframe->r12);
  log_trace("R13: 0x%.16lx R14: 0x%.16lx R15: 0x%.16lx", iframe->r13, iframe->r14, iframe->r15);
  log_trace("EC : 0x%.16lx USP: 0x%.16lx USS: 0x%.16lx", iframe->err_code, iframe->user_sp,
            iframe->user_ss);
}

void exception_handler(struct iframe* iframe) {
  dump_interrupt_frame(iframe);
  log_panic("Unhandled Exception %lu!", iframe->vector);
}

void nmi_handler(struct nmi_frame* nmi) {
  dump_interrupt_frame(&nmi->regs);
  log_panic("Unhandled NMI Exception %lu -> %p!", nmi->regs.vector, nmi->expected_gs);
}