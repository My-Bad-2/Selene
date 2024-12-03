#include <kernel/arch/arch.h>
#include <lock.h>

/**
 * @details The function waits until the calling thread's ticket matches the `serving_ticket`. This
 * ensures fair access based on ticket order.
 */
void mutex_lock(struct mutex *mutex)
{
    size_t ticket = atomic_fetch_add_explicit(&mutex->next_ticket, 1, memory_order_relaxed);

    while (atomic_load_explicit(&mutex->serving_ticket, memory_order_acquire) != ticket) {
        arch_pause();
    }
}

/**
 * @details Increments the `serving_ticket` to allow the next waiting thread to acquire the lock.
 */
void mutex_unlock(struct mutex *mutex)
{
    if (mutex_is_locked(mutex)) { return; }

    size_t current = atomic_load_explicit(&mutex->serving_ticket, memory_order_relaxed);
    atomic_store_explicit(&mutex->serving_ticket, current + 1, memory_order_release);
}

/**
 * @details Compares the `next_ticket` and `serving_ticket` to determine whether any thread
 * currently holds the lock.
 */
bool mutex_is_locked(struct mutex *mutex)
{
    size_t current = atomic_load_explicit(&mutex->serving_ticket, memory_order_relaxed);
    size_t next = atomic_load_explicit(&mutex->next_ticket, memory_order_relaxed);

    return current != next;
}

/**
 * @details If the mutex is not locked, it will acquire the lock and return `true`. If the mutex is
 * already locked, it returns `false` immediately.
 */
bool mutex_try_lock(struct mutex *mutex)
{
    if (mutex_is_locked(mutex)) { return false; }

    mutex_lock(mutex);
    return true;
}