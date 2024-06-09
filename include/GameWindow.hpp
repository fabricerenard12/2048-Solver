// GameWindow class for the QT6 view
// Author: Fabrice Renard
// Date : 23 / 06 / 2023

#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QPalette>
#include <QColor>
#include <QFont>
#include <QIcon>
#include <QThread>

#include "MonteCarlo.hpp"

constexpr int SPACEBAR_CHAR = 32;
constexpr int NUMBER_OF_SIMULATIONS_PER_MOVE = 1;
constexpr int WINDOW_SIZE = 500;
constexpr int GRID_SPACING = 10;

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget* parent, int gridSize, std::shared_ptr<Game> game);
    void setupWindow();
    void updateGrid();

protected:
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void keyPressed(char key, std::map<double, Move, Compare> bestMoves, std::shared_ptr<Game> game);

private:
    void setupGrid();
    void applyStyleSheet();
    void setWindowSize();
    void setLabelStyle(QLabel* label, int value);

private:
    const int gridSize_;
    std::shared_ptr<Game> game_;
    std::vector<std::vector<QLabel*>> gridLabels;
};

#endif // GAMEWINDOW_H