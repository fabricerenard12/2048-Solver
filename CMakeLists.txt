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

# Ensure all targets are compiled with -fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find the Qt6 package
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# Add Google Test and Google Benchmark as subdirectories
add_subdirectory(googletest)
add_subdirectory(benchmark)
# add_subdirectory(pytorch)

# Enable AUTOMOC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# Add the main executable
add_executable(2048_Solver
    src/main.cpp
    src/Game.cpp
    src/GameWindow.cpp
    src/MonteCarlo.cpp
    src/ThreadPool.cpp
    include/Consts.hpp
    include/Game.hpp
    include/GameWindow.hpp
    include/MonteCarlo.hpp
    include/ThreadPool.hpp
)

# Include directories
target_include_directories(2048_Solver PUBLIC "${PROJECT_SOURCE_DIR}/include")
# include_directories(${CMAKE_SOURCE_DIR}/pytorch/torch/csrc/api/include)
# include_directories(${CMAKE_SOURCE_DIR}/pytorch/torch/lib/include)

# Link Qt6 libraries
target_link_libraries(2048_Solver Qt6::Widgets gtest gtest_main)

# Link Torch libraries
# target_link_libraries(2048_Solver "${TORCH_LIBRARIES}")

# Add test executable
add_executable(2048_Solver_test
    tests/tests.cpp
    src/Game.cpp
    include/Consts.hpp
    include/Game.hpp
)

# Link Qt6 libraries
target_link_libraries(2048_Solver_test Qt6::Widgets)

# Link Google Test libraries
target_link_libraries(2048_Solver_test
    gtest
    gtest_main
)

# Link Torch libraries
# target_link_libraries(2048_Solver_test "${TORCH_LIBRARIES}")

# Include directories for tests
target_include_directories(2048_Solver_test PUBLIC "${PROJECT_SOURCE_DIR}/include")

# Enable testing
enable_testing()
add_test(NAME 2048_Solver_test COMMAND 2048_Solver_test)

# Add benchmark executable
add_executable(2048_Solver_benchmark
    benchmarks/benchmark.cpp
    src/Game.cpp
    include/Game.hpp
    src/MonteCarlo.cpp
    src/ThreadPool.cpp
    include/MonteCarlo.hpp
    include/Consts.hpp
    include/ThreadPool.hpp
)

# Link Google Benchmark libraries
target_link_libraries(2048_Solver_benchmark benchmark::benchmark)
target_link_libraries(2048_Solver_benchmark Qt6::Widgets gtest gtest_main)

# Link Torch libraries
# target_link_libraries(2048_Solver "${TORCH_LIBRARIES}")

# Include directories for benchmark
target_include_directories(2048_Solver_benchmark PUBLIC "${PROJECT_SOURCE_DIR}/include")
