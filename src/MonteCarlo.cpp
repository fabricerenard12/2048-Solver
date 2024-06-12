// Monte Carlo methods to compute the best move at each state
// Author: Fabrice Renard
// Date : 23 / 06 / 2023

#include "MonteCarlo.hpp"

std::unique_ptr<Game> move(const Game& game, Move move) {
    std::unique_ptr<Game> newGame = std::make_unique<Game>(game);
    switch (move) {
        case Move::LEFT:
            newGame->moveLeft();
            break;
        case Move::RIGHT:
            newGame->moveRight();
            break;
        case Move::UP:
            newGame->moveUp();
            break;
        case Move::DOWN:
            newGame->moveDown();
            break;
        default:
            break;
    }
    return newGame;
}

void simulate(std::unique_ptr<Game> game, std::mt19937& localGen, double& totalScore, std::mutex& scoreMutex) {
    std::uniform_int_distribution<int> intDistribution(0, 3);

    double localScore = 0.0;
    for (int i = 0; i < DEPTH && !game->isGameOver(); ++i) {
        Move randomMove = static_cast<Move>(intDistribution(localGen));
        game = move(*game, randomMove);
    }

    localScore += game->getScore();

    std::lock_guard<std::mutex> lock(scoreMutex);
    totalScore += localScore;
}

void runSimulations(const Game& game, Move currentMove, int numberOfSimulations, double& totalScore, std::mutex& scoreMutex) {
    thread_local std::random_device rd;
    thread_local std::mt19937 gen(rd());

    for (int i = 0; i < numberOfSimulations; ++i) {
        std::unique_ptr<Game> gameCopy = move(game, currentMove);
        simulate(std::move(gameCopy), gen, totalScore, scoreMutex);
    }
}

Move performMC(const Game& game, int numberOfSimulationsPerMove, int numThreads) {
    std::vector<std::thread> threads;
    std::vector<double> scores(4, 0.0);
    std::vector<std::mutex> scoreMutexes(4);

    for (int j = 0; j < 4; ++j) {
        for (int t = 0; t < numThreads; ++t) {
            threads.emplace_back(runSimulations, std::cref(game), static_cast<Move>(j), numberOfSimulationsPerMove / numThreads, std::ref(scores[j]), std::ref(scoreMutexes[j]));
        }
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    auto bestMoveIter = std::max_element(scores.begin(), scores.end());
    int bestMoveIndex = std::distance(scores.begin(), bestMoveIter);

    return static_cast<Move>(bestMoveIndex);
}
