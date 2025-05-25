# Compiler and flags
CXX = g++
CXXFLAGS =-g -std=c++20 -Wall
DEBUG_CXXFLAGS = -g -std=c++20 -Wall

# Source and object files
BUILD_DIR = build
EXEC = $(BUILD_DIR)/emulator

# Default target
all: $(EXEC)

# Link object file to create the executable
$(EXEC): main.o snapshot.o arm7tdmi.o
	$(CXX) $(BUILD_DIR)/main.o $(BUILD_DIR)/arm7tdmi.o $(BUILD_DIR)/snapshot.o  -o $(EXEC)

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
debug: CXXFLAGS += -g
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