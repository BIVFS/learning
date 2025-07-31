NAME = socket_lib

TARGET=lib$(NAME).so

CC=g++

CFLAGS=-c -fPIC -std=c++14 -Wall -Wextra -pedantic -g -O0 -I. -DDEBUG # -Werror

LFLAGS =
LIBDEP = -lz

SOURCES=src/connection.cpp \
	src/raw_client.cpp \
	src/raw_server.cpp \
	src/tcp_client.cpp \
	src/tcp_server.cpp \
	src/udp_client.cpp \
	src/udp_server.cpp \
	src/simple_protocol.cpp \
	src/default_connection_params.cpp \
	src/prototypes/network_params.cpp \
	singleton/src/unique_factory.cpp \
	prototype/src/prototype_poller.cpp


OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -shared $(LFLAGS) $(LIBDEP) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o src/*.o $(TARGET)
