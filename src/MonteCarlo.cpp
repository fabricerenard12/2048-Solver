// Monte Carlo methods to compute the best move at each state
// Author: Fabrice Renard
// Date : 23 / 06 / 2023

#include "MonteCarlo.hpp"

Game move(Game game, Move move) {
    // Perform move on game
    switch (move) {
    case Move::LEFT:
        game.moveLeft();
        break;
    case Move::RIGHT:
        game.moveRight();
        break;
    case Move::UP:
        game.moveUp();
        break;
    case Move::DOWN:
        game.moveDown();
        break;
    default:
        break;
    }

    return game;
}

void simulate(Game& game, std::mt19937& localGen, double& localScore) {
    int minValue = 0;
    int maxValue = 3;

    std::uniform_int_distribution<int> intDistribution(minValue, maxValue);

    // Simulate game until end
    while (!game.isGameOver()) {
        Move randomMove = static_cast<Move>(intDistribution(localGen));

        switch (randomMove) {
        case Move::LEFT:
            game.moveLeft();
            break;
        case Move::RIGHT:
            game.moveRight();
            break;
        case Move::UP:
            game.moveUp();
            break;
        case Move::DOWN:
            game.moveDown();
            break;
        default:
            break;
        }
    }

    // Update local score
    localScore += game.getScore();
}

std::map<double, Move, Compare> performMC(Game game, int numberOfSimulationsPerMove) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<double> scores(4, 0.0);
    std::map<double, Move, Compare> moves;

    // Compute best move using Monte Carlo Computation
    for (int j = 0; j < 4; j++) {
        Move currentMove = static_cast<Move>(j);

        for (int i = 0; i < numberOfSimulationsPerMove; i++) {
            Game gameCopy = move(game, currentMove);
            double localScore = 0.0;
            simulate(gameCopy, gen, localScore);
            scores[j] += localScore;
        }
    }

    // Construct moves map
    for (int i = 0; i < 4; i++) {
        moves.insert({ scores[i], static_cast<Move>(i) });
    }

    return moves;
}
