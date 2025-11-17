cmake_minimum_required(VERSION 3.26)
project(MyProject LANGUAGES CXX)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable folders in IDEs (nice for CLion)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Add source files automatically
file(GLOB_RECURSE SOURCES
        src/*.cpp
        src/*.c
        include/*.h
        include/*.hpp
)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES}
        main.cpp
        main.cpp
        DFA.cpp
        DFA.h)

# Add include directories
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)