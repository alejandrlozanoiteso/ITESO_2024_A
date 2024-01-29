CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME_WRITER = shm_writer
OBJFILES_WRITER = shm_writer.o

APP_NAME_READER = shm_reader
OBJFILES_READER = shm_reader.o

LIB_DIRS = .
LIB = pthread

all: $(APP_NAME_WRITER) $(APP_NAME_READER)

$(APP_NAME_WRITER): $(OBJFILES_WRITER)
	$(CC) $^ -o $@ -L$(LIB_DIRS) -l$(LIB)

$(APP_NAME_READER): $(OBJFILES_READER)
	$(CC) $^ -o $@ -L$(LIB_DIRS) -l$(LIB)

%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm *.o  $(APP_NAME_WRITER)  $(APP_NAME_READER)

fresh:
	make clean
	make all 