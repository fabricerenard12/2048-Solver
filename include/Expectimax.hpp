// Expectimax.hpp
// Header file for Expectimax algorithm for 2048 game
// Author: Fabrice Renard
// Date: 23 / 06 / 2023

#ifndef EXPECTIMAX_HPP
#define EXPECTIMAX_HPP

#include <limits>
#include <memory>
#include <unordered_map>
#include <thread>
#include <vector>
#include <mutex>
#include "Game.hpp"

double evaluateGrid(const Grid& grid);

double expectimax(std::shared_ptr<Game> game, int depth, bool isMaximizingPlayer, double alpha, double beta);

std::pair<Move, double> findBestMove(std::shared_ptr<Game> game, int depth);

int determineDepth(const Grid& grid);

#endif // EXPECTIMAX_HPP
