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

# Add Google Test
add_subdirectory(googletest)

# Enable AUTOMOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Add the main executable
add_executable(2048_Solver
    src/main.cpp
    src/Game.cpp
    src/GameWindow.cpp
    src/MonteCarlo.cpp
    src/Expectimax.cpp
    include/Game.hpp
    include/GameWindow.hpp
    include/MonteCarlo.hpp
    include/Expectimax.hpp
)

# Include directories
target_include_directories(2048_Solver PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Link Qt6 libraries
target_link_libraries(2048_Solver Qt6::Widgets gtest gtest_main)

# Add test executable
add_executable(2048_Solver_test
    tests/tests.cpp
    src/Game.cpp
    include/Game.hpp
)

# Link Qt6 libraries
target_link_libraries(2048_Solver_test Qt6::Widgets)

# Link Google Test libraries
target_link_libraries(2048_Solver_test
    gtest
    gtest_main
)

# Include directories for tests
target_include_directories(2048_Solver_test PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Enable testing
enable_testing()
add_test(NAME 2048_Solver_test COMMAND 2048_Solver_test)
