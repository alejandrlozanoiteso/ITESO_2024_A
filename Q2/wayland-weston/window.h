#ifndef WINDOW_H
#define WINDOW_H

#define MAX_SURFACES 10

typedef int surfaceId;

int wayland_client_init();

surfaceId create_surface(int width, int height, int x, int y);

int draw(surfaceId surface, uint32_t * buff);

#endif