/**
 * @file lock.h
 * @brief Ticket-based spinlock implementation for mutual exclusion.
 */
#ifndef LOCK_H
#define LOCK_H 1

#include <stdatomic.h>
#include <stdbool.h>

/**
 * @brief Macro to initialize a mutex.
 * @details Creates a mutex with `next_ticket` and `serving_ticket` set to 0, and the data pointer
 * set to `NULL`.
 */
#define INITIALIZE_MUTEX \
  ((struct mutex){       \
      0,                 \
      0,                 \
      NULL,              \
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