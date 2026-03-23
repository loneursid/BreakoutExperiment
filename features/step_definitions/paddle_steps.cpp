#include <cucumber-cpp/autodetect.hpp>
#include "world.h"
#include <boost/test/unit_test.hpp>

GIVEN("^the game enters PLAYING state$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->transitionToPlaying();
})

WHEN("^the first frame renders$", []() {
    // State already set; no additional action
})

THEN("^the paddle is centred horizontally$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    float expected = (static_cast<float>(SCREEN_WIDTH) - PADDLE_WIDTH) / 2.0f;
    BOOST_CHECK_CLOSE(ctx->game.getPaddle().getX(), expected, 0.01f);
})

THEN("^the paddle is 40 pixels above the bottom edge$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    float expected = static_cast<float>(SCREEN_HEIGHT) - PADDLE_Y_OFFSET - PADDLE_HEIGHT;
    BOOST_CHECK_CLOSE(ctx->game.getPaddle().getY(), expected, 0.01f);
})

GIVEN("^the game is in PLAYING state$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->transitionToPlaying();
})

GIVEN("^the paddle is not at the left edge$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_GT(ctx->game.getPaddle().getX(), 0.0f);
})

GIVEN("^the paddle is not at the right edge$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    float maxX = static_cast<float>(SCREEN_WIDTH) - PADDLE_WIDTH;
    BOOST_CHECK_LT(ctx->game.getPaddle().getX(), maxX);
})

WHEN("^the left key is held$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    float before = ctx->game.getPaddle().getX();
    ctx->paddle.reset();
    ctx->paddle.moveLeft(1.0f / static_cast<float>(TARGET_FPS));
    ctx->scoreBefore = static_cast<int>(before);  // reuse field to store startX
})

THEN("^the paddle moves left at 400 pixels per second$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    // Check that a left-move step reduces X by ~PADDLE_SPEED/FPS
    Paddle p;
    p.reset();
    float before = p.getX();
    p.moveLeft(1.0f / static_cast<float>(TARGET_FPS));
    BOOST_CHECK_LT(p.getX(), before);
    BOOST_CHECK_CLOSE(before - p.getX(), PADDLE_SPEED / static_cast<float>(TARGET_FPS), 0.1f);
})

WHEN("^the right key is held$", []() {
    // Action verified in Then
})

THEN("^the paddle moves right at 400 pixels per second$", []() {
    Paddle p;
    p.reset();
    float before = p.getX();
    p.moveRight(1.0f / static_cast<float>(TARGET_FPS));
    BOOST_CHECK_GT(p.getX(), before);
    BOOST_CHECK_CLOSE(p.getX() - before, PADDLE_SPEED / static_cast<float>(TARGET_FPS), 0.1f);
})

GIVEN("^the paddle is at the left screen edge$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    for (int i = 0; i < 200; ++i) ctx->paddle.moveLeft(0.1f);
    BOOST_CHECK_EQUAL(ctx->paddle.getX(), 0.0f);
})

WHEN("^the left key is held$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->paddle.moveLeft(0.1f);
})

THEN("^the paddle x position is 0$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_EQUAL(ctx->paddle.getX(), 0.0f);
})

GIVEN("^the paddle is at the right screen edge$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    for (int i = 0; i < 200; ++i) ctx->paddle.moveRight(0.1f);
    float maxX = static_cast<float>(SCREEN_WIDTH) - PADDLE_WIDTH;
    BOOST_CHECK_CLOSE(ctx->paddle.getX(), maxX, 0.01f);
})

WHEN("^the right key is held$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->paddle.moveRight(0.1f);
})

THEN("^the paddle x position does not exceed the right boundary$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    float maxX = static_cast<float>(SCREEN_WIDTH) - PADDLE_WIDTH;
    BOOST_CHECK_LE(ctx->paddle.getX(), maxX);
})
