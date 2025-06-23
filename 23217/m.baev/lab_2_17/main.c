#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


typedef struct Node{
    char *data;
    struct Node* next;
} Node;

Node *head = NULL;
int list_len = 0;
pthread_mutex_t main_mutex;

void print_list() {
    printf("AAAAA\n");
    Node *print_node = head;

    while (print_node != NULL) {
        printf("%s\n", print_node->data);
        print_node = print_node->next;
    }
}

void sort_linked_list() {
    if (!head || !head->next) return;
    for (int i = 0; i < list_len; i++) {
        Node *prev = head;
        for (int j = 0; j < list_len - 1 - i; j++) {
            if (strcmp(prev->data, prev->next->data) > 0) {
                char *temp = prev->next->data;
                prev->next->data = prev->data;
                prev->data = temp;
            }
            prev = prev->next;
        }
    }
}

void *thread_function() {
    while (1) {
        sleep(5);
        pthread_mutex_lock(&main_mutex);
        sort_linked_list();
        pthread_mutex_unlock(&main_mutex);
    }
}

int main() {

    char buffer[1024];

    pthread_t sub_thread;

    if (pthread_mutex_init(&main_mutex, NULL) != 0) {
        perror("Ошибка при инициализации мутекса");
        return 1;
    }
    if (pthread_create(&sub_thread, NULL, thread_function, NULL) != 0) {
        perror("Ошибка при создании потока");
        return 1;
    }

    while (1) {

        printf("Input: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) break;
        size_t len = strlen(buffer);
        printf("%d\n", (int)(len));
        if (buffer[len - 1] == '\n') buffer[len - 1] = '\0';
        pthread_mutex_lock(&main_mutex);
        if (len == 1) {
            print_list();
        } else {
            Node *new_node = malloc(sizeof(Node));
            new_node->data = malloc(len + 1);
            strcpy(new_node->data, buffer);
            new_node->next = head;
            head = new_node;
            list_len++;
        }
        pthread_mutex_unlock(&main_mutex);
    }

    pthread_mutex_destroy(&main_mutex);
    return 0;
}