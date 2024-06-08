// Main file
// Author: Fabrice Renard
// Date : 23 / 06 / 2023

#include "GameWindow.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    constexpr int GRID_SIZE = 4;

    QApplication a(argc, argv);
    std::shared_ptr<Game> game = std::make_shared<Game>(GRID_SIZE);
    GameWindow w(nullptr, GRID_SIZE, game);

    QObject::connect(&w, &GameWindow::keyPressed, game.get(), &Game::handleKeyPress);

    w.show();
    return a.exec();
}