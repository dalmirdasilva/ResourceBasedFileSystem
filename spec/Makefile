CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-g
SOURCES=main.c rbfs_spec.c rbfs_spec_helper.c ../rbfs.c ../rbfs_make_partition.c ../rbfs_util.c
OBJECTS=$(SOURCES:.cpp=.o)
TARGET=srbfs
INCLUDES=-I./ -I../

all: $(SOURCES) $(TARGET)
    
$(TARGET): $(OBJECTS) 
	$(CC) $(INCLUDES) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(INCLUDES) $(CFLAGS) $< -o $@

clear:
	rm $(TARGET) 
