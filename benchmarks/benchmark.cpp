#include <benchmark/benchmark.h>
#include <iostream>
#include "Game.hpp"
#include "MonteCarlo.hpp"

static int reach2048Count = 0;

static void BM_2048Game(benchmark::State& state) {
    constexpr int NUMBER_OF_SIMULATIONS_PER_MOVE = 200;
    int NUMBER_OF_THREADS = std::thread::hardware_concurrency();
    for (auto _ : state) {
        std::unique_ptr<Game> game = std::make_unique<Game>();

        while (!game->isGameOver() && !game->reached2048()) {
            Move bestMove = performMC(*(game.get()), NUMBER_OF_SIMULATIONS_PER_MOVE, NUMBER_OF_THREADS);
            bool validMove = game->makeMove(bestMove);

            if (!validMove) {
                for (int i = 0; i < 3; i++) {
                    validMove = game->makeMove(static_cast<Move>(i));
                    if (validMove) {
                        break;
                    } 
                }
            }

            if (game->reached2048()) {
                reach2048Count++;
            }
        }
        std::cout << *game << " " << reach2048Count << std::endl;
    }

    state.counters["2048_Reached"] = reach2048Count;
}

BENCHMARK(BM_2048Game)->Repetitions(20);

BENCHMARK_MAIN();
