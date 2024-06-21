// Monte Carlo methods to compute the best move at each state
// Author: Fabrice Renard
// Date : 23 / 06 / 2023

#ifndef MONTECARLO_H
#define MONTECARLO_H

#include <vector>
#include <thread>
#include <memory>
#include <future>
#include <algorithm>
#include <random>
#include <mutex>
#include "Game.hpp"

constexpr int DEPTH = 3000;

// std::unique_ptr<Game> move(const Game& game, Move move);
// void simulate(std::unique_ptr<Game> game, std::mt19937& localGen, double& totalScore, std::mutex& scoreMutex);
// void runSimulations(const Game& game, Move currentMove, int numberOfSimulations, double& totalScore, std::mutex& scoreMutex);
// Move performMC(const Game& game, int numberOfSimulationsPerMove, int numThreads);

std::unique_ptr<Game> move(const Game& game, Move move);
double simulate(std::unique_ptr<Game> game, std::ranlux48& localGen);
double runSimulations(const Game& game, Move currentMove, int numberOfSimulations);
Move performMC(const Game& game, int numberOfSimulationsPerMove, int numThreads);

#endif // !MONTECARLO_H
