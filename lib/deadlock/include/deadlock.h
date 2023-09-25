#include <zephyr.h>

typedef struct k_sem k_sem_t;
typedef struct k_thread k_thread_t;

void count_a_b(k_sem_t *sem_a, k_sem_t *sem_b, int *count);

void count_b_a(k_sem_t *sem_a, k_sem_t *sem_b, int *count);