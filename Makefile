CC= gcc
CFLAGS = -Wall -Wextra -Iinclude

SRC = src/Allocator.c src/Debug.c 
TEST= tests/test_allocator.c 

TARGET = test


all:
	$(CC) $(CFLAGS) $(SRC) $(TEST) -o $(TARGET)


clean:
	rm -f $(TARGET)