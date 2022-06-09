.PHONY: all clean

BIN = loader

CPP = loader.cpp ../filters.cpp ../ppm.cpp ../helper.cpp
OBJ = $(CPP:.cpp=.o)
SRC = $(CPP)

all: loader

clean:
	rm -f $(BIN) $(OBJ)

loader: loader.o ../filters.o ../ppm.o ../helper.o
	g++ -Wall -Wpedantic -g -pthread -o $@ $^

%.o: %.cpp
	g++ -Wall -Wpedantic -g -pthread -c -o $@ $<
