# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -Wextra -Werror -g
# Linker flags
LDFLAGS =

PROG = mall

# Source directory
SRC_DIR = .
# Object directory
OBJ_DIR = obj
# Output directory
OUT_DIR = .

# List of source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# List of object files
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
# Name of the executable
EXEC = $(OUT_DIR)/$(PROG)

# Build target
all: $(EXEC)

# Link object files to create the executable
$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

# Compile each .c file into its own object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf *.out *.o $(PROG) 
