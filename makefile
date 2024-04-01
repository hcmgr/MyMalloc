# Compiler
CC = gcc
# Compiler flags
# CFLAGS = -Wall -Wextra -Werror -g
CFLAGS = 
# Linker flags
LDFLAGS =

# Executable names
PROG = mall
TEST_PROG = testMall

# Source directory
SRC_DIR = .
# Object directory
OBJ_DIR = obj
# Output directory
OUT_DIR = .
# Test directory
TEST_DIR = test

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# List of object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
# List of test source files
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
# List of object files for tests
TEST_OBJS = $(patsubst $(TEST_DIR)/%.c,$(OBJ_DIR)/test_%.o,$(TEST_SRCS))

# Executable paths
EXEC = $(OUT_DIR)/$(PROG)
TEST = $(TEST_DIR)/$(TEST_PROG)

# Build target
all: $(EXEC)

## Project executable
$(EXEC): $(OBJS) $(TEST_OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/test_%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf *.out *.o $(PROG) $(TEST)
