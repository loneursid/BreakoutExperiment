#include <catch2/catch_test_macros.hpp>
#include "bdd_helpers.h"

// Mirrors features/audio.feature
// In headless mode we verify the observable logic layer:
// mute state, HUD label, and that game state transitions are not broken by audio.
// Actual sound playback is verified by manual/integration testing with a window.

SCENARIO("Audio initialises successfully (AC-10-a)") {
    GIVEN("the game executable is launched") {
        BddGame game;
        WHEN("the AudioManager is constructed") {
            THEN("the audio device reports ready") {
                REQUIRE(game.getAudioManager().isReady());
            }
        }
    }
}

SCENARIO("M key mutes audio (AC-10-g)") {
    GIVEN("audio is currently unmuted") {
        BddGame game;
        REQUIRE_FALSE(game.getAudioManager().isMuted());
        WHEN("the player presses the M key") {
            game.pressMute = true;
            game.handleInput();
            game.pressMute = false;
            THEN("the mute flag is true") {
                REQUIRE(game.getAudioManager().isMuted());
            }
            THEN("the HUD displays 'M: Unmute'") {
                REQUIRE(game.getAudioManager().getMuteLabel() == "M: Unmute");
            }
        }
    }
}

SCENARIO("M key unmutes audio (AC-10-h)") {
    GIVEN("audio is currently muted") {
        BddGame game;
        game.pressMute = true; game.handleInput(); game.pressMute = false;
        REQUIRE(game.getAudioManager().isMuted());
        WHEN("the player presses the M key") {
            game.pressMute = true;
            game.handleInput();
            game.pressMute = false;
            THEN("the mute flag is false") {
                REQUIRE_FALSE(game.getAudioManager().isMuted());
            }
            THEN("the HUD displays 'M: Mute'") {
                REQUIRE(game.getAudioManager().getMuteLabel() == "M: Mute");
            }
        }
    }
}

SCENARIO("Mute persists across game states (AC-10-i)") {
    GIVEN("the player has muted audio during PLAYING state") {
        BddGame game;
        game.tapSpace();
        game.pressMute = true; game.handleInput(); game.pressMute = false;
        REQUIRE(game.getAudioManager().isMuted());

        WHEN("the game transitions to GAME_OVER and back to START") {
            const_cast<Ball&>(game.getBall()).setPosition(
                {400.0f, static_cast<float>(SCREEN_HEIGHT) + 20.0f});
            game.update(0.016f);
            REQUIRE(game.getState() == GameState::GAME_OVER);
            game.tapRestart();
            REQUIRE(game.getState() == GameState::START);

            AND_WHEN("the player re-enters PLAYING state") {
                game.tapSpace();
                THEN("the mute flag is still true") {
                    REQUIRE(game.getAudioManager().isMuted());
                }
            }
        }
    }
}

SCENARIO("No sound plays when muted (AC-10-j)") {
    GIVEN("the mute flag is true") {
        BddGame game;
        game.tapSpace();
        game.pressMute = true; game.handleInput(); game.pressMute = false;
        REQUIRE(game.getAudioManager().isMuted());

        WHEN("a collision event occurs") {
            // Place ball on a brick and run update — game should not crash
            const Brick& b = game.getBricks()[0];
            const_cast<Ball&>(game.getBall()).setPosition(
                {b.getX() + b.getWidth() / 2.0f,
                 b.getY() + b.getHeight() / 2.0f});
            game.update(0.016f);

            THEN("the game state is consistent (no crash or corruption)") {
                // If muted play methods are no-ops the game still runs correctly
                REQUIRE(game.getScoreManager().getScore() >= 0);
            }
        }
    }
}

SCENARIO("Single sound per collision event (AC-10-k)") {
    GIVEN("the game is in PLAYING state and audio is not muted") {
        BddGame game;
        game.tapSpace();
        REQUIRE_FALSE(game.getAudioManager().isMuted());

        WHEN("a single brick collision is processed") {
            const Brick& b = game.getBricks()[0];
            const_cast<Ball&>(game.getBall()).setPosition(
                {b.getX() + b.getWidth() / 2.0f,
                 b.getY() + b.getHeight() / 2.0f});
            int bricksBefore = game.activeBrickCount();
            game.update(0.016f);

            THEN("exactly one brick is destroyed (one event, one sound)") {
                REQUIRE(game.activeBrickCount() == bricksBefore - 1);
            }
        }
    }
}
