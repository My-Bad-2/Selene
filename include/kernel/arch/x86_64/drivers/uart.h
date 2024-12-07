/**
 * @file uart.h
 * @brief UART driver for serial communication.
 */
#ifndef KERNEL_DRIVERS_UART_H
#define KERNEL_DRIVERS_UART_H 1

#include <stdint.h>

/// @brief Base I/O port address for COM1.
#define COM_PORT1 0x3f8

/// @brief Base I/O port address for COM2.
#define COM_PORT2 0x2f8

/// @brief Base I/O port address for COM3.
#define COM_PORT3 0x3e8

/// @brief Base I/O port address for COM4.
#define COM_PORT4 0x2e8

/**
 * @brief Initializes the UART device on the specified port.
 * @param port Base I/O port address of the UART device.
 */
void uart_initialize(uint16_t port);

/**
 * @brief Sends a character to the specified UART port.
 * @param symbol The character to send.
 * @param port Base I/O port address of the UART device.
 * @details Waits for the transmitter buffer to be empty before sending the
 * character.
 */
void uart_putc(uint8_t symbol, uint16_t port);

#endif  // KERNEL_DRIVERS_UART_H