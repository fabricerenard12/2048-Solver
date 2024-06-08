mkdir -p build && cd build
cmake ..
make
ctest
./2048_Solver
