CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC_DIR = src
TEST_DIR = tests
BIN_DIR = bin

# Sources
CORE_SRC = $(SRC_DIR)/mesh.cpp $(SRC_DIR)/input_parser.cpp
TEST_SRC = $(TEST_DIR)/test_runner.cpp

# Executables
TEST_BIN = $(BIN_DIR)/run_tests

all: $(BIN_DIR) $(TEST_BIN)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TEST_BIN): $(CORE_SRC) $(TEST_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BIN_DIR)

.PHONY: all test clean
