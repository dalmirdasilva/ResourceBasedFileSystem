CC=gcc
CFLAGS=-c -Wall
LDFLAGS=-g
SOURCES=main.c srbfs_spec.c srbfs_spec_helper.c ../srbfs.c ../srbfs_init_partition.c ../srbfs_io.c ../srbfs_util.c
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
