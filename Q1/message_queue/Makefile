CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME = mqueue_process
OBJFILES = mqueue_process.o

APP_NAME_WRITTER = mqueue_writter
OBJFILES_WRITTER = mqueue_writter.o

LIB_DIRS = .
LIB = rt

all: $(APP_NAME) $(APP_NAME_WRITTER)

$(APP_NAME): $(OBJFILES)
	$(CC) $^ -o $@ -L$(LIB_DIRS) -l$(LIB)

$(APP_NAME_WRITTER): $(OBJFILES_WRITTER)
	$(CC) $^ -o $@ -L$(LIB_DIRS) -l$(LIB)

%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm *.o  $(APP_NAME)  $(APP_NAME_WRITTER)

fresh:
	make clean
	make all 