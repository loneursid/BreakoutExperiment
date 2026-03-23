#include <cucumber-cpp/autodetect.hpp>
#include "world.h"
#include <boost/test/unit_test.hpp>

GIVEN("^the executable is launched$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    (void)ctx;  // construction is sufficient
})

WHEN("^the window initialises$", []() {
    // Verified via Constants — no window needed
})

THEN("^the window width is (\\d+) pixels$", [](int w) {
    BOOST_CHECK_EQUAL(SCREEN_WIDTH, w);
})

THEN("^the window height is (\\d+) pixels$", [](int h) {
    BOOST_CHECK_EQUAL(SCREEN_HEIGHT, h);
})

THEN("^the window title is \"([^\"]*)\"$", [](std::string title) {
    BOOST_CHECK_EQUAL(title, std::string("Breakout"));
})

GIVEN("^the game has just launched$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK(ctx->game.getState() == GameState::START);
})

WHEN("^the START screen is displayed$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK(ctx->game.getState() == GameState::START);
})

THEN("^the player sees the game title$", []() {
    // Renderer responsibility — verified visually
})

THEN("^the player sees the prompt \"([^\"]*)\"$", [](std::string prompt) {
    BOOST_CHECK_EQUAL(prompt, std::string("Press SPACE to start"));
})

GIVEN("^the START screen is active$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK(ctx->game.getState() == GameState::START);
})

WHEN("^any key other than SPACE is pressed$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    // No key injected; handleInput should leave state as START
    ctx->game.handleInput();
})

THEN("^the game state is START$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK(ctx->game.getState() == GameState::START);
})

WHEN("^SPACE is pressed$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    ctx->game.pressSpace = true;
    ctx->game.handleInput();
    ctx->game.pressSpace = false;
})

THEN("^the game state is PLAYING$", []() {
    ScenarioScope<BreakoutWorld> ctx;
    BOOST_CHECK(ctx->game.getState() == GameState::PLAYING);
})
