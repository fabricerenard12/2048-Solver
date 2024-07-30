// Game class for the 2048 game logic
// Author: Fabrice Renard
// Date : 23 / 06 / 2023

#include "Game.hpp"

void printBinary(uint64_t value) {
    std::bitset<64> binary(value);
    std::cout << binary << std::endl;
}

Game::Game() 
    : grid_(0), score_(0), rd_(std::random_device()), gen_(std::ranlux48(rd_())) {
    int minValue = 0;
    int maxValue = (GRID_SIZE * GRID_SIZE) - 1;
    double probability;

    std::uniform_int_distribution<int> intDistribution(minValue, maxValue);
    std::uniform_real_distribution<double> realDistribution(0.0, 1.0);

    int randomTile1 = intDistribution(gen_);
    probability = realDistribution(gen_);

    grid_ |= (static_cast<uint64_t>((probability < 0.9) ? 1 : 2) << (randomTile1 * 4));

    int randomTile2 = intDistribution(gen_);
    while (randomTile2 == randomTile1) {
        randomTile2 = intDistribution(gen_);
    }
    probability = realDistribution(gen_);

    grid_ |= (static_cast<uint64_t>((probability < 0.9) ? 1 : 2) << (randomTile2 * 4));
}

Game::Game(const Game& other) {
    grid_ = other.grid_;
    score_ = other.score_;
}

bool Game::addTile() {
    bool gridFull = true;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++) {
        int tile = (grid_ >> (i * 4)) & 0xF;
        if (tile == 0) { 
            gridFull = false;
            break;
        }
    }

    if (gridFull) {
        return false;
    }

    int minValue = 0;
    int maxValue = (GRID_SIZE * GRID_SIZE) - 1;

    std::uniform_int_distribution<int> intDistribution(minValue, maxValue);
    std::uniform_real_distribution<double> realDistribution(0.0, 1.0);

    double probability = realDistribution(gen_);

    while (true) {
        int randomTile = intDistribution(gen_);
        int tile = (grid_ >> (randomTile * 4)) & 0xF;

        if (tile == 0) {
            grid_ &= ~(0xFULL << (randomTile * 4));
            grid_ |= (static_cast<uint64_t>((probability < 0.9) ? 1 : 2) << (randomTile * 4));
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
        validMove = false;
        break;
    }

    return validMove;
}

bool Game::merge() {
    bool merged = false;

    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE - 1; ++col) {
            int bitPositionCurrent = (row * GRID_SIZE + col) * 4;
            int bitPositionNext = (row * GRID_SIZE + col + 1) * 4;

            int currentTile = (grid_ >> bitPositionCurrent) & 0xF;
            int nextTile = (grid_ >> bitPositionNext) & 0xF;

            if (currentTile == nextTile && currentTile != 0) {
                int sum = currentTile + 1;
                score_ += (1 << sum);

                grid_ &= ~(0xFULL << bitPositionCurrent);
                grid_ &= ~(0xFULL << bitPositionNext);

                grid_ |= (static_cast<uint64_t>(sum & 0xF) << bitPositionCurrent);
                merged = true;
                col++;
            }
        }
    }

    return merged;
}

bool Game::compress() {
    bool compressed = false;

    for (int row = 0; row < GRID_SIZE; ++row) {
        int insertPos = row * GRID_SIZE;

        for (int col = 0; col < GRID_SIZE; ++col) {
            int bitPosition = (row * GRID_SIZE + col) * 4;
            int tile = (grid_ >> bitPosition) & 0xF;

            if (tile != 0) {
                if ((row * GRID_SIZE + col) != insertPos) {
                    int insertBitPosition = insertPos * 4;

                    grid_ &= ~(0xFULL << insertBitPosition);
                    grid_ |= (static_cast<uint64_t>(tile) << insertBitPosition);

                    grid_ &= ~(0xFULL << bitPosition);

                    compressed = true;
                }
                insertPos++;
            }
        }
    }

    return compressed;
}


void Game::flip() {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = 0; col < GRID_SIZE / 2; ++col) {
            int bitPosition = (row * GRID_SIZE + col) * 4;
            int mirrorIndex = row * GRID_SIZE + (GRID_SIZE - 1 - col);
            int mirrorBitPosition = mirrorIndex * 4;

            int currentTile = (grid_ >> bitPosition) & 0xF;
            int mirrorTile = (grid_ >> mirrorBitPosition) & 0xF;

            grid_ &= ~(0xFULL << bitPosition);
            grid_ |= (static_cast<uint64_t>(mirrorTile) << bitPosition);

            grid_ &= ~(0xFULL << mirrorBitPosition);
            grid_ |= (static_cast<uint64_t>(currentTile) << mirrorBitPosition);
        }
    }
}

void Game::transpose() {
    for (int row = 0; row < GRID_SIZE; ++row) {
        for (int col = row + 1; col < GRID_SIZE; ++col) {
            int bitPositionCurrent = (row * GRID_SIZE + col) * 4;
            int bitPositionTranspose = (col * GRID_SIZE + row) * 4;

            int currentTile = (grid_ >> bitPositionCurrent) & 0xF;
            int transposedTile = (grid_ >> bitPositionTranspose) & 0xF;

            grid_ &= ~(0xFULL << bitPositionCurrent);
            grid_ |= (static_cast<uint64_t>(transposedTile) << bitPositionCurrent);

            grid_ &= ~(0xFULL << bitPositionTranspose);
            grid_ |= (static_cast<uint64_t>(currentTile) << bitPositionTranspose);
        }
    }
}


void Game::handleKeyPress(char key, Move bestMove, std::shared_ptr<Game> game) {
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
        if (!game->makeMove(bestMove)) {
            for (int i = 0; i < 3; i++) {
                if (game->makeMove(static_cast<Move>(i))) {
                    break;
                } 
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

void Game::setGrid(Grid grid) {
    grid_ = grid;
}

bool Game::reached2048() {
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        int tile = (grid_ >> (i * 4)) & 0xF;
        if (tile == 11) {
            return true;
        }
    }
    return false;
}

bool Game::isGameOver() {
    Game game = *this;
    if (!(game.moveLeft() || game.moveRight() || game.moveUp() || game.moveDown())) {
        return true;
    }

    return false;
}

bool operator==(const Game& left, const Game& right) {
    return left.score_ == right.score_ && left.grid_ == right.grid_;
}

std::ostream& operator<<(std::ostream& os, const Game& game) {
    std::bitset<64> binary(game.grid_);
    os << binary;
    return os;
}
