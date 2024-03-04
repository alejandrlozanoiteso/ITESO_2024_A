CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME_WAYLAND_OPENGL = wayland_opengl
APP_OBJ_FILES__WAYLAND_OPENGL = wayland_opengl.o 

LIBS = -lwayland-client -lwayland-server -lwayland-client -lwayland-egl -lEGL -lGLESv2

all: $(APP_NAME_WAYLAND_OPENGL)

$(APP_NAME_WAYLAND_OPENGL) : $(APP_OBJ_FILES__WAYLAND_OPENGL)
	$(CXX) $^ -o $@  $(LIBS)

%.o : %.cc
	$(CXX) -c $^ -o $@


clean:
	rm *.o  $(APP_NAME_WAYLAND_OPENGL) 