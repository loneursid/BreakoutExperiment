#include <catch2/catch_test_macros.hpp>
#include "bdd_helpers.h"

// Mirrors features/scoring.feature

SCENARIO("Score initialises to 0 (AC-06-a)") {
    GIVEN("a new game starts") {
        BddGame game;
        WHEN("the PLAYING state is entered") {
            game.tapSpace();
            THEN("the score is 0") {
                REQUIRE(game.getScoreManager().getScore() == 0);
            }
        }
    }
}

SCENARIO("Brick destroyed adds 10 points (AC-06-b)") {
    GIVEN("the game is in PLAYING state") {
        BddGame game;
        game.tapSpace();
        int scoreBefore = game.getScoreManager().getScore();
        WHEN("a brick is destroyed") {
            const Brick& b = game.getBricks()[0];
            Ball& ball = const_cast<Ball&>(game.getBall());
            ball.setPosition({b.getX() + b.getWidth() / 2.0f,
                              b.getY() + b.getHeight() / 2.0f});
            game.update(0.016f);
            THEN("the score increases by exactly 10") {
                REQUIRE(game.getScoreManager().getScore() == scoreBefore + SCORE_PER_BRICK);
            }
        }
    }
}

SCENARIO("Score label format (AC-06-c)") {
    GIVEN("the game is in PLAYING state") {
        BddGame game;
        game.tapSpace();
        THEN("the formatted score starts with 'Score: '") {
            std::string label = game.getScoreManager().getFormattedScore();
            REQUIRE(label.rfind("Score: ", 0) == 0);
        }
    }
}
