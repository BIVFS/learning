NAME = config_parser

TARGET=lib$(NAME).so

CC=g++

CFLAGS=-c -fPIC -std=c++14 -Wall -Wextra -pedantic -g -O0 -I. # -Werror

LFLAGS =
LIBDEP = 

SOURCES=src/parser.cpp

OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -shared $(LFLAGS) $(LIBDEP) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o src/*.o $(TARGET)
