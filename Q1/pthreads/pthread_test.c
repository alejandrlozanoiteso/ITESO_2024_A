#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define PRINT_TIMES 10
#define MAX_THREADS 10

void * my_thread(void * arg) {

    for(int i = 0; i < PRINT_TIMES; i ++ ) {
        printf("Hi from  Thread %ld\n", (long) arg);
    }

    printf("Thread %ld exit\n", pthread_self());
    pthread_exit(arg);
}

int main(int argc, char * argv[]) {

    pthread_t th[MAX_THREADS];
    long status;
    int ret;

    for (long i = 0 ; i < MAX_THREADS; i++) {
        ret = pthread_create(&th[i], NULL, my_thread, (void *)i);
    }
   
    for (int i = 0 ; i < MAX_THREADS; i++) {
        ret =  pthread_join(th[i], (void **)&status);
        printf("Thread %ld finished\n", status);
    }

   
    return 0;
}