#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#include <xf86drm.h>
#include <xf86drmMode.h>
#include <string.h>
#include <sys/mman.h>
#include <time.h>

int main(int argc, char * argv[]) {

    drmModeRes *res;    
    drmModeModeInfo mode;
    drmModeConnector *conn;

    int color;
    int fb_id; 
    
    sscanf(argv[1], "0x%x", &color);

    int fd = open("/dev/dri/card1", O_RDWR);
    int conn_id = 35;
    int enc_id = 34;
    int crtc_id = 33;
    int plane_id = 31;

    /*Obtiene Recursos*/
    res = drmModeGetResources(fd);
    printf("Connectors: %d\n", res->count_connectors);
    conn = drmModeGetConnector(fd, res->connectors[0]);
    memcpy(&mode, &conn->modes[0], sizeof(mode));
    printf("hdisplay %d, vdisplay %d\n", mode.hdisplay, mode.vdisplay);

    struct drm_mode_create_dumb creq;
    memset(&creq, 0, sizeof(creq));
	creq.width =  mode.hdisplay;
	creq.height = mode.vdisplay;
	creq.bpp = 32;
	int ret = drmIoctl(fd, DRM_IOCTL_MODE_CREATE_DUMB, &creq);

    /* Crea Frame Buffer */
	ret = drmModeAddFB(fd, creq.width, creq.height, 24, 32, creq.pitch,
			   creq.handle, &fb_id);


    struct drm_mode_map_dumb mreq;
    mreq.handle = creq.handle;
	ret = drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq);

    uint32_t * fb_mem = mmap(0, creq.size, PROT_READ | PROT_WRITE, MAP_SHARED,
		        fd, mreq.offset);

    memset(fb_mem, 0, creq.size);

    drmModeSetCrtc(fd, crtc_id, fb_id, 0, 0,
				     &conn_id, 1, &mode);

	
    for (int j = 0; j <  creq.width * creq.height; ++j) {
            fb_mem[j] = color;
    }

    usleep(5000000);

    memset(fb_mem, 0, creq.size);

    return 0;
}