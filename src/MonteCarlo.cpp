// // Monte Carlo methods to compute the best move at each state
// // Author: Fabrice Renard
// // Date : 23 / 06 / 2023

// #include "MonteCarlo.hpp"

// Game move(Game game, Move move) {
//     // Perform move on game
//     switch (move) {
//     case Move::LEFT:
//         game.moveLeft();
//         break;
//     case Move::RIGHT:
//         game.moveRight();
//         break;
//     case Move::UP:
//         game.moveUp();
//         break;
//     case Move::DOWN:
//         game.moveDown();
//         break;
//     default:
//         break;
//     }
//     return game;
// }

// void simulate(Game& game, std::mt19937& localGen, double& localScore) {
//     int minValue = 0;
//     int maxValue = 3;
//     std::uniform_int_distribution<int> intDistribution(minValue, maxValue);

//     // Simulate game until end
//     while (!game.isGameOver()) {
//         Move randomMove = static_cast<Move>(intDistribution(localGen));
//         switch (randomMove) {
//         case Move::LEFT:
//             game.moveLeft();
//             break;
//         case Move::RIGHT:
//             game.moveRight();
//             break;
//         case Move::UP:
//             game.moveUp();
//             break;
//         case Move::DOWN:
//             game.moveDown();
//             break;
//         default:
//             break;
//         }
//     }
//     // Update local score
//     localScore += game.getScore();
// }

// void runSimulations(Game game, Move currentMove, int numberOfSimulations, double& totalScore, std::mutex& scoreMutex) {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     double localScore = 0.0;

//     for (int i = 0; i < numberOfSimulations; ++i) {
//         Game gameCopy = move(game, currentMove);
//         simulate(gameCopy, gen, localScore);
//     }

//     std::lock_guard<std::mutex> lock(scoreMutex);
//     totalScore += localScore;
// }

// std::map<double, Move, Compare> performMC(Game game, int numberOfSimulationsPerMove) {
//     std::vector<std::thread> threads;
//     std::vector<double> scores(4, 0.0);
//     std::mutex scoreMutex;

//     // Run simulations for each move in parallel
//     for (int j = 0; j < 4; ++j) {
//         threads.emplace_back(runSimulations, game, static_cast<Move>(j), numberOfSimulationsPerMove, std::ref(scores[j]), std::ref(scoreMutex));
//     }

//     for (auto& thread : threads) {
//         thread.join();
//     }

//     // Construct moves map
//     std::map<double, Move, Compare> moves;
//     for (int i = 0; i < 4; ++i) {
//         moves.insert({ scores[i], static_cast<Move>(i) });
//     }

//     return moves;
// }

#include "MonteCarlo.hpp"

class SimulationThread : public QThread {
    Q_OBJECT
public:
    SimulationThread(Game game, Move move, int numberOfSimulations, double& totalScore, QMutex& scoreMutex)
        : game(game), move(move), numberOfSimulations(numberOfSimulations), totalScore(totalScore), scoreMutex(scoreMutex) {}

    void run() override {
        std::random_device rd;
        std::mt19937 gen(rd());
        double localScore = 0.0;

        for (int i = 0; i < numberOfSimulations; ++i) {
            Game gameCopy = moveGame(game, move);
            simulate(gameCopy, gen, localScore);
        }

        QMutexLocker locker(&scoreMutex);
        totalScore += localScore;
    }

private:
    Game game;
    Move move;
    int numberOfSimulations;
    double& totalScore;
    QMutex& scoreMutex;

    Game moveGame(Game game, Move move) {
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
        int i = 0;
        // Simulate game until end
        while (!game.isGameOver()/* || i < 20*/) {
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
            i++;
        }
        // Update local score
        localScore += game.getScore();
    }
};

Move performMC(Game game, int numberOfSimulationsPerMove) {
    QVector<SimulationThread*> threads;
    QVector<double> scores(4, 0.0);
    QMutex scoreMutex;

    // Run simulations for each move in parallel
    for (int j = 0; j < 4; ++j) {
        SimulationThread* thread = new SimulationThread(game, static_cast<Move>(j), numberOfSimulationsPerMove, scores[j], scoreMutex);
        threads.append(thread);
        thread->start();
    }

    for (SimulationThread* thread : threads) {
        thread->wait();
        delete thread;
    }

    // Find the best move based on the scores
    double bestScore = -1.0;
    Move bestMove = Move::LEFT;  // Initialize with a default move

    for (int i = 0; i < 4; ++i) {
        if (scores[i] > bestScore) {
            bestScore = scores[i];
            bestMove = static_cast<Move>(i);
        }
    }

    return bestMove;
}

#include "MonteCarlo.moc"
