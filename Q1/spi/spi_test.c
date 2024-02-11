#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <linux/ioctl.h>
#include <stdint.h>
#include <string.h>

#include <linux/gpio.h>


#define LEN 2

int main(int argc, char * argv[]) {

    int  ret = 0;
    int  fd;
    uint32_t mode = SPI_MODE_1;
    uint8_t  bits = 8;
    uint32_t speed = 1000000;
    uint16_t delay = 0;
    uint8_t tx[LEN] = {0x8F, 0x00}; 
    uint8_t rx[LEN];

    
    fd = open("/dev/spidev1.0", O_RDWR);



    // spi mode
    ret = ioctl(fd, SPI_IOC_WR_MODE32, &mode);

    // bits per word
    ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);

    // max speed hz
    ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = LEN,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		perror("can't send spi message");

    printf("WHO_AM_I: 0x%x\n", rx[1]);

    return 0;
}