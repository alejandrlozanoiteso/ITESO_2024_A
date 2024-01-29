#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>

#include "common.h"

#define MAX_LEN 80

int main(int argc, char * argv[]) {
    
    char * buff;
    int ret;

    int shm_id = shmget(SHM_KEY, 4096, IPC_CREAT | 0666);
    if ( shm_id == -1 ) 
        exit(-1);

    buff = shmat(shm_id, 0, 0);

    /*Creemos semáforo*/
    sem_t * sem_wr = sem_open(SEM_WR_NAME, O_CREAT, 0666, 0);
    if ( sem_wr == NULL)
        exit(-1);

    for(;;) {
        char * str = fgets(buff, MAX_LEN, stdin);
        if ( str == NULL )
            break;

        int ret = sem_post(sem_wr);

    }

    ret = sem_unlink(SEM_WR_NAME);

    ret = shmdt(buff);
    if ( ret == 0 )
        printf("Memory dettached\n");

    return 0;
}