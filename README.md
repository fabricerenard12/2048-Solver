# 2048-Solver-WASM

## Overview

This repository contains a WebAssembly (WASM) implementation of a 2048 Solver. The solver uses a C++ algorithm to find the optimal moves to play the 2048 game. The project leverages WebAssembly to run the C++ code efficiently in a web environment.

## Features

- **Optimal Move Calculation**: Utilizes advanced algorithms to determine the best moves.
- **WebAssembly**: Compiles C++ code to WebAssembly for high performance in the browser.
- **Interactive Demo**: Run the solver directly in your browser.

## Installation

To set up the project locally, follow these steps:

1. **Clone the repository**:
    ```sh
    git clone https://github.com/fabricerenard12/2048-Solver-WASM.git
    cd 2048-Solver-WASM
    ```

2. **Install Emscripten**:
   Follow the instructions on the [Emscripten website](https://emscripten.org/docs/getting_started/downloads.html) to install and set up Emscripten.

3. **Compile the C++ code to WebAssembly**:
    ```sh
    emcc src/solver.cpp -o public/solver.js -s MODULARIZE -s 'EXPORT_NAME="createModule"' -s EXPORTED_FUNCTIONS='["_solve"]'
    ```

4. **Serve the project**:
   You can use any static file server to serve the project. For example, using Python's built-in HTTP server:
    ```sh
    cd public
    python3 -m http.server
    ```
   Open your browser and navigate to `http://localhost:8000` to see the solver in action.

## Usage

Once the project is up and running, you can interact with the 2048 solver through the web interface. Input your 2048 game state and let the solver calculate the optimal moves for you.

## Contributing

Contributions are welcome! If you find any issues or have suggestions for improvements, feel free to open an issue or submit a pull request.

1. **Fork the repository**
2. **Create a new branch**: `git checkout -b my-feature-branch`
3. **Commit your changes**: `git commit -m 'Add some feature'`
4. **Push to the branch**: `git push origin my-feature-branch`
5. **Open a pull request**

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.

## Acknowledgements

- [Emscripten](https://emscripten.org/) for the WebAssembly compiler

