#include <zephyr.h>

void timer_init(struct k_timer *timer, unsigned int timer_delay);

int update_count(int *counter, struct k_sem *semaphore, char *thread_name, k_timeout_t timeout);