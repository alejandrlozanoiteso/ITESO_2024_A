#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/gpio.h>

int main(int argc, char **argv)
{
	struct gpiohandle_request req;
	struct gpiohandle_data data;

	/*  Open piochip2*/
	int fd = open("/dev/gpiochip2", 0);

	/* request GPIO line: GPIO_2_0 */
	req.lineoffsets[0] = 0;
	req.flags = GPIOHANDLE_REQUEST_OUTPUT;
	strcpy(req.consumer_label, "led_gpio_2_0");
	req.lines  = 1;
	int ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &req);
    
	/*  Toogle */
	while(1) {

		data.values[0] = !data.values[0];
		ret = ioctl(req.fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);

		sleep(1);
	}

	/*  release line */
	ret = close(req.fd);
	return ret;
}