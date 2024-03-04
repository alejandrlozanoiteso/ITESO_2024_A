CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME_V4L2 = wayland_basic
APP_OBJ_FILES_V4L2 = wayland_basic.o

LIBS = -lwayland-client

all: $(APP_NAME_V4L2)

$(APP_NAME_V4L2) : $(APP_OBJ_FILES_V4L2)
	$(CXX) $^ -o $@  $(LIBS)

%.o : %.cc
	$(CXX) -c $^ -o $@


clean:
	rm *.o  $(APP_NAME_V4L2)