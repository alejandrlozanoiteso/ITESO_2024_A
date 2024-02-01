#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define PRINT_TIMES 10
#define MAX_THREADS 1
#define MAX_COUNT 50000000

int count;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;

void * my_thread_add(void * arg) {
    
    int ret;
   
    printf("Adding\n");
    for(int i = 0; i < MAX_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        count++;;
        pthread_mutex_unlock(&mutex);
    } 

    pthread_exit(NULL);
}



void * my_thread_sub(void * arg) {

    int ret;
    printf("Substracting\n");
    for(int i = 0; i < MAX_COUNT; i++) {
        pthread_mutex_lock(&mutex);
        count--; 
        pthread_mutex_unlock(&mutex);
    } 

    pthread_exit(NULL);
}

int main(int argc, char * argv[]) {

    pthread_t th_c, th_p;
    int ret;

    printf("Creating Threads\n");

    ret = pthread_create(&th_c, NULL, my_thread_add, NULL);
    ret = pthread_create(&th_p, NULL, my_thread_sub, NULL);
    
    ret = pthread_join(th_c, NULL);
    ret = pthread_join(th_p, NULL);

    printf("Count %d\n", count);
    

    return 0;
}