CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME_1 = file_stream
APP_NAME_2 = file_stream2
OBJS = file_stream.o
OBJS_2 = file_stream2.o

all: $(APP_NAME_1) $(APP_NAME_2)

$(APP_NAME_1) : $(OBJS)
	$(CC) $^ -o $@

$(APP_NAME_2) : $(OBJS_2)
	$(CC) $^ -o $@

%.o : %.c
	$(CC) -c $^ -o $@ -g


clean:
	rm *.o $(APP_NAME_1) $(APP_NAME_2)
