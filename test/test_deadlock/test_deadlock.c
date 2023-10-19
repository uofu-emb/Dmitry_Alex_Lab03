#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>
#include <unity.h>
#include "deadlock.h"

#define STACKSIZE 2000

typedef struct k_thread k_thread_t;
K_THREAD_STACK_DEFINE(thread_a_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(thread_b_stack, STACKSIZE);
K_THREAD_STACK_DEFINE(thread_stack, STACKSIZE);

k_sem_t sem_a, sem_b;
k_thread_t thread_a, thread_b;
int counter;

void setUp(void) {}

void tearDown(void) {}

void thread_a_run(void)
{
    count_a_b(&sem_a, &sem_b, &counter);
}

void thread_b_run(void)
{
    count_b_a(&sem_a, &sem_b, &counter);
}

void test_deadlock_occurs(void)
{
    counter = 0;
    k_sem_init(&sem_a, 1, 1);
    k_sem_init(&sem_b, 1, 1);

    
    
    k_thread_create(&thread_a,
                    thread_a_stack,
                    STACKSIZE,
                    (k_thread_entry_t) thread_a_run,
                    NULL,
                    NULL,
                    NULL,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);

    k_thread_create(&thread_b,
                    thread_a_stack,
                    STACKSIZE,
                    (k_thread_entry_t) thread_b_run,
                    NULL,
                    NULL,
                    NULL,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);
    // k_sleep(K_MSEC(200));

    int a_status = k_thread_join(&thread_a, K_NO_WAIT);
    int b_status = k_thread_join(&thread_b, K_NO_WAIT);

    TEST_ASSERT_EQUAL_INT(-EBUSY, a_status);
    TEST_ASSERT_EQUAL_INT(-EBUSY, b_status);
    
    k_thread_abort(&thread_a);
    k_thread_abort(&thread_b);

    TEST_ASSERT_EQUAL_INT(0, k_sem_count_get(&sem_a));
    TEST_ASSERT_EQUAL_INT(0, k_sem_count_get(&sem_b));

    TEST_ASSERT_EQUAL_INT(0, counter);
}

void test_orphan_deadlock(void)
{
    int counter = 0;
    k_thread_t thread;
    k_sem_t semaphore;
    k_sem_init(&semaphore, 1, 1);
    k_thread_create(&thread,
                    thread_stack,
                    STACKSIZE,
                    (k_thread_entry_t) orphaned_lock,
                    &semaphore,
                    &counter,
                    NULL,
                    K_PRIO_COOP(7),
                    0,
                    K_NO_WAIT);

    int status = k_thread_join(&thread, K_NO_WAIT);

    TEST_ASSERT_EQUAL_INT(-EBUSY, status);

    k_thread_abort(&thread);
}


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_deadlock_occurs);
    RUN_TEST(test_orphan_deadlock);
    return UNITY_END();
}