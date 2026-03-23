#include <doctest/doctest.h>
#include "../src/Brick.h"
#include "../src/Constants.h"

TEST_CASE("Brick is active on construction") {
    Brick b(10.0f, 20.0f, {255, 0, 0, 255});
    CHECK(b.isActive());
}

TEST_CASE("Brick has correct dimensions") {
    Brick b(10.0f, 20.0f, {255, 0, 0, 255});
    CHECK(b.getWidth()  == doctest::Approx(BRICK_WIDTH));
    CHECK(b.getHeight() == doctest::Approx(BRICK_HEIGHT));
}

// AC-04-d: brick deactivated after one hit
TEST_CASE("Brick becomes inactive after one hit") {
    Brick b(10.0f, 20.0f, {255, 0, 0, 255});
    b.hit();
    CHECK_FALSE(b.isActive());
}

TEST_CASE("Hitting an inactive brick has no effect") {
    Brick b(10.0f, 20.0f, {255, 0, 0, 255});
    b.hit();   // deactivate
    b.hit();   // should not throw or change state
    CHECK_FALSE(b.isActive());
}

TEST_CASE("Brick stores and returns its colour") {
    BrickColor c{230, 41, 55, 255};
    Brick b(0, 0, c);
    CHECK(b.getColor().r == 230);
    CHECK(b.getColor().g ==  41);
    CHECK(b.getColor().b ==  55);
}

TEST_CASE("Brick stores and returns its position") {
    Brick b(15.0f, 30.0f, {0, 0, 0, 255});
    CHECK(b.getX() == doctest::Approx(15.0f));
    CHECK(b.getY() == doctest::Approx(30.0f));
}
