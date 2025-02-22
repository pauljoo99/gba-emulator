# Compiler and flags
CXX = g++
CXXFLAGS =-g -std=c++20 -Wall

# Source and object files
BUILD_DIR = build
SRC = main.cpp
OBJ = $(BUILD_DIR)/$(SRC:.cpp=.o)
EXEC = $(BUILD_DIR)/emulator

# Default target
all: $(EXEC)

# Link object file to create the executable
$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC)

# Compile source file to object file
$(OBJ): 
	$(CXX) $(CXXFLAGS) -c $(SRC) -I. -o $(OBJ)

# Clean target to remove object and executable files
clean:
	rm -f $(OBJ) $(EXEC) *.gch

.PHONY: all clean