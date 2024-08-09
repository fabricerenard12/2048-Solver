// Python bindings for the 2048 Game class
// Author: Fabrice Renard
// Date : 30 / 08 / 2024

#include <pybind11/pybind11.h>
#include "Game.hpp"

namespace py = pybind11;

PYBIND11_MODULE(game_bindings, m) {
    py::enum_<Move>(m, "Move")
        .value("LEFT", Move::LEFT)
        .value("RIGHT", Move::RIGHT)
        .value("UP", Move::UP)
        .value("DOWN", Move::DOWN)
        .export_values();

    py::class_<Game, std::shared_ptr<Game>>(m, "Game")
        .def(py::init<>())
        .def(py::init<const Game&>())
        .def("add_tile", &Game::addTile)
        .def("move_left", &Game::moveLeft)
        .def("move_right", &Game::moveRight)
        .def("move_up", &Game::moveUp)
        .def("move_down", &Game::moveDown)
        .def("make_move", &Game::makeMove)
        .def("merge", &Game::merge)
        .def("compress", &Game::compress)
        .def("flip", &Game::flip)
        .def("transpose", &Game::transpose)
        .def("handle_key_press", &Game::handleKeyPress)
        .def("get_score", &Game::getScore)
        .def("get_grid", &Game::getGrid)
        .def("set_grid", &Game::setGrid)
        .def("reached_2048", &Game::reached2048)
        .def("is_game_over", &Game::isGameOver)
        .def("__eq__", &Game::operator==)
        .def("__repr__",
             [](const Game &a) {
                 std::ostringstream oss;
                 oss << a;
                 return oss.str();
             });
}
