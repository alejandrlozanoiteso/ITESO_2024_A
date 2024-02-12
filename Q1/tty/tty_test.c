// C library header1
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <sys/ioctl.h> // Used for TCGETS2/TCSETS2, which is required for custom baud rates
#include <asm/termbits.h>

int main (int argc, char * argv[]) {

    uint8_t tx[] = {"ITESO\n"};
    uint8_t rx[sizeof(tx)];
    struct termios2  tty;

    int serial_port = open(argv[1], O_RDWR);

    /*Custom buadrate*/
    ioctl(serial_port, TCGETS2, &tty);

    tty.c_cflag = 0;
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag |= CREAD;
    tty.c_lflag = 0;
    tty.c_iflag = 0;

    tty.c_oflag = 0;

    tty.c_cflag |= CBAUDEX;
    tty.c_ispeed = 115200;
    tty.c_ospeed = 115200;

    ioctl(serial_port, TCSETS2, &tty);

    int res = write(serial_port, tx, sizeof(tx));
    close(serial_port);
}