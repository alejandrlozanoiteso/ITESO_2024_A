CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME_WRITER = shm_writer
OBJFILES_WRITER = shm_writer.o

APP_NAME_READER = shm_reader
OBJFILES_READER = shm_reader.o

LIB_DIRS = .

all: $(APP_NAME_WRITER) $(APP_NAME_READER)

$(APP_NAME_WRITER): $(OBJFILES_WRITER)
	$(CC) $^ -o $@ -L$(LIB_DIRS)

$(APP_NAME_READER): $(OBJFILES_READER)
	$(CC) $^ -o $@ -L$(LIB_DIRS)

%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm *.o  $(APP_NAME_WRITER)  $(APP_NAME_READER)

fresh:
	make clean
	make all 