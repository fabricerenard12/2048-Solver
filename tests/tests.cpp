#include <gtest/gtest.h>
#include "Game.hpp"

class GameTest : public ::testing::Test {
protected:
    void SetUp() override {
        game = std::make_unique<Game>();
    }

    std::unique_ptr<Game> game;
};