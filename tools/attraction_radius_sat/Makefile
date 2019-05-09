SHELL = /bin/bash

COMPILERFLAGS = -Wall -O2
CFLAGS = $(COMPILERFLAGS) 
CC = g++ 

EXE = attradius

SOURCES = attradius.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(EXE): $(SOURCES) $(OBJECTS) 
	$(CC) $(CFLAGS) -o $@ $(OBJECTS)

clean:
	rm $(OBJECTS)
.cpp.o:
	$(CC) $< -c -o $@