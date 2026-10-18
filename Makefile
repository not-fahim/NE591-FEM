CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude

SRC_DIR = src
TEST_DIR = tests
BIN_DIR = bin

# Core source files
CORE_SRC = $(wildcard $(SRC_DIR)/*.cpp)

# Test executables
UNIT_TEST_BIN = $(BIN_DIR)/run_unit_tests
PIPELINE_TEST_BIN = $(BIN_DIR)/run_pipeline_test

all: $(BIN_DIR) $(UNIT_TEST_BIN) $(PIPELINE_TEST_BIN)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(UNIT_TEST_BIN): $(CORE_SRC) $(TEST_DIR)/test_runner.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

$(PIPELINE_TEST_BIN): $(CORE_SRC) $(TEST_DIR)/full_test.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

test: all
	@echo "--- Running Unit Tests ---"
	./$(UNIT_TEST_BIN)
	@echo "--- Running Pipeline Integration Test ---"
	./$(PIPELINE_TEST_BIN)

clean:
	rm -rf $(BIN_DIR)

.PHONY: all test clean
```[cite: 4]
