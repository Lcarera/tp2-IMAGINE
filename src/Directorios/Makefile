.PHONY: all clean

BIN = mainDir

CPP = ./mainDir.cpp ./filters.cpp ./ppm.cpp ./helper.cpp
OBJ = $(CPP:.cpp=.o)
SRC = $(CPP)

all: main

clean:
	rm -f $(BIN) $(OBJ)

main: mainDir.o filters.o ppm.o helper.o
	g++ -Wall -Wpedantic -g -pthread -o $@ $^

%.o: %.cpp
	g++ -Wall -Wpedantic -g -pthread -c -o $@ $<
