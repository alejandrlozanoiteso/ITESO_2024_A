#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stdint.h>
#include <wayland-client.h>
#include "window.h"

#define WIDTH 1280
#define HEIGHT 720
#define X_POS 0
#define Y_POS 0


extern struct wl_display * display;

int main(int argc, char * argv[]) {

    int ret = wayland_client_init();
    surfaceId surface = create_surface(WIDTH, HEIGHT, X_POS, Y_POS);

    uint32_t * image = (uint32_t *)malloc(WIDTH * HEIGHT * sizeof(uint32_t));

    for (;;) {

        for (int i = 0 ; i < (WIDTH * HEIGHT); i++) 
            image[i] = 0xFFFF0000;

        ret = draw(surface, image);
        sleep(1);
        
        for (int i = 0 ; i < (WIDTH * HEIGHT); i++)
            image[i] = 0xFF0000FF;

        ret = draw(surface, image);
        sleep(1);
    }

    return 0;
}