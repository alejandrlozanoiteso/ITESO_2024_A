CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME = pthread_test
OBJFILES = pthread_test.o

APP_NAME_MUTEX = pthread_mutex_test
OBJFILES_MUTEX = pthread_mutex_test.o

LIB_DIRS = .
LIB = pthread

all: $(APP_NAME) $(APP_NAME_MUTEX) 

$(APP_NAME): $(OBJFILES)
	$(CC) $^ -o $@ -L$(LIB_DIRS) -l$(LIB)

$(APP_NAME_MUTEX): $(OBJFILES_MUTEX)
	$(CC) $^ -o $@ -L$(LIB_DIRS) -l$(LIB)

%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm *.o  $(APP_NAME) $(APP_NAME_MUTEX)

fresh:
	make clean
	make all 