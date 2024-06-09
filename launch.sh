mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
./2048_Solver_test
./2048_Solver
