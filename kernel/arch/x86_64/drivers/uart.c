#include <kernel/arch/x86_64/arch.h>
#include <kernel/arch/x86_64/drivers/uart.h>

#include "internal/uart.h"

void uart_write_reg(uint16_t port, uint16_t reg, uint8_t val) { outp(port + reg, val); }

uint8_t uart_read_reg(uint16_t port, uint16_t reg)
{
    uint8_t val = 0;
    inp(port + reg, &val);
    return val;
}

/**
 * @details Waits for the UART's transmitter buffer to be empty before writing
 * the character.
 */
void uart_putc(uint8_t symbol, uint16_t port)
{
    while (!(uart_read_reg(port, UART_LINE_STATUS) & UART_LINE_TRANSMITTER_BUF_EMPTY)) {
        arch_pause();
    }

    uart_write_reg(port, UART_DATA, symbol);
}

/**
 * @details Configures the UART device for 8 bits, no parity, one stop bit,
 * enables FIFOs, clears them, and sets baud rate divisor.
 */
void uart_initialize(uint16_t port)
{
    uart_write_reg(port, UART_INTERRUPT_IDENTIFACTOR, 0);

    // Enable DLAB and set baud rate divisor
    uart_write_reg(port, UART_LINE_CONTROL, UART_LINE_DLAB_STATUS);

    // Set divisor low byte (9600 baud)
    uart_write_reg(port, UART_BAUD_RATE_LOW, 3);

    // Set divisor high byte
    uart_write_reg(port, UART_BAUD_RATE_HIGH, 0);

    // Configure for 8 bits, no parity, and one stop bit
    uart_write_reg(port, UART_LINE_CONTROL, UART_LINE_DS_8);

    // Enable FIFO, clear them with 14-byte threshold
    uart_write_reg(port,
      UART_FIFO_CONTROLLER,
      UART_ENABLE_FIFO | UART_FIFO_CLEAR_RECEIVE | UART_FIFO_CLEAR_TRANSMIT
        | UART_FIFO_TRIGGER_LEVEL4);

    // Enable IRQs, set RTS/DSR, and set in loopback mode
    uart_write_reg(port,
      UART_MODEM_CONTROL,
      UART_MODEM_RTS | UART_MODEM_DTR | UART_MODEM_OUT2 | UART_MODEM_LOOPBACK);

    uart_write_reg(port,
      UART_MODEM_CONTROL,
      UART_MODEM_RTS | UART_MODEM_DTR | UART_MODEM_OUT1 | UART_MODEM_OUT2);
}