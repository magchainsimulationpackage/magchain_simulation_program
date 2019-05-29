SHELL = /bin/bash
COMPILERFLAGS = -v -Wall -O2
CFLAGS = $(COMPILERFLAGS) 
CC = g++ 
EXE = magchain_v2

SOURCES = magchain.cpp aggregate.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(SOURCES) $(OBJECTS) 
	$(CC) $(CFLAGS) -o $(EXE) $(OBJECTS) $(LINUXLIBRARIES)

clean:
	rm $(OBJECTS)
.cpp.o:
	$(CC) $< -c -o $@
