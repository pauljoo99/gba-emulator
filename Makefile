# Compiler and flags
CXX = g++
CXXFLAGS =-g -std=c++20 -Wall
DEBUG_CXXFLAGS = -g -std=c++20 -Wall -DENABLE_LOGGING

# Source and object files
BUILD_DIR = build
EXEC = $(BUILD_DIR)/emulator
EXEC_CPU_RUNNER = $(BUILD_DIR)/cpu_runner.a

# Default target
all: $(EXEC) $(EXEC_CPU_RUNNER)

# Create CPU Runner library
$(EXEC_CPU_RUNNER): cpu_runner.o
	ar rcs $(EXEC_CPU_RUNNER) $(BUILD_DIR)/cpu_runner.o

# Link object file to create the executable
$(EXEC): main.o snapshot.o arm7tdmi.o
	$(CXX) $(BUILD_DIR)/main.o $(BUILD_DIR)/arm7tdmi.o $(BUILD_DIR)/snapshot.o  -o $(EXEC)

# Compile cpu_runner
cpu_runner.o:
	$(CXX) $(CXXFLAGS) -c cpu_runner.cpp -I. -Iinclude -o $(BUILD_DIR)/cpu_runner.o

# Compile main
main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp -I. -o $(BUILD_DIR)/main.o

# Compile arm
arm7tdmi.o:
	$(CXX) $(CXXFLAGS) -c arm7tdmi.cpp -I. -o $(BUILD_DIR)/arm7tdmi.o

# Compile snapshot
snapshot.o:
	$(CXX) $(CXXFLAGS) -c snapshot.cpp -I. -o $(BUILD_DIR)/snapshot.o

# debug target
debug: CXXFLAGS := $(DEBUG_CXXFLAGS)
debug: $(EXEC)

# make all tests
tests: bitutils_test

# bitutils tests
bitutils_test:
	$(CXX) $(CXXFLAGS) bitutils_test.cpp -I. -o $(BUILD_DIR)/bitutils_test

# Clean target to remove object and executable files
clean:
	rm -f $(BUILD_DIR)/main.o $(BUILD_DIR)/arm7tdmi.o $(BUILD_DIR)/snapshot.o $(EXEC) *.gch

.PHONY: all clean