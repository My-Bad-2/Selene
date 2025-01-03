/**
 * @file
 * @brief Provides a ticket-based spinlock implementation for mutual exclusion.
 *
 * This file defines a simple lock mechanism using a ticket-based spinlock (`mutex`), which ensures
 * mutual exclusion for concurrent processes. The mutex is implemented using atomic operations for
 * efficient locking in a multi-core environment. It also includes several utility functions to
 * acquire, release, and check the status of the lock.
 *
 * The ticket-based approach ensures fair access to the lock, where each thread or process receives
 * a unique ticket and is served in the order of ticket numbers.
 *
 * Key components:
 * - `TicketLock`: Represents the ticket-based spinlock.
 * - `TicketLock::lock`: Acquires the lock, blocking if necessary.
 * - `TicketLock::unlock`: Releases the lock.
 * - `TicketLock::try_lock`: Attempts to acquire the lock without blocking.
 * - `TicketLock::is_locked`: Checks if the mutex is currently locked.
 *
 * @note This implementation uses atomic operations from `<atomic>` to ensure correctness in a
 * concurrent environment.
 */
#ifndef LOCK_H
#define LOCK_H 1

#include <atomic>
#include <cstddef>

#include <kernel/arch/arch.hpp>

class TicketLock {
 public:
  TicketLock() = default;

  /**
   * @brief Acquires the lock on the mutex.
   *
   * @details The function waits until the calling thread's ticket matches the `m_serving_ticket`.
   * This ensures fair access based on ticket order.
   */
  void lock() {
    size_t ticket = this->m_next_ticket.fetch_add(1, std::memory_order_relaxed);

    while (this->m_serving_ticket.load(std::memory_order_acquire) != ticket) {
      arch_pause();
    }
  }

  /**
   * @brief Releases the lock on the mutex.
   *
   * @details Increments the `serving_ticket` to allow the next waiting thread to acquire the lock.
   */
  void unlock() {
    if (this->is_locked()) {
      return;
    }

    this->m_serving_ticket.fetch_add(1, std::memory_order_relaxed);
  }

  /**
   * @brief Checks if the mutex is currently locked.
   *
   * @details Compares the `m_next_ticket` and `m_serving_ticket` to determine whether any thread
   * currently holds the lock.
   *
   * @return `true` if the mutex is locked; otherwise, `false`.
   */
  bool is_locked() {
    size_t current = this->m_serving_ticket.load(std::memory_order_relaxed);
    size_t next = this->m_next_ticket.load(std::memory_order_relaxed);

    return current != next;
  }

  /**
   * @brief Attempts to acquire the lock without blocking.
   *
   * @details If the mutex is not locked, it will acquire the lock and return `true`. If the mutex
   * is already locked, it returns `false` immediately.
   *
   * @return `true` if the lock was successfully acquired; otherwise, `false`.
   */
  bool try_lock() {
    if (this->is_locked()) {
      return false;
    }

    this->lock();
    return true;
  }

 private:
  std::atomic<size_t> m_next_ticket;     ///< Tracks the next ticket number.
  std::atomic<size_t> m_serving_ticket;  ///< Tracks the currently served ticket number.
};

#endif  // LOCK_H