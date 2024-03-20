CC ?= gcc
CXX ?= g++
CPP ?= g++

APP_NAME = tflite_audio_classification_example
APP_OBJ_FILES = tflite_audio_classification_example.o

LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lopencv_imgcodecs -ltensorflow-lite -ltensorflowlite_flex -lasound -lfftw3

all: $(APP_NAME)

$(APP_NAME) : $(APP_OBJ_FILES)
	$(CXX) $^ -o $@  $(LIBS) -L. -Wl,--no-as-needed

%.o : %.cc
	$(CXX) -c $^ -o $@  -Wl,--no-as-needed


clean:
	rm *.o $(APP_NAME)