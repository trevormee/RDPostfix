# Compiler and flags
CXX = g++
CXXFLAGS = -Wall 

# Target executable
TARGET = proj1

# Source files
SRCS = main.cpp scanner.cpp parser.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Header files
HEADERS = scanner.hpp parser.hpp

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean