NAME = socket_lib

TARGET=lib$(NAME).so
TARGET_EXAMPLE=main

CC=g++

CFLAGS=-c -fPIC -std=c++14 -Wall -Wextra -pedantic -g -O0 -I. -DDEBUG # -Werror


LFLAGS =
LIBDEP = -lz

SOURCES=factory/connection.cpp \
	common/raw_client.cpp \
	common/raw_server.cpp \
	common/tcp_client.cpp \
	common/tcp_server.cpp \
	common/udp_client.cpp \
	common/udp_server.cpp \
	prototype/simple_protocol.cpp \
	prototype/default_connection_params.cpp \
	prototype/network_params.cpp \
	singleton/unique_factory.cpp \
	prototype/prototype_poller.cpp \
	prototype/src/select.cpp

OBJECTS=$(SOURCES:.cpp=.o)


SOURCES_EXAMPLE=example/test_server.cpp example/test_client.cpp example/main.cpp

OBJECTS_EXAMPLE=$(SOURCES_EXAMPLE:.cpp=.o)

LFLAGS_EXAMPLE = -L /tmp
LIBDEP_EXAMPLE = -lsocket_lib -lz -pthread

all: $(SOURCES) $(TARGET)

example: $(SOURCES_EXAMPLE) $(TARGET_EXAMPLE)

$(TARGET): $(OBJECTS)
	$(CC) -shared $(LFLAGS) $(LIBDEP) $(OBJECTS) -o $@
	cp libsocket_lib.so /tmp/

$(TARGET_EXAMPLE): $(OBJECTS_EXAMPLE)
	$(CC) $(LFLAGS_EXAMPLE) $(OBJECTS_EXAMPLE) -o $@ $(LIBDEP_EXAMPLE)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o common/*.o factory/*.o prototype/*.o singleton/*.o example/*.o $(TARGET) $(TARGET_EXAMPLE)
