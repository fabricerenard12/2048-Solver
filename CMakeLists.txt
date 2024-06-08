cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(2048_Solver VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set compiler flags for different build types
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-Os")

# Find the Qt6 package
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Enable AUTOMOC
set(CMAKE_AUTOMOC ON)

# Add the executable
add_executable(2048_Solver
    src/main.cpp
    src/Game.cpp
    src/GameWindow.cpp
    src/MonteCarlo.cpp
    include/Game.hpp
    include/GameWindow.hpp
    include/MonteCarlo.hpp
)

# Include directories
target_include_directories(2048_Solver PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Link Qt6 libraries
target_link_libraries(2048_Solver Qt6::Widgets)