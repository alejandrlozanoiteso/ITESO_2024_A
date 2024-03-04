#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <wayland-client.h>

#include <string.h>

#include <syscall.h>
#include <sys/mman.h>

struct wl_display * display;

/*Get Proxy Objects*/
struct wl_compositor *compositor;
struct wl_shm *shm;
struct wl_shell *shell;

/*Global Handlers*/
void registry_global_handler(  void *data, struct wl_registry *registry, 
        uint32_t id, const char *interface, uint32_t version) {

    printf("interface %s, version %d, id: %d\n", interface, version, id);

    if (strcmp(interface, "wl_compositor") == 0) {
        compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
    } else if (strcmp(interface, "wl_shm") == 0) {
        shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
    } else if (strcmp(interface, "wl_shell") == 0) {
        shell = wl_registry_bind(registry, id, &wl_shell_interface, 1);
    }
}


void registry_global_remove_handler(  void *data, struct wl_registry *registry, 
        uint32_t id) {

    printf("Removed %d\n", id);

}
/*Global Singleton Object to notify other objects*/
struct wl_registry * registry;
/*Callbacks*/
struct wl_registry_listener listener = {
    registry_global_handler,
    registry_global_remove_handler
};

int main(int argc, char * argv[]) {

    /*Connect to Server*/
    display = wl_display_connect(NULL);
    
    int color;
    sscanf(argv[1], "0x%x", &color);

    if(display) {
        printf("Connected to Wayland Server\n");
    } else {
        printf("Error Connecting\n");
    }

    /*Get Global Registry Object*/
    registry = wl_display_get_registry(display);
    /*Add Listeners to get*/
    wl_registry_add_listener(registry, &listener, NULL);

    /*Wait for all objects to be listed*/
    wl_display_dispatch(display);
    wl_display_roundtrip(display);

    if ( compositor && shm && shell ) {
        printf("Got all objects\n");
    } else {
        printf("Objects cannot be retrieved\n");
    }

    struct wl_surface *surface = wl_compositor_create_surface(compositor);
    struct wl_shell_surface *shell_surface = wl_shell_get_shell_surface(shell, surface);
    wl_shell_surface_set_toplevel(shell_surface);

    int width = 400;
    int height = 400;
    int stride = width * sizeof(int);
    int size = stride * height;

    // open an anonymous file and write some zero bytes to it
    int fd = syscall(SYS_memfd_create, "buffer", 0);
    //int fd = memfd_create("buffer", 0);
    int ret = ftruncate(fd, size);

    // map it to the memory
    unsigned int *data = (unsigned int *)mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    for(int i = 0 ; i < width * height; i++)
        data[i] = color;

    // turn it into a shared memory pool
    struct wl_shm_pool *pool = wl_shm_create_pool(shm, fd, size);

    // allocate the buffer in that pool
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool,
        0, width, height, stride, WL_SHM_FORMAT_ARGB8888);

    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_commit(surface);

    while (1) {
        wl_display_dispatch(display);
    }

    wl_display_disconnect(display);

    return 0;
}