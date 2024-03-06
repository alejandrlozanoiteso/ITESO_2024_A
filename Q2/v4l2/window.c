#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <wayland-client.h>

#include <string.h>

#include <syscall.h>
#include <sys/mman.h>

#include "window.h"

typedef struct  {
    struct wl_surface * surface;
    struct wl_shell_surface * shell_surface;
    uint32_t width;
    uint32_t height;
    uint32_t *data; 
    int fd;
    struct wl_shm_pool *pool;
    struct wl_buffer *buffer;
    uint8_t cb;
} surface_info_t;

struct wl_display * display;
static surface_info_t  surfaces[MAX_SURFACES];
static surfaceId surface_index;

/*Get Proxy Objects*/
static struct wl_compositor *compositor;
static struct wl_shm *shm;
static struct wl_shell *shell;
/*Global Singleton Object to notify other objects*/
static struct wl_registry * registry;

/*Global Handlers*/
void registry_global_handler(  void *data, struct wl_registry *registry, 
        uint32_t id, const char *interface, uint32_t version) {

    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
    } else if (strcmp(interface, "wl_shm") == 0) {
        shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
    } else if (strcmp(interface, "wl_shell") == 0) {
        shell = wl_registry_bind(registry, id, &wl_shell_interface, 1);
    }
}

static void registry_global_remove_handler(  void *data, struct wl_registry *registry, 
        uint32_t id) {
    printf("Removed %d\n", id);
}

/*Callbacks*/
static struct wl_registry_listener listener = {
    registry_global_handler,
    registry_global_remove_handler
};


int wayland_client_init() {

    display = wl_display_connect(NULL);

    /*Get Global Registry Object*/
    registry = wl_display_get_registry(display);
    /*Add Listeners to get*/
    wl_registry_add_listener(registry, &listener, NULL);

    /*Wait for all objects to be listed*/
    wl_display_dispatch(display);
    wl_display_roundtrip(display);

    if ( compositor && shm && shell ) {
        return 0;
    } else {
        return -1;
    }

}

surfaceId create_surface(int width, int height, int x, int y) {
    surfaceId surface_id = surface_index;
    char anonymous_file[10] = "";
    struct wl_surface *surface = wl_compositor_create_surface(compositor);
    struct wl_shell_surface *shell_surface = wl_shell_get_shell_surface(shell, surface);
    wl_shell_surface_set_transient (shell_surface, surface, x, y, 0);
    wl_shell_surface_set_toplevel(shell_surface);

    uint32_t stride = width * sizeof(uint32_t);
    /*Double Buffer*/
    uint32_t size = stride * height;

    snprintf(anonymous_file, 10, "buffer%d", surface_id);
    int fd = syscall(SYS_memfd_create, anonymous_file, 0);
    int ret = ftruncate(fd, size);

    // map it to the memory
    uint32_t *data = (uint32_t *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    // turn it into a shared memory pool
    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);

    // allocate the buffer in that pool
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool,
        0, width, height, stride, WL_SHM_FORMAT_XRGB8888);

    surfaces[surface_id].surface = surface;
    surfaces[surface_id].shell_surface = shell_surface;
    surfaces[surface_id].width = width;
    surfaces[surface_id].height = height;
    surfaces[surface_id].pool = pool;
    surfaces[surface_id].buffer = buffer;
    surfaces[surface_id].data = data;
    surfaces[surface_id].fd = fd;
    surfaces[surface_id].cb = 0;

    surface_index++;
    return surface_id;
}

int draw(surfaceId surface_id, uint32_t * buff) {

    int size = surfaces[surface_id].width * surfaces[surface_id].height;
    
    uint32_t * pixels = surfaces[surface_id].data;
    memcpy(pixels, buff, sizeof(uint32_t) * size);

    wl_surface_attach(surfaces[surface_id].surface, surfaces[surface_id].buffer, 0, 0);
    wl_surface_damage(surfaces[surface_id].surface, 0, 0, surfaces[surface_id].width,  surfaces[surface_id].height);
    wl_surface_commit(surfaces[surface_id].surface);

    wl_display_dispatch(display);

}