#include <gtest/gtest.h>
#include "Game.hpp"

class GameTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>(4);
    }

    std::unique_ptr<Game> game;
};

TEST_F(GameTest, InitialGridHasTwoTiles) {
    Grid grid = game->getGrid();
    int tileCount = 0;

    for (const auto& row : grid) {
        for (const auto& tile : row) {
            if (tile != 0) {
                tileCount++;
            }
        }
    }

    ASSERT_EQ(tileCount, 2);
}

TEST_F(GameTest, AddTileWorksCorrectly) {
    Grid grid = game->getGrid();
    for (auto& row : grid) {
        for (auto& tile : row) {
            tile = 2;
        }
    }
    grid[3][3] = 0;
    game->setGrid(grid);

    ASSERT_TRUE(game->addTile());
    grid = game->getGrid();
    ASSERT_NE(grid[3][3], 0);
}

TEST_F(GameTest, AddTileFailsWhenGridIsFull) {
    Grid grid = game->getGrid();
    for (auto& row : grid) {
        for (auto& tile : row) {
            tile = 2;
        }
    }
    game->setGrid(grid);

    ASSERT_FALSE(game->addTile());
}

TEST_F(GameTest, MoveLeftWorks) {
    Grid grid = game->getGrid();
    grid[0][0] = 2;
    grid[0][1] = 2;
    game->setGrid(grid);

    ASSERT_TRUE(game->moveLeft());
    grid = game->getGrid();
    ASSERT_EQ(grid[0][0], 4);
}

TEST_F(GameTest, MoveRightWorks) {
    Grid grid = game->getGrid();
    grid[0][2] = 2;
    grid[0][3] = 2;
    game->setGrid(grid);

    ASSERT_TRUE(game->moveRight());
    grid = game->getGrid();
    ASSERT_EQ(grid[0][3], 4);
}

TEST_F(GameTest, MoveUpWorks) {
    Grid grid = game->getGrid();
    grid[0][0] = 2;
    grid[1][0] = 2;
    game->setGrid(grid);

    ASSERT_TRUE(game->moveUp());
    grid = game->getGrid();
    ASSERT_EQ(grid[0][0], 4);
}

TEST_F(GameTest, MoveDownWorks) {
    Grid grid = game->getGrid();
    grid[2][0] = 2;
    grid[3][0] = 2;
    game->setGrid(grid);

    ASSERT_TRUE(game->moveDown());
    grid = game->getGrid();
    ASSERT_EQ(grid[3][0], 4);
}

TEST_F(GameTest, IsGameOver) {
    Grid grid = game->getGrid();
    grid = {
        {2, 4, 2, 4},
        {4, 2, 4, 2},
        {2, 4, 2, 4},
        {4, 2, 4, 2}
    };
    game->setGrid(grid);

    ASSERT_TRUE(game->isGameOver());
}

TEST_F(GameTest, ScoreCalculation) {
    Grid grid = game->getGrid();
    grid[0][0] = 2;
    grid[0][1] = 2;
    game->setGrid(grid);

    game->moveLeft();
    ASSERT_GE(game->getScore(), 4);
}
