#include <stdio.h>
#include <zephyr.h>
#include <arch/cpu.h>
#include <sys/printk.h>
#include <unity.h>
#include "hellothread.h"

void setUp(void) {}

void tearDown(void) {}

void test_sem_block(void)
{
    int count = 0;
    struct k_sem semaphore;
    k_sem_init(&semaphore, 1, 1);

    k_sem_take(&semaphore, K_FOREVER);
    int result = update_count(&count, &semaphore, "test", K_NO_WAIT);

    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_INT(0, count);
}

void test_sem_take(void)
{
    int count = 0;
    struct k_sem semaphore;
    k_sem_init(&semaphore, 1, 1);

    int result = update_count(&count, &semaphore, "Hello", K_FOREVER);

    TEST_ASSERT_EQUAL_INT(1, count);
    TEST_ASSERT_EQUAL_INT(0, result);
}

/**************** runner ****************/
int main (void)
{
    UNITY_BEGIN();
    RUN_TEST(test_sem_block);
    RUN_TEST(test_sem_take);
    // RUN_TEST(test_loop_blocks);
    // RUN_TEST(test_loop_runs);
    // RUN_TEST(test_orphaned);
    // RUN_TEST(test_unorphaned);
    return UNITY_END();
}
