# Ensures commands run in one shell and prevent issues with file names
.ONESHELL:
.PHONY: setup setup-prod build build-prod run run-build-if clean rebuild production help cppcheck lint analyze

# Sets up basic build variables
BUILD_DIR := build
BIN_DIR := $(BUILD_DIR)\bin
JOBS ?= 4
EXECUTABLE := JAKARTA
EXECUTABLE_TEST := JAKARTA_TEST
CLANG_TIDY := clang-tidy
CPPCHECK   := cppcheck

# Sets up cross-platform support
ifeq ($(OS),Windows_NT)
	EXE := .exe
	SEP := \\
	CLEAN := cmd /c del /Q
	EXEC_PREFIX :=
else
	EXE :=
	SEP := /
	CLEAN := rm -rf
	EXEC_PREFIX := ./
endif

# Run once when first initializing project
setup:
	pre-commit install
	cmake -S . -B $(BUILD_DIR) -G Ninja \
	-DCMAKE_C_COMPILER=clang \
	-DCMAKE_CXX_COMPILER=clang++ \
	-DCMAKE_BUILD_TYPE=Debug

# Sets up program to be build for production
setup-prod:
	cmake -S . -B $(BUILD_DIR) -G Ninja -DCMAKE_BUILD_TYPE=Release

# Runs the CMake file
build:
	cmake --build $(BUILD_DIR) --config Debug -- -j$(JOBS)

# Runs the CMake file in production mode
build-prod:
	cmake --build $(BUILD_DIR) --config Release -- -j$(JOBS)

lint:
	cmake --build $(BUILD_DIR) --target clang-tidy

cppcheck:
	cmake --build $(BUILD_DIR) --target cppcheck

analyze: lint cppcheck

# Runs the program: Use this when you make changed
run: build
	@echo Running main executable...
	xcopy "src$(SEP)lib" "$(BIN_DIR)$(SEP)lib" /E /I /Y
	xcopy "test" "$(BIN_DIR)$(SEP)test" /E /I /Y
	cd $(BIN_DIR) && "${EXEC_PREFIX}$(EXECUTABLE)$(EXE)" $(args)

# Same run, but using the test exe
run-test: build
	@echo Running test executable...
	cd $(BIN_DIR) && "${EXEC_PREFIX}$(EXECUTABLE_TEST)$(EXE)" $(args)

# Cleans up the build directory
clean:
	$(CLEAN) $(BUILD_DIR)
	
rebuild: clean setup run
production: clean setup-prod build-prod

help:
	@echo "Available targets:"
	@echo "  setup       - Configure Debug build"
	@echo "  setup-prod  - Configure Release build"
	@echo "  build       - Build Debug build"
	@echo "  build-prod  - Build Release build"
	@echo "  run         - Run Debug main executable"
	@echo "  run-test    - Run Debug test executable"
	@echo "  clean       - Remove build directory"
	@echo "  rebuild     - Clean, setup Debug, build and run"
	@echo "  lint        - Run clang-tidy"
	@echo "  cppcheck    - Run cppcheck"
	@echo "  analyze     - Run both analyzers"
