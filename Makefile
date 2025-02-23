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
$(EXEC): main.o arm968es.o
	$(CXX) $(BUILD_DIR)/main.o $(BUILD_DIR)/arm968es.o -o $(EXEC)

# Compile main
main.o:
	$(CXX) $(CXXFLAGS) -c main.cpp -I. -o $(BUILD_DIR)/main.o

# Compile main
arm968es.o:
	$(CXX) $(CXXFLAGS) -c arm968es.cpp -I. -o $(BUILD_DIR)/arm968es.o

# debug target
debug: CXXFLAGS += -g
debug: $(EXEC)

# Clean target to remove object and executable files
clean:
	rm -f $(BUILD_DIR)/main.o $(BUILD_DIR)/arm968es.o $(EXEC) *.gch

.PHONY: all clean