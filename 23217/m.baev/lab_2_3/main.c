#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void* thread_function(void* arg) {

    char *output = arg;

    printf("%s", output);

    return NULL;
}


int main() {
    pthread_t threads[4];

    int thread_one_error = pthread_create(&threads[0], NULL, thread_function, "Thread one\n");
    int thread_two_error = pthread_create(&threads[1], NULL, thread_function, "Thread two\n");
    int thread_three_error = pthread_create(&threads[2], NULL, thread_function, "Thread three\n");
    int thread_four_error = pthread_create(&threads[3], NULL, thread_function, "Thread four\n");

    if (thread_one_error * thread_two_error * thread_three_error * thread_four_error != 0) {
        perror("Ошибка при создании потока");
        return 1;
    }

    for (int i = 0; i < 4; i ++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < 10; i++) {
        printf("Родительский поток: строка %d\n", i + 1);
        usleep(100000);
    }

    return 0;
}