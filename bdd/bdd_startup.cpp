#include <catch2/catch_test_macros.hpp>
#include "bdd_helpers.h"

// Mirrors features/startup.feature

SCENARIO("Window opens at correct resolution (AC-01-a)") {
    GIVEN("the executable is launched") {
        THEN("the window width constant is 800") {
            REQUIRE(SCREEN_WIDTH == 800);
        }
        THEN("the window height constant is 600") {
            REQUIRE(SCREEN_HEIGHT == 600);
        }
    }
}

SCENARIO("START screen is the initial state (AC-01-b)") {
    GIVEN("the game has just launched") {
        BddGame game;
        THEN("the game is in START state") {
            REQUIRE(game.getState() == GameState::START);
        }
    }
}

SCENARIO("Non-SPACE key does not start the game (AC-01-c)") {
    GIVEN("the START screen is active") {
        BddGame game;
        WHEN("a non-SPACE key is pressed") {
            game.pressLeft = true;
            game.handleInput();
            THEN("the game state remains START") {
                REQUIRE(game.getState() == GameState::START);
            }
        }
    }
}

SCENARIO("SPACE transitions to PLAYING (AC-01-d)") {
    GIVEN("the START screen is active") {
        BddGame game;
        WHEN("SPACE is pressed") {
            game.tapSpace();
            THEN("the game transitions to PLAYING") {
                REQUIRE(game.getState() == GameState::PLAYING);
            }
        }
    }
}
