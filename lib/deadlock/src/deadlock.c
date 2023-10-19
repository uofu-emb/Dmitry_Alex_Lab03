#include <zephyr.h>
#include "deadlock.h"

void count_a_b(k_sem_t *sem_a, struct k_sem *sem_b, int *count)
{
    k_sem_take(sem_a, K_FOREVER);
    {
        k_sleep(K_MSEC(50));
        k_sem_take(sem_b, K_FOREVER);
        {
            (*count)++;
        }
        k_sem_give(sem_b);
        
    }
    k_sem_give(sem_a);
}

void count_b_a(k_sem_t *sem_a, k_sem_t *sem_b, int *count)
{
    k_sem_take(sem_b, K_FOREVER);
    {
        k_sleep(K_MSEC(50));
        k_sem_take(sem_a, K_FOREVER);
        {
            (*count)++;
        }
        k_sem_give(sem_a);
    }
    k_sem_give(sem_b);
}

void orphaned_lock(k_sem_t* semaphore, int *counter)
{
    while (1) {
        k_sem_take(semaphore, K_FOREVER);
        (*counter)++;
        if ((*counter) % 2) {
            continue;
        }
        printk("Count %d\n", *counter);
        k_sem_give(semaphore);
    }
}