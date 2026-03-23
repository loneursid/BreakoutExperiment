#include <cucumber-cpp/autodetect.hpp>
#include "world.h"
#include <boost/test/unit_test.hpp>

WHEN("^the brick grid is initialised$", []() {
    // Already done by transitionToPlaying in Given
})

THEN("^exactly 50 bricks are active$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_EQUAL(ctx->game.activeBrickCount(), BRICK_ROWS * BRICK_COLS);
})

GIVEN("^the brick grid is rendered$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->transitionToPlaying();
})

WHEN("^rows are drawn top to bottom$", []() {
    // Verified in Then
})

THEN("^row (\\d+) colour is red$", [](int /*row*/) {
    // Row 0 = red: (230, 41, 55)
    const auto& bricks = ScenarioScope<BreakoutWorld>{}->game.getBricks();
    auto c = bricks[0].getColor();
    BOOST_CHECK_GT(c.r, 200);
    BOOST_CHECK_LT(c.g,  80);
    BOOST_CHECK_LT(c.b,  80);
})

THEN("^row (\\d+) colour is orange$", [](int /*row*/) {
    const auto& bricks = ScenarioScope<BreakoutWorld>{}->game.getBricks();
    auto c = bricks[BRICK_COLS].getColor();  // row 1
    BOOST_CHECK_GT(c.r, 200);
    BOOST_CHECK_GT(c.g, 100);
    BOOST_CHECK_LT(c.b,  30);
})

THEN("^row (\\d+) colour is yellow$", [](int /*row*/) {
    const auto& bricks = ScenarioScope<BreakoutWorld>{}->game.getBricks();
    auto c = bricks[2 * BRICK_COLS].getColor();  // row 2
    BOOST_CHECK_GT(c.r, 200);
    BOOST_CHECK_GT(c.g, 200);
    BOOST_CHECK_LT(c.b,  30);
})

THEN("^row (\\d+) colour is green$", [](int /*row*/) {
    const auto& bricks = ScenarioScope<BreakoutWorld>{}->game.getBricks();
    auto c = bricks[3 * BRICK_COLS].getColor();  // row 3
    BOOST_CHECK_LT(c.r,  30);
    BOOST_CHECK_GT(c.g, 150);
    BOOST_CHECK_LT(c.b,  80);
})

THEN("^row (\\d+) colour is blue$", [](int /*row*/) {
    const auto& bricks = ScenarioScope<BreakoutWorld>{}->game.getBricks();
    auto c = bricks[4 * BRICK_COLS].getColor();  // row 4
    BOOST_CHECK_LT(c.r,  30);
    BOOST_CHECK_LT(c.g, 150);
    BOOST_CHECK_GT(c.b, 150);
})

GIVEN("^an active brick exists$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->transitionToPlaying();
    BOOST_CHECK_GT(ctx->game.activeBrickCount(), 0);
})

GIVEN("^the ball's bounding circle overlaps the brick's AABB$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    // Place ball centred on the first brick
    const auto& bricks = ctx->game.getBricks();
    const Brick& b = bricks[0];
    float cx = b.getX() + b.getWidth() / 2.0f;
    float cy = b.getY() + b.getHeight() / 2.0f;
    const_cast<Ball&>(ctx->game.getBall()).setPosition({cx, cy});
    const_cast<Ball&>(ctx->game.getBall()).setVelocity({0.0f, -300.0f});
})

WHEN("^the collision is processed$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->scoreBefore = ctx->game.getScoreManager().getScore();
    ctx->game.update(0.016f);
})

THEN("^the brick is marked inactive$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_FALSE(ctx->game.getBricks()[0].isActive());
})

THEN("^10 points are added to the score$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_EQUAL(ctx->game.getScoreManager().getScore(),
                      ctx->scoreBefore + SCORE_PER_BRICK);
})
