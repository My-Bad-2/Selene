/**
 * @file
 * @brief This file provides definitions and functions for interacting with UART (Universal
 * Asynchronous Receiver/Transmitter) devices.
 *
 * The file includes:
 * - Base I/O port addresses for up to four COM ports (COM1, COM2, COM3, and COM4).
 * - Functions to initialize the UART device and send characters over the specified UART port.
 *
 * The UART functionality allows for communication between the kernel and external devices via
 * serial ports, such as sending debug output or interacting with peripheral devices.
 *
 * @note The functions assume the UART device is already mapped to the specified I/O port.
 */
#ifndef KERNEL_DRIVERS_UART_HPP
#define KERNEL_DRIVERS_UART_HPP 1

#include <cstdint>

#include <lock.hpp>

enum UartComPort : uint16_t {
  PORT1 = 0x3f8,  ///< Base I/O port address for COM1.
  PORT2 = 0x2f8,  ///< Base I/O port address for COM2.
  PORT3 = 0x3e8,  ///< Base I/O port address for COM3.
  PORT4 = 0x2e8,  ///< Base I/O port address for COM4.
};

class UartDriver {
 public:
  explicit UartDriver(UartComPort port) : m_port(port) {}

  void initialize();
  void shutdown();
  void debug() const;

  void uart_putc(uint8_t symbol);

 private:
  void write_reg(uint16_t reg, uint8_t val) const;
  uint8_t read_reg(uint16_t reg) const;

  uint16_t m_port = 0;
  TicketLock m_lock;
};

#endif  // KERNEL_DRIVERS_UART_HPP