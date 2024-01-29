#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include "common.h"

#define MAX_LEN 80

int main(int argc, char * argv[]) {
    
    char * buff;
    int ret;

    int shm_id = shmget(SHM_KEY, 0, 0);
    if ( shm_id == -1) 
        exit(-1);

    buff = shmat(shm_id, 0, 0);

    sem_t * sem_wr = sem_open(SEM_WR_NAME, O_CREAT, 0666);
    if ( sem_wr == NULL)
        exit(-1);


    for (;;) {
        int ret = sem_wait(sem_wr);
        printf("%s", buff);
    }
  
    ret = sem_unlink(SEM_WR_NAME);

    ret = shmdt(buff);
    if ( ret == 0 )
        printf("Memory dettached\n");

    return 0;
}