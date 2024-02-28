#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define FBCTL(_fd, _cmd, _arg)  \
    if(ioctl(_fd, _cmd, _arg) == -1) { \
        printf("ioctl failed\n");  \
    }

int main(int argc, char * argv []) {

    int fd;
    int x, y, pos;
    int r, g, b;
    unsigned int  color;
    uint32_t * fbmem;

    sscanf(argv[1], "0x%x", &color);

    printf("0x%x\n", color);

    struct fb_var_screeninfo var_info;
    struct fb_fix_screeninfo fix_info;

    fd = open("/dev/fb0", O_RDWR);
    if(fd < 0) 
        printf("Error opening FB\n");

    FBCTL(fd, FBIOGET_VSCREENINFO, &var_info);
    FBCTL(fd, FBIOGET_FSCREENINFO, &fix_info);

    printf("Frame Buffer Info\n");

    printf("Visible: %d, %d \nvirtual : %d,%d \nline_len %d\n",
            var_info.xres, var_info.yres,
            var_info.xres_virtual, var_info.yres_virtual,
            fix_info.line_length);


    fbmem = (uint32_t *)mmap(0, var_info.yres * fix_info.line_length, \
                PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);

    if (fbmem == 0)
        printf("Map failed\n");

    int zero = 0;
    ioctl(fd, FBIO_WAITFORVSYNC, &zero);
    
    for(int i = 0; i < var_info.yres * var_info.xres; i++  ) {
        *(fbmem + i) = color;
    }

    munmap((void *)fbmem, var_info.yres * fix_info.line_length );
    return 0;
}