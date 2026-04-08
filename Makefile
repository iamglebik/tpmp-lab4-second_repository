CC = gcc
CFLAGS = -Wall -Wextra -I./includes
LDFLAGS = -lsqlite3
COV_CFLAGS = -fprofile-arcs -ftest-coverage
COV_LDFLAGS = -lgcov

SRC_DIR = src
INC_DIR = includes
BIN_DIR = bin
OBJ_DIR = obj
TEST_DIR = tests

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/autopark

TEST_SOURCES = $(wildcard $(TEST_DIR)/test_*.c)
TEST_NAMES = $(notdir $(TEST_SOURCES))
TEST_NAMES := $(TEST_NAMES:.c=)
TEST_TARGETS = $(addprefix $(BIN_DIR)/, $(TEST_NAMES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/%: $(TEST_DIR)/%.c
	mkdir -p $(BIN_DIR)
	$(CC) $< -o $@ -lsqlite3
	./$@

test: $(TEST_TARGETS)
	@echo "All tests passed"

clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET) $(BIN_DIR)/test_* *.gcda *.gcno *.gcov

run: $(TARGET)
	./$(TARGET)

coverage: CFLAGS += $(COV_CFLAGS)
coverage: LDFLAGS += $(COV_LDFLAGS)
coverage: clean test
	gcov -o $(OBJ_DIR) $(SRC_DIR)/*.c
	@echo "Coverage reports generated"

.PHONY: all clean run test coverage
