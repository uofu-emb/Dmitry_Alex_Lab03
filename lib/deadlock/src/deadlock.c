#include <zephyr.h>
#include "deadlock.h"

void count_a_b(k_sem_t *sem_a, struct k_sem *sem_b, int *count)
{
    k_sem_take(sem_a, K_FOREVER);
    {
        k_sleep(K_MSEC(100));
        k_sem_take(sem_b, K_FOREVER);
        {
            count++;
        }
        k_sem_give(sem_b);
        
    }
    k_sem_give(sem_a);
}

void count_b_a(k_sem_t *sem_a, k_sem_t *sem_b, int *count)
{
    k_sem_take(sem_b, K_FOREVER);
    {
        k_sleep(K_MSEC(100));
        k_sem_take(sem_a, K_FOREVER);
        {
            count++;
        }
        k_sem_give(sem_a);
    }
    k_sem_give(sem_b);
}