#include "internal/uart.h"

#include <log.hpp>

#include <kernel/arch/x86_64/arch.hpp>
#include <kernel/arch/x86_64/drivers/uart.hpp>

void UartDriver::write_reg(uint16_t reg, uint8_t val) const { outp(this->m_port + reg, val); }

uint8_t UartDriver::read_reg(uint16_t reg) const { return inp<uint8_t>(this->m_port + reg); }

void UartDriver::uart_putc(uint8_t symbol) {
  while (!(this->read_reg(UART_LINE_STATUS) & UART_LINE_TRANSMITTER_BUF_EMPTY)) {
    arch_pause();
  }

  this->write_reg(UART_DATA, symbol);
}

void UartDriver::initialize() {
  this->write_reg(UART_INTERRUPT_IDENTIFACTOR, 0);

  // Enable DLAB and set baud rate divisor
  this->write_reg(UART_LINE_CONTROL, UART_LINE_DLAB_STATUS);

  // Set divisor low byte (9600 baud)
  this->write_reg(UART_BAUD_RATE_LOW, 3);

  // Set divisor high byte
  this->write_reg(UART_BAUD_RATE_HIGH, 0);

  // Configure for 8 bits, no parity, and one stop bit
  this->write_reg(UART_LINE_CONTROL, UART_LINE_DS_8);

  // Enable FIFO, clear them with 14-byte threshold
  this->write_reg(UART_FIFO_CONTROLLER, UART_ENABLE_FIFO | UART_FIFO_CLEAR_RECEIVE |
                                            UART_FIFO_CLEAR_TRANSMIT | UART_FIFO_TRIGGER_LEVEL4);

  // Enable IRQs, set RTS/DSR, and set in loopback mode
  this->write_reg(UART_MODEM_CONTROL,
                  UART_MODEM_RTS | UART_MODEM_DTR | UART_MODEM_OUT2 | UART_MODEM_LOOPBACK);

  this->write_reg(UART_MODEM_CONTROL,
                  UART_MODEM_RTS | UART_MODEM_DTR | UART_MODEM_OUT1 | UART_MODEM_OUT2);
}

void UartDriver::shutdown() {}

void UartDriver::debug() const { log_debug("UART driver running on COM PORT 0x%x", this->m_port); }