#include "hellothread.h"

void timer_init(struct k_timer *timer, unsigned int timer_delay)
{
    k_timer_init(timer, NULL, NULL);
    k_timer_start(timer, K_MSEC(timer_delay), K_NO_WAIT);
    k_timer_status_sync(timer);
}

int update_count(int *counter, struct k_sem *semaphore, char *thread_name, k_timeout_t timeout)
{
    if (k_sem_take(semaphore, timeout))
        return 1;
    {
        *counter = *counter + 1;
        printk("hello world from %s! Count %d\n", thread_name, *counter);
    }
    k_sem_give(semaphore);
    return 0;
}