#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdlib.h>
#include "common.h"

#define SIZE    100
#define MSGS    5

int main(int argc, char * argv[]) {

    char msg_buffer[SIZE];
    struct mq_attr attr;

    int priority = atoi(argv[1]);

    attr.mq_maxmsg = MSGS;
    attr.mq_msgsize = SIZE;
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;

    mqd_t msq = mq_open(MQUEUE_NAME, O_CREAT | O_WRONLY, 0666, &attr);
    if (msq  == -1) 
        exit(-1);
    
    for(;;) {
        char * str = fgets(msg_buffer, SIZE, stdin);
        if(str == NULL)
            break;

        int bytes_read = mq_send(msq, msg_buffer, SIZE, priority);
    }

    int ret = mq_close(msq);
    ret = mq_unlink(MQUEUE_NAME);

    return 0;
}