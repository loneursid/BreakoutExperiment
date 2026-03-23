#include <cucumber-cpp/autodetect.hpp>
#include "world.h"
#include <boost/test/unit_test.hpp>
#include <cmath>

GIVEN("^the game has transitioned to PLAYING state$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->transitionToPlaying();
})

WHEN("^the first update tick runs$", []() {
    // Ball already reset by transitionToPlaying; no update needed
})

THEN("^the ball is centred horizontally$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_CLOSE(ctx->game.getBall().getPosition().x,
                      static_cast<float>(SCREEN_WIDTH) / 2.0f, 0.01f);
})

THEN("^the ball is 60 pixels above the paddle$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    float paddleY = ctx->game.getPaddle().getY();
    float ballY   = ctx->game.getBall().getPosition().y;
    BOOST_CHECK_CLOSE(paddleY - ballY, BALL_Y_ABOVE_PADDLE, 0.01f);
})

THEN("^the ball is moving at 45 degrees up-right$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    auto vel = ctx->game.getBall().getVelocity();
    BOOST_CHECK_GT(vel.x, 0.0f);
    BOOST_CHECK_LT(vel.y, 0.0f);
    BOOST_CHECK_CLOSE(std::abs(vel.x), std::abs(vel.y), 0.01f);
})

THEN("^the ball speed is 300 pixels per second$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_CLOSE(ctx->game.getBall().getSpeed(), BALL_SPEED, 0.01f);
})

GIVEN("^the ball is moving left$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->ball.setVelocity({-300.0f, 0.0f});
})

GIVEN("^the ball's left edge is at x equals (\\d+)$", [](int x) {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->ball.setPosition({static_cast<float>(x) + BALL_RADIUS, 300.0f});
})

WHEN("^the collision is detected$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->ball.update(0.016f);
})

THEN("^the ball's x velocity is negated$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_GT(ctx->ball.getVelocity().x, 0.0f);
})

THEN("^the ball moves rightward$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_GT(ctx->ball.getVelocity().x, 0.0f);
})

GIVEN("^the ball is moving right$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->ball.setVelocity({300.0f, 0.0f});
})

GIVEN("^the ball's right edge is at x equals (\\d+)$", [](int x) {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->ball.setPosition({static_cast<float>(x) - BALL_RADIUS, 300.0f});
})

THEN("^the ball moves leftward$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_LT(ctx->ball.getVelocity().x, 0.0f);
})

GIVEN("^the ball is moving upward$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->ball.setVelocity({0.0f, -300.0f});
})

GIVEN("^the ball's top edge is at y equals (\\d+)$", [](int y) {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->ball.setPosition({400.0f, static_cast<float>(y) + BALL_RADIUS});
})

THEN("^the ball's y velocity is negated$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_GT(ctx->ball.getVelocity().y, 0.0f);
})

THEN("^the ball moves downward$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_GT(ctx->ball.getVelocity().y, 0.0f);
})

GIVEN("^the ball is moving downward$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->ball.setVelocity({0.0f, 300.0f});
    ctx->transitionToPlaying();
    const_cast<Ball&>(ctx->game.getBall()).setVelocity({0.0f, 300.0f});
})

GIVEN("^the ball's bounding circle overlaps the paddle rect$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    const Paddle& p = ctx->game.getPaddle();
    float cx = p.getX() + p.getWidth() / 2.0f;
    float cy = p.getY() - BALL_RADIUS + 1.0f;
    const_cast<Ball&>(ctx->game.getBall()).setPosition({cx, cy});
})

THEN("^the ball moves upward$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->game.update(0.016f);
    BOOST_CHECK_LT(ctx->game.getBall().getVelocity().y, 0.0f);
})

WHEN("^the ball's bottom edge exceeds the screen height$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    const_cast<Ball&>(ctx->game.getBall()).setPosition(
        {400.0f, static_cast<float>(SCREEN_HEIGHT) + 20.0f});
    ctx->game.update(0.016f);
})

THEN("^the game transitions to GAME_OVER state$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK(ctx->game.getState() == GameState::GAME_OVER);
})
