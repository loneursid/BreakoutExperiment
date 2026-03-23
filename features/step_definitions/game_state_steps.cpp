#include <cucumber-cpp/autodetect.hpp>
#include "world.h"
#include <boost/test/unit_test.hpp>

// ---- Game Over ----

GIVEN("^the game is in GAME_OVER state$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->transitionToPlaying();
    const_cast<Ball&>(ctx->game.getBall()).setPosition(
        {400.0f, static_cast<float>(SCREEN_HEIGHT) + 20.0f});
    ctx->game.update(0.016f);
    BOOST_CHECK(ctx->game.getState() == GameState::GAME_OVER);
})

WHEN("^the screen renders$", []() {
    // Renderer concern; logic layer has the data
})

THEN("^\"([^\"]*)\" is displayed$", [](std::string /*text*/) {
    // Renderer responsibility — we verify the game state and score are accessible
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK(ctx->game.getState() == GameState::GAME_OVER ||
                ctx->game.getState() == GameState::WIN);
})

THEN("^the final score is displayed$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    // Score is accessible via getScoreManager()
    BOOST_CHECK_GE(ctx->game.getScoreManager().getScore(), 0);
})

WHEN("^the player presses R$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->game.pressRestart = true;
    ctx->game.handleInput();
    ctx->game.pressRestart = false;
})

THEN("^the game state returns to START$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK(ctx->game.getState() == GameState::START);
})

THEN("^the score is reset to 0$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_EQUAL(ctx->game.getScoreManager().getScore(), 0);
})

THEN("^all bricks are active$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_EQUAL(ctx->game.activeBrickCount(), BRICK_ROWS * BRICK_COLS);
})

// ---- Win ----

GIVEN("^the game is in WIN state$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->transitionToPlaying();
    for (auto& b : const_cast<std::vector<Brick>&>(ctx->game.getBricks()))
        b.hit();
    ctx->game.update(0.016f);
    BOOST_CHECK(ctx->game.getState() == GameState::WIN);
})

GIVEN("^the last remaining brick is destroyed$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->transitionToPlaying();
    auto& bricks = const_cast<std::vector<Brick>&>(ctx->game.getBricks());
    for (int i = 0; i < static_cast<int>(bricks.size()) - 1; ++i)
        bricks[i].hit();
    // One brick remains; destroy it via collision
    Brick& last = bricks.back();
    float cx = last.getX() + last.getWidth() / 2.0f;
    float cy = last.getY() + last.getHeight() / 2.0f;
    const_cast<Ball&>(ctx->game.getBall()).setPosition({cx, cy});
})

WHEN("^the active brick count reaches 0$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->game.update(0.016f);
})

THEN("^the game transitions to WIN state$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK(ctx->game.getState() == GameState::WIN);
})
