CC ?= gcc
CXX ?= g++
CPP ?= g++


APP_NAME = drm_basic
OBJ_FILES = drm_basic.o

LIBS = -lpthread -ldrm

CFLAGS += -I$(SDKTARGETSYSROOT)/usr/include/libdrm

$(APP_NAME) : $(OBJ_FILES)
	$(CC) $^ -o $@ $(LIBS)


%.o : %.c
	$(CC) -c $^ -o $@ $(CFLAGS)


clean:
	rm *.o $(APP_NAME)
