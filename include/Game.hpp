// Game class for the 2048 game logic
// Author: Fabrice Renard
// Date : 23 / 06 / 2023

#ifndef GAME_H
#define GAME_H

#include <iomanip>
#include <iostream>
#include <random>
#include <QApplication>
#include <QDebug>
#include <QObject>
#include <vector>
#include <gtest/gtest.h>

using Grid = std::vector<std::vector<int>>;
enum class Move { LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3 };

struct Compare {
	bool operator() (const int& a, const int& b) const {
		return a > b;
	}
};

class Game : public QObject {
	Q_OBJECT
public:
	Game(int gridSize);
	Game(const Game& other);
	bool moveLeft();
	bool moveRight();
	bool moveUp();
	bool moveDown();
	int getScore();
	Grid getGrid();
	int getGridSize();
	bool isGameOver();
	friend bool operator==(const Game& left, const Game& right);
    friend class GameTest_AddTileWorksCorrectly_Test;
    friend class GameTest_AddTileFailsWhenGridIsFull_Test;

public slots:
	void handleKeyPress(char key, std::map<double, Move, Compare> bestMoves, std::shared_ptr<Game> game);

private:
	bool merge();
	bool compress();
	void flip();
	void transpose();
	bool addTile();
	void setGrid(Grid grid);
	bool makeMove(Move move);

	Grid grid_;
	int gridSize_;
	int score_;
	std::random_device rd_;
	std::mt19937 gen_;

	FRIEND_TEST(GameTest, AddTileWorksCorrectly);
	FRIEND_TEST(GameTest, AddTileFailsWhenGridIsFull);
	FRIEND_TEST(GameTest, MoveLeftWorks);
	FRIEND_TEST(GameTest, MoveRightWorks);
	FRIEND_TEST(GameTest, MoveUpWorks);
	FRIEND_TEST(GameTest, MoveDownWorks);
	FRIEND_TEST(GameTest, IsGameOver);
	FRIEND_TEST(GameTest, ScoreCalculation);
	friend class GameWindow;
};

#endif // !GAME_H