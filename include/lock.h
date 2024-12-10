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
 * - `mutex`: Represents the ticket-based spinlock.
 * - `mutex_lock`: Acquires the lock, blocking if necessary.
 * - `mutex_unlock`: Releases the lock.
 * - `mutex_try_lock`: Attempts to acquire the lock without blocking.
 * - `mutex_is_locked`: Checks if the mutex is currently locked.
 *
 * @note This implementation uses atomic operations from `<stdatomic.h>` to ensure correctness in a
 * concurrent environment.
 */
#ifndef LOCK_H
#define LOCK_H 1

#include <stdatomic.h>
#include <stdbool.h>

/**
 * @brief Macro to initialize a mutex.
 * @details Creates a mutex with `next_ticket` and `serving_ticket` set to 0, and the data pointer
 * set to `nullptr`.
 */
#define INITIALIZE_MUTEX \
  ((struct mutex){       \
      0,                 \
      0,                 \
      nullptr,              \
  })

/**
 * @struct mutex
 * @brief Represents a ticket-based spinlock for mutual exclusion.
 */
struct mutex {
  atomic_size_t next_ticket;     ///< Tracks the next ticket number.
  atomic_size_t serving_ticket;  ///< Tracks the currently served ticket number.
  void *data;                    ///< Optional user-defined data.
};

/**
 * @brief Acquires the lock on the mutex.
 * @param mutex Pointer to the mutex to acquire.
 */
void mutex_lock(struct mutex *mutex);

/**
 * @brief Releases the lock on the mutex.
 * @param mutex Pointer to the mutex to release.
 */
void mutex_unlock(struct mutex *mutex);

/**
 * @brief Attempts to acquire the lock without blocking.
 * @param mutex Pointer to the mutex to try locking.
 * @return `true` if the lock was successfully acquired; otherwise, `false`.
 */
bool mutex_try_lock(struct mutex *mutex);

/**
 * @brief Checks if the mutex is currently locked.
 * @param mutex Pointer to the mutex to check.
 * @return `true` if the mutex is locked; otherwise, `false`.
 */
bool mutex_is_locked(struct mutex *mutex);

#endif  // LOCK_H