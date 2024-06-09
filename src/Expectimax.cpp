// Expectimax.cpp
// Implementation file for Expectimax algorithm for 2048 game
// Author: Fabrice Renard
// Date: 23 / 06 / 2023

#include "Expectimax.hpp"

std::unordered_map<Grid, double> transpositionTable;
std::mutex transpositionTableMutex;

double evaluateGrid(const Grid& grid) {
    double score = 0.0;
    int emptyTiles = 0;
    double smoothness = 0.0;
    double monotonicityLeftRight = 0.0;
    double monotonicityUpDown = 0.0;
    int maxTile = 0;
    int cornerWeight = 4; // More weight for corner tiles

    int tiles[GRID_SIZE][GRID_SIZE];
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int index = i * GRID_SIZE + j;
            int tile = (grid >> (index * 4)) & 0xF;
            tiles[i][j] = tile;
            if (tile == 0) {
                emptyTiles++;
            } else {
                int tileValue = 1 << tile;
                score += tileValue;
                if (tileValue > maxTile) {
                    maxTile = tileValue;
                }
                // Add corner heuristic
                if ((i == 0 || i == GRID_SIZE - 1) && (j == 0 || j == GRID_SIZE - 1)) {
                    score += tileValue * cornerWeight;
                }
            }
        }
    }

    // Calculate smoothness and monotonicity
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (tiles[i][j] != 0) {
                int currentTileValue = 1 << tiles[i][j];
                if (i < GRID_SIZE - 1 && tiles[i + 1][j] != 0) {
                    smoothness -= abs(currentTileValue - (1 << tiles[i + 1][j]));
                }
                if (j < GRID_SIZE - 1 && tiles[i][j + 1] != 0) {
                    smoothness -= abs(currentTileValue - (1 << tiles[i][j + 1]));
                }
                if (i < GRID_SIZE - 1) {
                    int nextTileValue = 1 << tiles[i + 1][j];
                    if (currentTileValue > nextTileValue) {
                        monotonicityUpDown += currentTileValue - nextTileValue;
                    } else {
                        monotonicityUpDown -= currentTileValue - nextTileValue;
                    }
                }
                if (j < GRID_SIZE - 1) {
                    int nextTileValue = 1 << tiles[i][j + 1];
                    if (currentTileValue > nextTileValue) {
                        monotonicityLeftRight += currentTileValue - nextTileValue;
                    } else {
                        monotonicityLeftRight -= currentTileValue - nextTileValue;
                    }
                }
            }
        }
    }

    score += emptyTiles * 1000.0;
    score += smoothness * 0.1;
    score += (monotonicityLeftRight + monotonicityUpDown) * 1.0;
    score += maxTile * 10.0;

    return score;
}

double expectimax(std::shared_ptr<Game> game, int depth, bool isMaximizingPlayer, double alpha, double beta) {
    if (depth == 0 || game->isGameOver()) {
        return evaluateGrid(game->getGrid());
    }

    Grid currentGrid = game->getGrid();
    {
        std::lock_guard<std::mutex> lock(transpositionTableMutex);
        if (transpositionTable.find(currentGrid) != transpositionTable.end()) {
            return transpositionTable[currentGrid];
        }
    }

    if (isMaximizingPlayer) {
        double maxEval = std::numeric_limits<double>::lowest();
        for (Move move : {Move::LEFT, Move::RIGHT, Move::UP, Move::DOWN}) {
            std::shared_ptr<Game> newGame = std::make_shared<Game>(*game);
            if (newGame->makeMove(move)) {
                double eval = expectimax(newGame, depth - 1, false, alpha, beta);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) break;
            }
        }
        {
            std::lock_guard<std::mutex> lock(transpositionTableMutex);
            transpositionTable[currentGrid] = maxEval;
        }
        return maxEval;
    } else {
        double totalEval = 0;
        int numPossibleTiles = 0;

        for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
            if ((game->getGrid() & (0xFLL << (4 * i))) == 0) {
                for (int tile : {2, 4}) {
                    std::shared_ptr<Game> newGame = std::make_shared<Game>(*game);
                    newGame->setGrid(game->getGrid() | (tile << (4 * i)));
                    double eval = expectimax(newGame, depth - 1, true, alpha, beta);
                    totalEval += (tile == 2 ? 0.9 : 0.1) * eval;
                    ++numPossibleTiles;
                }
            }
        }
        double avgEval = totalEval / numPossibleTiles;
        {
            std::lock_guard<std::mutex> lock(transpositionTableMutex);
            transpositionTable[currentGrid] = avgEval;
        }
        return avgEval;
    }
}

std::pair<Move, double> findBestMove(std::shared_ptr<Game> game, int depth) {
    double bestValue = std::numeric_limits<double>::lowest();
    Move bestMove = Move::LEFT;
    std::vector<std::thread> threads;
    std::mutex bestMoveMutex;

    for (Move move : {Move::LEFT, Move::RIGHT, Move::UP, Move::DOWN}) {
        threads.emplace_back([&, move]() {
            std::shared_ptr<Game> newGame = std::make_shared<Game>(*game);
            if (newGame->makeMove(move)) {
                double moveValue = expectimax(newGame, depth, false, std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
                std::lock_guard<std::mutex> lock(bestMoveMutex);
                if (moveValue > bestValue) {
                    bestValue = moveValue;
                    bestMove = move;
                }
            }
        });
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    return std::make_pair(bestMove, bestValue);
}

int determineDepth(const Grid& grid) {
    int emptyTiles = 0;
    for (int i = 0; i < GRID_SIZE * GRID_SIZE; ++i) {
        if ((grid & (0xFLL << (4 * i))) == 0) {
            ++emptyTiles;
        }
    }
    if (emptyTiles > 8) return 6; // Early game
    if (emptyTiles > 4) return 4; // Mid game
    return 2; // Late game
}