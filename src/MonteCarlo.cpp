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

double simulate(std::unique_ptr<Game> game, std::ranlux48& localGen) {
    std::uniform_int_distribution<int> intDistribution(0, 3);

    double localScore = 0.0;
    for (int i = 0; i < DEPTH && !game->isGameOver(); ++i) {
        Move randomMove = static_cast<Move>(intDistribution(localGen));
        game = move(*game, randomMove);
    }

    localScore += game->getScore();
    return localScore;
}

double runSimulations(const Game& game, Move currentMove, int numberOfSimulations) {
    thread_local std::random_device rd;
    thread_local std::ranlux48 gen(rd());

    double totalScore = 0.0;

    for (int i = 0; i < numberOfSimulations; ++i) {
        std::unique_ptr<Game> gameCopy = move(game, currentMove);
        totalScore += simulate(std::move(gameCopy), gen);
    }

    return totalScore;
}

Move performMC(const Game& game, int numberOfSimulationsPerMove, int numThreads) {
    ThreadPool pool(numThreads);
    std::vector<std::future<double>> futures;

    for (int j = 0; j < 4; ++j) {
        for (int t = 0; t < numThreads; ++t) {
            futures.push_back(pool.enqueue(runSimulations, std::cref(game), static_cast<Move>(j), numberOfSimulationsPerMove / numThreads));
        }
    }

    std::vector<double> scores(4, 0.0);
    for (int j = 0; j < 4; ++j) {
        for (int t = 0; t < numThreads; ++t) {
            scores[j] += futures[j * numThreads + t].get();
        }
    }

    auto bestMoveIter = std::max_element(scores.begin(), scores.end());
    int bestMoveIndex = std::distance(scores.begin(), bestMoveIter);

    return static_cast<Move>(bestMoveIndex);
}
