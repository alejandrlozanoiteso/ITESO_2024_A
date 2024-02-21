#include <stdio.h>
#include <sys/time.h>
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char * argv[]) {


    struct input_event ev;

    printf("Input Test\n");
    int input_fd = open("/dev/input/event0", O_RDWR);
    if (input_fd == -1)
        return -1;

    for (;;) {

        int n_bytes = read(input_fd, &ev, sizeof(struct input_event));
        if (ev.type == EV_KEY) {
            printf("Type: 0x%x\n", ev.type);
            printf("Code: 0x%x\n", ev.code);
            printf("Value: 0x%x\n", ev.value);
        }

    }

    close(input_fd);

    return  0;
}