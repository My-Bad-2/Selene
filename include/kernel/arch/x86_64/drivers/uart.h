/**
 * @file
 * @brief This file provides definitions and functions for interacting with UART (Universal Asynchronous
 *        Receiver/Transmitter) devices.
 * 
 * The file includes:
 * - Base I/O port addresses for up to four COM ports (COM1, COM2, COM3, and COM4).
 * - Functions to initialize the UART device and send characters over the specified UART port.
 * 
 * The UART functionality allows for communication between the kernel and external devices via serial
 * ports, such as sending debug output or interacting with peripheral devices.
 * 
 * @note The functions assume the UART device is already mapped to the specified I/O port.
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