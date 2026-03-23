#include <doctest/doctest.h>
#include "../src/ScoreManager.h"
#include "../src/Constants.h"

// AC-06-a: starts at 0
TEST_CASE("Score initialises to 0") {
    ScoreManager sm;
    CHECK(sm.getScore() == 0);
}

// AC-06-b: +10 per brick
TEST_CASE("Score increases by SCORE_PER_BRICK per brick") {
    ScoreManager sm;
    sm.addBrickScore();
    CHECK(sm.getScore() == SCORE_PER_BRICK);
}

TEST_CASE("Score accumulates correctly over multiple bricks") {
    ScoreManager sm;
    sm.addBrickScore();
    sm.addBrickScore();
    sm.addBrickScore();
    CHECK(sm.getScore() == SCORE_PER_BRICK * 3);
}

// AC-06-c: formatted display
TEST_CASE("Formatted score starts as 'Score: 0'") {
    ScoreManager sm;
    CHECK(sm.getFormattedScore() == "Score: 0");
}

TEST_CASE("Formatted score updates after brick destroyed") {
    ScoreManager sm;
    sm.addBrickScore();
    CHECK(sm.getFormattedScore() == "Score: 10");
}

// AC-07-c / AC-08-c: reset
TEST_CASE("Score resets to 0") {
    ScoreManager sm;
    sm.addBrickScore();
    sm.addBrickScore();
    sm.reset();
    CHECK(sm.getScore() == 0);
}
