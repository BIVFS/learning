NAME = socket_lib

TARGET=lib$(NAME).so

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
	singleton/unique_factory.cpp

OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -shared $(LFLAGS) $(LIBDEP) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o common/*.o factory/*.o prototype/*.o singleton/*.o $(TARGET)
