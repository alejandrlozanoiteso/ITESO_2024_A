CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME = i2c_test
APP_OBJ_FILES = i2c_test.o

LIBS = 

all: $(APP_NAME)

$(APP_NAME) : $(APP_OBJ_FILES)
	$(CXX) $^ -o $@  $(LIBS)

%.o : %.cc
	$(CXX) -c $^ -o $@


clean:
	rm *.o $(APP_NAME)