#include <doctest/doctest.h>
#include "../src/Game.h"
#include "../src/Constants.h"

// Testable subclass that lets tests drive input
class TestGame : public Game {
public:
    bool left = false, right = false, space = false, restart = false;

    bool isLeftPressed()    const override { return left;    }
    bool isRightPressed()   const override { return right;   }
    bool isSpacePressed()   const override { return space;   }
    bool isRestartPressed() const override { return restart; }
    bool isMutePressed()    const override { return false;   }
};

// AC-01-b / AC-01-d: starts in START, SPACE transitions to PLAYING
TEST_CASE("Game starts in START state") {
    TestGame g;
    CHECK(g.getState() == GameState::START);
}

TEST_CASE("SPACE transitions from START to PLAYING") {
    TestGame g;
    g.space = true;
    g.handleInput();
    CHECK(g.getState() == GameState::PLAYING);
}

// AC-01-c: other keys don't change state
TEST_CASE("Non-SPACE key does not leave START state") {
    TestGame g;
    g.left = true;
    g.handleInput();
    CHECK(g.getState() == GameState::START);
}

// AC-04-a: 50 active bricks on start
TEST_CASE("Exactly 50 bricks are active when PLAYING begins") {
    TestGame g;
    g.space = true;
    g.handleInput();
    CHECK(g.activeBrickCount() == BRICK_ROWS * BRICK_COLS);
}

// AC-03-f / AC-07-a: game over when ball exits bottom
TEST_CASE("Game transitions to GAME_OVER when ball exits bottom") {
    TestGame g;
    g.space = true;
    g.handleInput();

    // Push ball below screen
    const_cast<Ball&>(g.getBall()).setPosition(
        {400.0f, static_cast<float>(SCREEN_HEIGHT) + 20.0f});

    g.update(0.016f);
    CHECK(g.getState() == GameState::GAME_OVER);
}

// AC-08-a: win when all bricks destroyed
TEST_CASE("Game transitions to WIN when all bricks are destroyed") {
    TestGame g;
    g.space = true;
    g.handleInput();

    // Manually destroy all bricks
    for (auto& b : const_cast<std::vector<Brick>&>(g.getBricks()))
        b.hit();

    g.update(0.016f);
    CHECK(g.getState() == GameState::WIN);
}

// AC-07-c / AC-08-c: R key resets to START
TEST_CASE("R key resets game to START from GAME_OVER") {
    TestGame g;
    g.space = true;
    g.handleInput();
    const_cast<Ball&>(g.getBall()).setPosition(
        {400.0f, static_cast<float>(SCREEN_HEIGHT) + 20.0f});
    g.update(0.016f);
    CHECK(g.getState() == GameState::GAME_OVER);

    g.restart = true;
    g.handleInput();
    CHECK(g.getState() == GameState::START);
}

// AC-06-a: score is 0 at start of PLAYING
TEST_CASE("Score is 0 when PLAYING begins") {
    TestGame g;
    g.space = true;
    g.handleInput();
    CHECK(g.getScoreManager().getScore() == 0);
}
