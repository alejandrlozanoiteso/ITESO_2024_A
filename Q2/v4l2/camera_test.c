#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>

#include "window.h"

#define  RGB(v) ({    \
			int value = (v); \
			(value > 0) * value | (255 * (value > 255));\
		})

void yuyvtorgb32(unsigned char *yuyv, unsigned char *dst)
{
	int r0, g0, b0;
	int r1, g1, b1;
	int y0, y1, u, v;
	char *src;

	src = (char *)yuyv;
	y0 = *(src + 0);
	u = *(src + 1);
	y1 = *(src + 2);
	v = *(src + 3);

	u = u - 128;
	v = v - 128;
	r0 = RGB(y0 + v + (v >> 2) + (v >> 3) + (v >> 5));
	g0 = RGB(y0 - ((u >> 2) + (u >> 4) + (u >> 5)) - (v >> 1) + (v >> 3) + (v >> 4) + (v >> 5));
	b0 = RGB(y0 + u + (u >> 1) + (u >> 2) + (u >> 6));

	r1 = RGB(y1 + v + (v >> 2) + (v >> 3) + (v >> 5));
	g1 = RGB(y1 - ((u >> 2) + (u >> 4) + (u >> 5)) - (v >> 1) + (v >> 3) + (v >> 4) + (v >> 5));
	b1 = RGB(y1 + u + (u >> 1) + (u >> 2) + (u >> 6));

	*(dst) = b0;
	*(dst + 1) = g0;
	*(dst + 2) = r0;

	*(dst + 4) = b1;
	*(dst + 5) = g1;
	*(dst + 6) = r1;
}

void software_csc(unsigned char *inbuf, unsigned char *outbuf, int xres, int yres)
{
	unsigned char *yuv;
	unsigned char *rgb;
	int x;


    for (x = 0; x < xres*yres/2; x++) {
        yuv = inbuf + x*4;
        rgb = outbuf + x*8;
        yuyvtorgb32(yuv, rgb);
    }

}

#define MODE 	2
#define WIDTH 	1280
#define HEIGHT 720
#define BUFFER_NUM 3

struct testbuffer
{
	unsigned char *start;
	size_t offset;
	unsigned int length;
};

struct testbuffer buffers[BUFFER_NUM];

int main(int argc, char * argv[]) {

	enum v4l2_buf_type type;

    int ret = wayland_client_init();
    surfaceId surface = create_surface(WIDTH, HEIGHT, 0, 0);

    uint32_t * image = (uint32_t *)malloc(WIDTH * HEIGHT * sizeof(uint32_t));

    /*Open Device*/
    int fd = open("/dev/video0", O_RDWR);
	
    /*Setup Capture Device*/
	/*Set Rate*/
    struct v4l2_streamparm parm;
	memset(&parm, 0, sizeof(parm));
    parm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	parm.parm.capture.capturemode = MODE;
	parm.parm.capture.timeperframe.denominator = 30;
	parm.parm.capture.timeperframe.numerator = 1;
	if (ioctl(fd, VIDIOC_S_PARM, &parm) < 0)
	{
		perror("VIDIOC_S_PARM failed\n");
		return -1;
	}


  	/*Set Format*/
	struct v4l2_frmsizeenum frmsize;
	memset(&frmsize, 0, sizeof(frmsize));
	frmsize.pixel_format = V4L2_PIX_FMT_YUYV;
	frmsize.index = MODE;
	if (ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize) < 0) {
		perror("VIDIOC_ENUM_FRAMESIZES\n");
	}

  
    struct v4l2_format format;
    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    format.fmt.pix.width = WIDTH;
    format.fmt.pix.height = HEIGHT;
    
    if(ioctl(fd, VIDIOC_S_FMT, &format) < 0){
        perror("VIDIOC_S_FMT");
       return -1;
    }

    /*Request Buffers*/
    struct v4l2_requestbuffers bufrequest;
    memset(&bufrequest, 0, sizeof (bufrequest));
    bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufrequest.memory = V4L2_MEMORY_MMAP;
    bufrequest.count = BUFFER_NUM;
    
    if(ioctl(fd, VIDIOC_REQBUFS, &bufrequest) < 0){
        perror("VIDIOC_REQBUFS");
        return -1;
    }

	/*Query Buffers*/
    struct v4l2_buffer buf;
    for (int i = 0; i < BUFFER_NUM; i++) {
        memset(&buf, 0, sizeof (buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;

        if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
            perror("VIDIOC_QUERYBUF error\n");
            return -1;
        }

        buffers[i].length = buf.length;
        buffers[i].offset = (size_t) buf.m.offset;
        buffers[i].start = mmap(NULL, buffers[i].length,
            PROT_READ | PROT_WRITE, MAP_SHARED,
            fd, buffers[i].offset);

    }
	
	/*Queue Buffers and Start Streaming*/
	for (int i = 0; i < BUFFER_NUM; i++)
	{
		memset(&buf, 0, sizeof (buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = i;
		buf.length = buffers[i].length;
        buf.m.offset = buffers[i].offset;

		if (ioctl (fd, VIDIOC_QBUF, &buf) < 0) {
			perror("VIDIOC_QBUF error\n");
			return -1;
		}
	}

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if (ioctl (fd, VIDIOC_STREAMON, &type) < 0) {
		perror("VIDIOC_STREAMON error\n");
		return -1;
	}

    for (;;) {

        memset(&buf, 0, sizeof (buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		if (ioctl (fd, VIDIOC_DQBUF, &buf) < 0) {
			perror("VIDIOC_DQBUF failed.\n");
			break;
		}
       
        //software_csc(buffers[buf.index].start, (unsigned char *)image, WIDTH, HEIGHT);
		memcpy(image, buffers[buf.index].start, buffers[buf.index].length);
        ret = draw(surface, image);
		

        if (ioctl (fd, VIDIOC_QBUF, &buf) < 0) {
			perror("VIDIOC_QBUF failed\n");
			break;
		}

    }


    type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	ioctl (fd, VIDIOC_STREAMOFF, &type);

    return 0;
}