#include <cucumber-cpp/autodetect.hpp>
#include "world.h"
#include <boost/test/unit_test.hpp>
#include <cmath>

GIVEN("^the ball is approaching a brick from the top$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    // Place ball just above brick centre
    float cx = ctx->brick.getX() + ctx->brick.getWidth() / 2.0f;
    float cy = ctx->brick.getY() - BALL_RADIUS + 1.0f;
    ctx->ball.setPosition({cx, cy});
    ctx->ball.setVelocity({0.0f, 300.0f});
})

WHEN("^the ball's circle intersects the brick's AABB$", []() {
    // Already positioned in Given
})

THEN("^the y velocity component is reversed$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    float vyBefore = ctx->ball.getVelocity().y;
    CollisionAxis axis = CollisionDetector::ballVsBrick(ctx->ball, ctx->brick);
    BOOST_CHECK(axis != CollisionAxis::None);
    if (axis == CollisionAxis::Y) {
        ctx->ball.reflectY();
        BOOST_CHECK_LT(ctx->ball.getVelocity().y, 0.0f);
        BOOST_CHECK_LT(ctx->ball.getVelocity().y, vyBefore);
    } else {
        BOOST_CHECK(false);  // expected Y axis
    }
})

GIVEN("^the ball is approaching the paddle from above$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->paddle.reset();
    float cx = ctx->paddle.getX() + ctx->paddle.getWidth() / 2.0f;
    float cy = ctx->paddle.getY() - BALL_RADIUS + 1.0f;
    ctx->ball.setPosition({cx, cy});
    ctx->ball.setVelocity({0.0f, 300.0f});
})

WHEN("^the ball's circle intersects the paddle's AABB$", []() {
    // Already positioned
})

THEN("^the y velocity is negated$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    CollisionAxis axis = CollisionDetector::ballVsPaddle(ctx->ball, ctx->paddle);
    BOOST_CHECK(axis == CollisionAxis::Y);
    ctx->ball.reflectY();
    BOOST_CHECK_LT(ctx->ball.getVelocity().y, 0.0f);
})

GIVEN("^the ball is in motion$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->ball.setPosition({BALL_RADIUS, 300.0f});
    ctx->ball.setVelocity({-300.0f, -150.0f});
})

WHEN("^it contacts a wall$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->ball.update(0.016f);  // triggers left wall reflection
})

THEN("^the ball's speed magnitude remains constant$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    float expected = std::sqrt(300.0f * 300.0f + 150.0f * 150.0f);
    BOOST_CHECK_CLOSE(ctx->ball.getSpeed(), expected, 0.01f);
})
