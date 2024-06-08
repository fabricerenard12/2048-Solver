// Game class for the 2048 game logic
// Author: Fabrice Renard
// Date : 23 / 06 / 2023

#include "Game.hpp"

Game::Game(int gridSize) 
    : gridSize_(gridSize), grid_(gridSize), score_(0), rd_(std::random_device()), gen_(std::mt19937(rd_())) {

    // Initialize grid
    for (std::vector<int>& row : grid_) {
        row.resize(gridSize_);
    }

    for (std::vector<int>& row : grid_) {
        for (int& tile : row) {
            tile = 0;
        }
    }

    // Add two random tiles to grid
    int minValue = 0;
    int maxValue = (gridSize_ * gridSize_) - 1;
    int row, col;
    double probability;

    std::uniform_int_distribution<int> intDistribution(minValue, maxValue);
    std::uniform_real_distribution<double> realDistribution(0.0, 1.0);

    int randomTile1 = intDistribution(gen_);
    row = randomTile1 / gridSize_;
    col = randomTile1 % gridSize_;
    probability = realDistribution(gen_);

    grid_[row][col] = (probability < 0.9) ? 2 : 4;

    int randomTile2 = intDistribution(gen_);
    while (randomTile2 == randomTile1) {
        randomTile2 = intDistribution(gen_);
    }
    row = randomTile2 / gridSize_;
    col = randomTile2 % gridSize_;
    probability = realDistribution(gen_);

    grid_[row][col] = (probability < 0.9) ? 2 : 4;
}

Game::Game(const Game& other) {
    gridSize_ = other.gridSize_;
    grid_ = other.grid_;
    score_ = other.score_;
}

bool Game::addTile() {
    // Check if grid is full
    bool gridFull = true;
    for (const std::vector<int>& row : grid_) {
        if (std::find(row.begin(), row.end(), 0) != row.end()) {
            gridFull = false;
            break;
        }
    }

    if (gridFull) {
        return false;
    }

    // Set initial random values in grid
    int minValue = 0;
    int maxValue = (gridSize_ * gridSize_) - 1;
    int row, col;

    std::uniform_int_distribution<int> intDistribution(minValue, maxValue);
    std::uniform_real_distribution<double> realDistribution(0.0, 1.0);

    double probability = realDistribution(gen_);

    while (true) {
        int randomTile = intDistribution(gen_);
        row = randomTile / gridSize_;
        col = randomTile % gridSize_;

        if (grid_[row][col] == 0) {
            grid_[row][col] = (probability < 0.9) ? 2 : 4;
            return true;
        }
    }
}

bool Game::moveLeft() {
    bool compressed = compress();
    bool merged = merge();
    compress();

    if (compressed || merged) {
        addTile();
        return true;
    }

    return false;
}

bool Game::moveRight() {
    flip();
    bool validMove = moveLeft();
    flip();

    return validMove;
}

bool Game::moveUp() {
    transpose();
    bool validMove = moveLeft();
    transpose();

    return validMove;
}

bool Game::moveDown() {
    transpose();
    bool validMove = moveRight();
    transpose();

    return validMove;
}

bool Game::makeMove(Move move) {
    bool validMove;
    switch (move) {
    case Move::LEFT:
        validMove = moveLeft();
        break;
    case Move::RIGHT:
        validMove = moveRight();
        break;
    case Move::UP:
        validMove = moveUp();
        break;
    case Move::DOWN:
        validMove = moveDown();
        break;
    default:
        break;
    }

    return validMove;
}

bool Game::merge() {
    bool merged = false;

    // Merge tiles together
    for (int i = 0; i < gridSize_; i++) {
        bool mergedPrivate = false;

        for (int j = 0; j < gridSize_ - 1; j++) {
            if (grid_[i][j] == grid_[i][j + 1]) {
                mergedPrivate = true;
                int sum = grid_[i][j] + grid_[i][j + 1];
                score_ += sum;

                grid_[i][j] = sum;
                grid_[i][j + 1] = 0;
                break;
            }
        }

        merged = merged || mergedPrivate;
    }

    return merged;
}

bool Game::compress() {
    bool compressed = false;

    // Compress tiles together
    for (int i = 0; i < gridSize_; i++) {
        bool compressedPrivate = false;

        for (int k = 0; k < gridSize_ - 1; k++) {
            if (grid_[i][k] == 0) {
                for (int j = k + 1; j < gridSize_; j++) {
                    if (grid_[i][j] != 0) {
                        compressedPrivate = true;
                        grid_[i][k] = grid_[i][j];
                        grid_[i][j] = 0;
                        break;
                    }
                }
            }
        }

        compressed = compressed || compressedPrivate;
    }

    return compressed;
}


void Game::flip() {
    // Flip grid with respect to the vertical axis
    for (int i = 0; i < gridSize_; i++) {
        for (int j = 0; j < gridSize_ / 2; j++) {
            int temp = grid_[i][j];
            grid_[i][j] = grid_[i][gridSize_ - (j + 1)];
            grid_[i][gridSize_ - (j + 1)] = temp;
        }
    }
}

void Game::transpose() {
    // Transpose grid
    for (int i = 0; i < gridSize_; i++) {
        for (int j = i + 1; j < gridSize_; j++) {
            int temp = grid_[i][j];
            grid_[i][j] = grid_[j][i];
            grid_[j][i] = temp;
        }
    }
}

void Game::handleKeyPress(char key, std::map<double, Move, Compare> bestMoves, std::shared_ptr<Game> game) {
    switch (key) {
    case 'A':
        game->moveLeft();
        break;
    case 'D':
        game->moveRight();
        break;
    case 'S':
        game->moveDown();
        break;
    case 'W':
        game->moveUp();
        break;
    case ' ':
        for (const auto& pair : bestMoves) {
            bool validMove = game->makeMove(pair.second);
            if (validMove) {
                break;
            }
        }
        break;
    default:
        break;
    }
}

int Game::getScore() {
    return score_;
}

Grid Game::getGrid() {
    return grid_;
}

int Game::getGridSize() {
    return gridSize_;
}

bool Game::isGameOver() {
    Game game = *this;
    if (!(game.moveLeft() || game.moveRight() || game.moveUp() || game.moveDown())) {
        return true;
    }

    return false;
}

bool operator==(const Game& left, const Game& right) {
    bool isEqual = true;

    if (left.score_ != right.score_) { return false; }
    if (left.gridSize_ != right.gridSize_) { return false; };

    for (int i = 0; i < left.gridSize_; i++) {
        for (int j = 0; j < left.gridSize_; j++) {
            if (left.grid_[i][j] != right.grid_[i][j]) {
                isEqual = false;
            }
        }
    }

    return isEqual;
}