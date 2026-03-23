#include <cucumber-cpp/autodetect.hpp>
#include "world.h"
#include <boost/test/unit_test.hpp>

GIVEN("^a new game starts$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->game.reset();
})

WHEN("^the PLAYING state is entered$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->transitionToPlaying();
})

THEN("^the score is 0$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_EQUAL(ctx->game.getScoreManager().getScore(), 0);
})

WHEN("^a brick is destroyed$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->scoreBefore = ctx->game.getScoreManager().getScore();
    // Position ball on first brick and update
    const auto& bricks = ctx->game.getBricks();
    const Brick& b = bricks[0];
    float cx = b.getX() + b.getWidth() / 2.0f;
    float cy = b.getY() + b.getHeight() / 2.0f;
    const_cast<Ball&>(ctx->game.getBall()).setPosition({cx, cy});
    ctx->game.update(0.016f);
})

THEN("^the score increases by 10 points$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK_EQUAL(ctx->game.getScoreManager().getScore(),
                      ctx->scoreBefore + SCORE_PER_BRICK);
})

WHEN("^any frame renders$", []() {
    // Rendering is a Renderer concern; we just check the score object
})

THEN("^the score label starts with \"([^\"]*)\"$", [](std::string prefix) {
    ScenarioScope<BreakoutWorld> ctx;
    std::string formatted = ctx->game.getScoreManager().getFormattedScore();
    BOOST_CHECK(formatted.rfind(prefix, 0) == 0);
})
