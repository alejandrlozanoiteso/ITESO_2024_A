CC ?= gcc
CXX ?= g++
CPP ?= g++

OBJFILES_SIMPLE = pipe.o 
OBJFILES_PROCESS = pipe_process.o 
APP_NAME_SIMPLE_PIPE = pipe_example
APP_NAME_PROCESS = pipe_example_process


OBJFILES_WRITER = writer_pipe.o 
OBJFILES_READER = reader_pipe.o 
APP_NAME_WRITER = pipe_writer
APP_NAME_READER = pipe_reader

LIB_DIRS = .

all: $(APP_NAME_SIMPLE_PIPE) $(APP_NAME_PROCESS) $(APP_NAME_WRITER) $(APP_NAME_READER)

$(APP_NAME_SIMPLE_PIPE) : $(OBJFILES_SIMPLE)
	$(CC) -o $@  $^ -L$(LIB_DIRS)

$(APP_NAME_PROCESS) : $(OBJFILES_PROCESS)
	$(CC) -o $@  $^ -L$(LIB_DIRS)

$(APP_NAME_WRITER) : $(OBJFILES_WRITER)
	$(CC) -o $@  $^ -L$(LIB_DIRS)

$(APP_NAME_READER) : $(OBJFILES_READER)
	$(CC) -o $@  $^ -L$(LIB_DIRS)

%.o : %.c
	$(CC) -c $^ -o $@

clean: 
	rm *.o $(APP_NAME_SIMPLE_PIPE) $(APP_NAME_PROCESS) $(APP_NAME_WRITER) $(APP_NAME_READER)

fresh:
	make clean
	make all 