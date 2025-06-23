#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


void* thread_function(void* arg) {

    for (int i = 0; i < 10; i++) {
        printf("Поток: строка %d\n", i + 1);
        usleep(100000);
    }

    return NULL;
}


int main() {
    pthread_t thread;

    if (pthread_create(&thread, NULL, thread_function, NULL) != 0) {
        perror("Ошибка при создании потока");
        return 1;
    }

    pthread_join(thread, NULL);

    for (int i = 0; i < 10; i++) {
        printf("Родительский поток: строка %d\n", i + 1);
        usleep(100000);
    }

    return 0;
}