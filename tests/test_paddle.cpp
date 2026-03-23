#include <doctest/doctest.h>
#include "../src/Paddle.h"
#include "../src/Constants.h"

// AC-02-a: centred and correct Y on reset
TEST_CASE("Paddle resets to centred position") {
    Paddle p;
    p.reset();

    float expectedX = (static_cast<float>(SCREEN_WIDTH) - PADDLE_WIDTH) / 2.0f;
    float expectedY = static_cast<float>(SCREEN_HEIGHT) - PADDLE_Y_OFFSET - PADDLE_HEIGHT;

    CHECK(p.getX() == doctest::Approx(expectedX));
    CHECK(p.getY() == doctest::Approx(expectedY));
}

// AC-02-b: moves left at correct speed
TEST_CASE("Paddle moves left at PADDLE_SPEED") {
    Paddle p;
    p.reset();
    float startX = p.getX();
    float dt = 0.1f;

    p.moveLeft(dt);

    CHECK(p.getX() == doctest::Approx(startX - PADDLE_SPEED * dt));
}

// AC-02-c: moves right at correct speed
TEST_CASE("Paddle moves right at PADDLE_SPEED") {
    Paddle p;
    p.reset();
    float startX = p.getX();
    float dt = 0.1f;

    p.moveRight(dt);

    CHECK(p.getX() == doctest::Approx(startX + PADDLE_SPEED * dt));
}

// AC-02-d: clamped at left edge
TEST_CASE("Paddle clamped at left screen edge") {
    Paddle p;
    p.reset();

    // Move far left
    for (int i = 0; i < 100; ++i) p.moveLeft(0.1f);

    CHECK(p.getX() >= 0.0f);
    CHECK(p.getX() == doctest::Approx(0.0f));
}

// AC-02-e: clamped at right edge
TEST_CASE("Paddle clamped at right screen edge") {
    Paddle p;
    p.reset();

    // Move far right
    for (int i = 0; i < 100; ++i) p.moveRight(0.1f);

    float maxX = static_cast<float>(SCREEN_WIDTH) - PADDLE_WIDTH;
    CHECK(p.getX() <= maxX);
    CHECK(p.getX() == doctest::Approx(maxX));
}

TEST_CASE("Paddle has correct dimensions") {
    Paddle p;
    CHECK(p.getWidth()  == doctest::Approx(PADDLE_WIDTH));
    CHECK(p.getHeight() == doctest::Approx(PADDLE_HEIGHT));
}
