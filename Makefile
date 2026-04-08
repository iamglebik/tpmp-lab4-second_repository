CC = gcc
CFLAGS = -Wall -Wextra -I./includes
LDFLAGS = -lsqlite3

SRC_DIR = src
INC_DIR = includes
BIN_DIR = bin
OBJ_DIR = obj

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/autopark

SRC_FILES = main.c auth.c menu.c database.c

all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

run: $(TARGET)
	./$(TARGET)

test:
	@echo "=== Running tests ==="
	@echo "No tests yet"

info:
	@echo "Source files: $(SOURCES)"
	@echo "Object files: $(OBJECTS)"

.PHONY: all clean run test info