CC = g++
CFLAGS = -Wall -g
HEADERS = -I include
ARGS = -std=c++11 -lstdc++

TARGET = test.out

OBJECT = obj/ManageChunk.o obj/MemPool.o obj/Mutex.o obj/test.o

all : $(TARGET)

$(TARGET): $(OBJECT)
	$(CC) $(CFLAGS) $(HEADERS) $(ARGS) $(OBJECT) -o $@

obj/%.o : src/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(HEADERS) $(ARGS) -c $< -o $@



clean:
	rm -f $(TARGET)
	rm -f obj/*.o
