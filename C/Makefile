# Compiler
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Source and Object Files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/filter_bank.c
OBJS = $(BUILD_DIR)/main.o $(BUILD_DIR)/filter_bank.o

# Output Executable
TARGET = $(BIN_DIR)/iir_factory

# Default rule
all: $(TARGET)

# Create the executable
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create directories if they don’t exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Rebuild
rebuild: clean all

.PHONY: all clean rebuild
