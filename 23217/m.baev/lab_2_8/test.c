#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


void *func_for_thread(void* arg_list) {
    printf("func for thread %d / %d\n", ((int *) arg_list)[0], ((int *) arg_list)[1]);

    return NULL;
}


int main() {

    pthread_t new_thread;

    int arg_list[2] = {78, 12};

    pthread_create(&new_thread, NULL, func_for_thread, (void *) arg_list);

    pthread_join(new_thread, NULL);

    return 0;
}