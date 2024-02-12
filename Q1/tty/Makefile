CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME = tty_test
APP_OBJ_FILES = tty_test.o

APP_NAME_RX = tty_test_rx
APP_OBJ_FILES_RX = tty_test_rx.o

LIBS = 

all: $(APP_NAME) $(APP_NAME_RX)

$(APP_NAME) : $(APP_OBJ_FILES)
	$(CXX) $^ -o $@  $(LIBS)

$(APP_NAME_RX) : $(APP_OBJ_FILES_RX)
	$(CXX) $^ -o $@  $(LIBS)

%.o : %.cc
	$(CXX) -c $^ -o $@


clean:
	rm *.o $(APP_NAME) $(APP_NAME_RX)