CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME = fb_test
OBJFILES = fb_test.o


LIB_DIRS = .

all: $(APP_NAME) 

$(APP_NAME): $(OBJFILES)
	$(CC) $^ -o $@ -L$(LIB_DIRS) $(LIBS)


%.o: %.c
	$(CC) -c $^ -o $@

clean:
	rm *.o  $(APP_NAME)

fresh:
	make clean
	make all 