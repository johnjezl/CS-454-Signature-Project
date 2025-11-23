cmake_minimum_required(VERSION 3.26)
project(MyProject LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable folders in IDEs (nice for CLion)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES}
        main.cpp
        DFA.cpp
        DFA.h
        RecurrenceEquationBuilder.cpp
        RecurrenceEquationBuilder.h
        RecurrenceEquationSolver.h
        RecurrenceEquationSolver.cpp)

# Add include directories
target_include_directories(${PROJECT_NAME}
        PRIVATE
)

