#include <catch2/catch_test_macros.hpp>
#include "bdd_helpers.h"

// Mirrors features/game_over.feature and features/win_condition.feature

// ── Game Over ────────────────────────────────────────────────────────────────

SCENARIO("Ball exit triggers game over (AC-07-a)") {
    GIVEN("the ball is moving downward in PLAYING state") {
        BddGame game;
        game.tapSpace();
        WHEN("the ball's bottom edge exceeds the screen height") {
            const_cast<Ball&>(game.getBall()).setPosition(
                {400.0f, static_cast<float>(SCREEN_HEIGHT) + 20.0f});
            game.update(0.016f);
            THEN("the game transitions to GAME_OVER immediately") {
                REQUIRE(game.getState() == GameState::GAME_OVER);
            }
        }
    }
}

SCENARIO("Game over screen data is available (AC-07-b)") {
    GIVEN("the game is in GAME_OVER state") {
        BddGame game;
        game.tapSpace();
        const_cast<Ball&>(game.getBall()).setPosition(
            {400.0f, static_cast<float>(SCREEN_HEIGHT) + 20.0f});
        game.update(0.016f);
        THEN("the game state is GAME_OVER") {
            REQUIRE(game.getState() == GameState::GAME_OVER);
        }
        THEN("the final score is accessible") {
            REQUIRE(game.getScoreManager().getScore() >= 0);
        }
    }
}

SCENARIO("R key resets from GAME_OVER (AC-07-c)") {
    GIVEN("the game is in GAME_OVER state") {
        BddGame game;
        game.tapSpace();
        const_cast<Ball&>(game.getBall()).setPosition(
            {400.0f, static_cast<float>(SCREEN_HEIGHT) + 20.0f});
        game.update(0.016f);
        REQUIRE(game.getState() == GameState::GAME_OVER);
        WHEN("the player presses R") {
            game.tapRestart();
            THEN("the game returns to START") {
                REQUIRE(game.getState() == GameState::START);
            }
            THEN("the score is reset to 0") {
                REQUIRE(game.getScoreManager().getScore() == 0);
            }
            THEN("all 50 bricks are active") {
                REQUIRE(game.activeBrickCount() == BRICK_ROWS * BRICK_COLS);
            }
        }
    }
}

// ── Win Condition ─────────────────────────────────────────────────────────────

SCENARIO("All bricks cleared triggers WIN (AC-08-a)") {
    GIVEN("the game is in PLAYING state") {
        BddGame game;
        game.tapSpace();
        WHEN("the last brick is destroyed") {
            for (auto& b : const_cast<std::vector<Brick>&>(game.getBricks()))
                b.hit();
            game.update(0.016f);
            THEN("the game transitions to WIN immediately") {
                REQUIRE(game.getState() == GameState::WIN);
            }
        }
    }
}

SCENARIO("Win screen data is available (AC-08-b)") {
    GIVEN("the game is in WIN state") {
        BddGame game;
        game.tapSpace();
        for (auto& b : const_cast<std::vector<Brick>&>(game.getBricks()))
            b.hit();
        game.update(0.016f);
        THEN("the game state is WIN") {
            REQUIRE(game.getState() == GameState::WIN);
        }
        THEN("the final score is accessible") {
            REQUIRE(game.getScoreManager().getScore() >= 0);
        }
    }
}

SCENARIO("R key resets from WIN (AC-08-c)") {
    GIVEN("the game is in WIN state") {
        BddGame game;
        game.tapSpace();
        for (auto& b : const_cast<std::vector<Brick>&>(game.getBricks()))
            b.hit();
        game.update(0.016f);
        REQUIRE(game.getState() == GameState::WIN);
        WHEN("the player presses R") {
            game.tapRestart();
            THEN("the game returns to START") {
                REQUIRE(game.getState() == GameState::START);
            }
            THEN("the score is reset to 0") {
                REQUIRE(game.getScoreManager().getScore() == 0);
            }
            THEN("all 50 bricks are active") {
                REQUIRE(game.activeBrickCount() == BRICK_ROWS * BRICK_COLS);
            }
        }
    }
}
