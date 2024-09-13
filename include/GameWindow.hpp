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
#include <QTimer>

#include "Consts.hpp"
#include "MonteCarlo.hpp"

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget* parent, std::shared_ptr<Game> game, bool autoplay = false);
    void setupWindow();
    void updateGrid();
    void startAutoPlay();

protected:
    void keyPressEvent(QKeyEvent* event) override;

signals:
    void keyPressed(char key, Move bestMove, std::shared_ptr<Game> game);

private:
    void setupGrid();
    void applyStyleSheet();
    void setWindowSize();
    void setLabelStyle(QLabel* label, int value);
    void autoPlayMove();

private:
    std::shared_ptr<Game> game_;
    std::vector<std::vector<QLabel*>> gridLabels;
};

#endif // GAMEWINDOW_H