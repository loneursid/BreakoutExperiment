#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "bdd_helpers.h"

// Mirrors features/paddle.feature

SCENARIO("Paddle starts centred (AC-02-a)") {
    GIVEN("the game enters PLAYING state") {
        BddGame game;
        game.tapSpace();
        WHEN("the first frame renders") {
            THEN("the paddle is centred horizontally") {
                float expected = (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0f;
                REQUIRE(game.getPaddle().getX() == Catch::Approx(expected));
            }
            THEN("the paddle is 40px above the bottom edge") {
                float expected = SCREEN_HEIGHT - PADDLE_Y_OFFSET - PADDLE_HEIGHT;
                REQUIRE(game.getPaddle().getY() == Catch::Approx(expected));
            }
        }
    }
}

SCENARIO("Paddle moves left at correct speed (AC-02-b)") {
    GIVEN("the game is in PLAYING state") {
        Paddle p;
        float startX = p.getX();
        WHEN("the left key is held for one frame") {
            float dt = 1.0f / TARGET_FPS;
            p.moveLeft(dt);
            THEN("the paddle moves left by PADDLE_SPEED * dt") {
                REQUIRE(p.getX() == Catch::Approx(startX - PADDLE_SPEED * dt));
            }
        }
    }
}

SCENARIO("Paddle moves right at correct speed (AC-02-c)") {
    GIVEN("the game is in PLAYING state") {
        Paddle p;
        float startX = p.getX();
        WHEN("the right key is held for one frame") {
            float dt = 1.0f / TARGET_FPS;
            p.moveRight(dt);
            THEN("the paddle moves right by PADDLE_SPEED * dt") {
                REQUIRE(p.getX() == Catch::Approx(startX + PADDLE_SPEED * dt));
            }
        }
    }
}

SCENARIO("Paddle clamped at left edge (AC-02-d)") {
    GIVEN("the paddle is at the left screen edge") {
        Paddle p;
        for (int i = 0; i < 200; ++i) p.moveLeft(0.1f);
        WHEN("the left key is held") {
            p.moveLeft(0.1f);
            THEN("the paddle x position stays at 0") {
                REQUIRE(p.getX() == Catch::Approx(0.0f));
            }
        }
    }
}

SCENARIO("Paddle clamped at right edge (AC-02-e)") {
    GIVEN("the paddle is at the right screen edge") {
        Paddle p;
        for (int i = 0; i < 200; ++i) p.moveRight(0.1f);
        WHEN("the right key is held") {
            p.moveRight(0.1f);
            THEN("the paddle x does not exceed SCREEN_WIDTH - PADDLE_WIDTH") {
                float maxX = SCREEN_WIDTH - PADDLE_WIDTH;
                REQUIRE(p.getX() <= maxX);
            }
        }
    }
}
