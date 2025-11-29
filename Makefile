# Makefile for CS 454 Signature Project

CXX = g++
CXXFLAGS = -std=c++20 -I. -Ithird_party -Wall
TARGET = MyProject
TEST_TARGET = tests
SOURCES = main.cpp DFA.cpp RecurrenceEquationBuilder.cpp RecurrenceEquationSolver.cpp
COMMON_SOURCES = DFA.cpp RecurrenceEquationBuilder.cpp RecurrenceEquationSolver.cpp
OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = tests.o $(COMMON_SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) tests.o $(TARGET) $(TEST_TARGET)

.PHONY: all test clean
