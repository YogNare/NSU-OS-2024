#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <clock.h>

#define num_steps 200000000
int num_threads;
// #define num_threads 4

typedef struct {
    int thread_id;
    double sum;
    char padding[64];
} ThreadData;

void *thread_function(void *arg) {
    ThreadData *data = (ThreadData *) arg;
    int thread = data->thread_id;
    double a = 0;

    for (int i = num_steps / num_threads * thread; i < num_steps / num_threads * (thread + 1); i++) {
        a += 1.0 / (i * 4.0 + 1.0);
        a -= 1.0 / (i * 4.0 + 3.0);
    }

    data->sum = a;
    return NULL;
}

int main(int argc, char** argv) {

    num_threads = atoi(argv[1]);
    // scanf("%d", &num_threads);
    pthread_t threads_list[num_threads];
    ThreadData thread_data[num_threads];

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;
        thread_data[i].sum = 0;
        if (pthread_create(&threads_list[i], NULL, thread_function, &thread_data[i]) != 0) {
            perror("Ошибка при создании потока");
            return 1;
        }
    }

    double pi = 0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads_list[i], NULL);
        pi += thread_data[i].sum;
    }

    pi = pi * 4.0;

    clock_t finish_time = clock();
    printf("pi done - %.15g | time - %ld\n", pi, finish_time);

    return 0;
}
