#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <string.h>

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#define SENSOR_ADDR     0x18

static int read_registers(int fd, uint8_t addr, uint8_t reg, uint8_t * buff, uint8_t size) {
    
    struct i2c_rdwr_ioctl_data msgs[1];
    struct i2c_msg i2cmsg[2];
    int ret;
    uint8_t register_address = reg | (1 << 7);

    /*Send WR Register Address*/
    i2cmsg[0].addr = addr;
    i2cmsg[0].flags = 0;
    i2cmsg[0].len = 1;
    i2cmsg[0].buf = &register_address;

    i2cmsg[1].addr = addr;
    i2cmsg[1].flags = I2C_M_RD; 
    i2cmsg[1].len = size;
    i2cmsg[1].buf = buff;

    msgs[0].msgs = i2cmsg;
    msgs[0].nmsgs = 2;

    if ( (ret = ioctl(fd, I2C_RDWR, msgs)) < 0) {
        perror("ERROR in I2C_RDWR");
        close(fd);
        return -1;
    }

    return 0;
}


static int write_registers(int fd, uint8_t addr, uint8_t reg, uint8_t * buff, uint8_t size) {

    char buffer[size + 1];

    if ( ioctl(fd, I2C_SLAVE, addr) < 0) {
        printf("ERROR in I2C_SLAVE\n");
        close(fd);
        return -1;
    }

    buffer[0] = reg;
    memcpy(&buffer[1], buff, size);

    /*Set Registers*/
    if (write(fd, &buffer, size + 1) < 0 ){
        perror("ERROR in WRITE\n");
        return -1;
    }
        
    return 0;
}


int main(int argc, char * argv[]) {

    uint8_t who_am_i;
    int fd = open("/dev/i2c-1", O_RDWR);

    int ret = read_registers(fd, SENSOR_ADDR, 0xF, &who_am_i, sizeof(who_am_i));
    if (ret < 0)
        return -1;

    printf("WHO_AM_I: 0x%x\n", who_am_i);

    return 0;
}