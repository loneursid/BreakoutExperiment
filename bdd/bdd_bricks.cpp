#include <catch2/catch_test_macros.hpp>
#include "bdd_helpers.h"

// Mirrors features/bricks.feature

SCENARIO("Exactly 50 bricks on game start (AC-04-a)") {
    GIVEN("the game enters PLAYING state") {
        BddGame game;
        game.tapSpace();
        WHEN("the brick grid is initialised") {
            THEN("exactly 50 bricks are active") {
                REQUIRE(game.activeBrickCount() == BRICK_ROWS * BRICK_COLS);
            }
        }
    }
}

SCENARIO("Row colours are correct top to bottom (AC-04-b)") {
    GIVEN("the brick grid is rendered") {
        BddGame game;
        game.tapSpace();
        const auto& bricks = game.getBricks();
        THEN("row 0 is red") {
            auto c = bricks[0].getColor();
            REQUIRE(c.r > 200); REQUIRE(c.g < 80); REQUIRE(c.b < 80);
        }
        THEN("row 1 is orange") {
            auto c = bricks[BRICK_COLS].getColor();
            REQUIRE(c.r > 200); REQUIRE(c.g > 100); REQUIRE(c.b < 30);
        }
        THEN("row 2 is yellow") {
            auto c = bricks[2 * BRICK_COLS].getColor();
            REQUIRE(c.r > 200); REQUIRE(c.g > 200); REQUIRE(c.b < 30);
        }
        THEN("row 3 is green") {
            auto c = bricks[3 * BRICK_COLS].getColor();
            REQUIRE(c.r < 30); REQUIRE(c.g > 150);
        }
        THEN("row 4 is blue") {
            auto c = bricks[4 * BRICK_COLS].getColor();
            REQUIRE(c.r < 30); REQUIRE(c.b > 150);
        }
    }
}

SCENARIO("Brick destroyed on ball contact (AC-04-d)") {
    GIVEN("an active brick exists and the ball overlaps it") {
        BddGame game;
        game.tapSpace();
        const Brick& firstBrick = game.getBricks()[0];
        Ball& ball = const_cast<Ball&>(game.getBall());
        ball.setPosition({firstBrick.getX() + firstBrick.getWidth() / 2.0f,
                          firstBrick.getY() + firstBrick.getHeight() / 2.0f});
        int scoreBefore = game.getScoreManager().getScore();
        WHEN("the collision is processed") {
            game.update(0.016f);
            THEN("the brick is marked inactive") {
                REQUIRE_FALSE(game.getBricks()[0].isActive());
            }
            THEN("10 points are added to the score") {
                REQUIRE(game.getScoreManager().getScore() == scoreBefore + SCORE_PER_BRICK);
            }
        }
    }
}
