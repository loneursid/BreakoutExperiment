#include <doctest/doctest.h>
#include "../src/CollisionDetector.h"
#include "../src/Ball.h"
#include "../src/Paddle.h"
#include "../src/Brick.h"
#include "../src/Constants.h"

// AC-03-e: ball vs paddle
TEST_CASE("Ball collides with paddle from above (Y axis)") {
    Paddle p;
    p.reset();

    Ball b;
    // Place ball just above paddle centre, moving down
    b.setPosition({p.getX() + p.getWidth() / 2.0f,
                   p.getY() - b.getRadius() + 1.0f});
    b.setVelocity({0.0f, 300.0f});

    CollisionAxis axis = CollisionDetector::ballVsPaddle(b, p);
    CHECK(axis == CollisionAxis::Y);
}

TEST_CASE("Ball does not collide with paddle when far away") {
    Paddle p;
    p.reset();

    Ball b;
    b.setPosition({400.0f, 100.0f});  // well above paddle

    CollisionAxis axis = CollisionDetector::ballVsPaddle(b, p);
    CHECK(axis == CollisionAxis::None);
}

// AC-04-d: ball vs brick
TEST_CASE("Ball collides with brick from below (Y axis)") {
    Brick brick(100.0f, 100.0f, {255, 0, 0, 255});
    Ball b;
    b.setPosition({brick.getX() + brick.getWidth() / 2.0f,
                   brick.getY() + brick.getHeight() + b.getRadius() - 1.0f});
    b.setVelocity({0.0f, -300.0f});

    CollisionAxis axis = CollisionDetector::ballVsBrick(b, brick);
    CHECK(axis != CollisionAxis::None);
}

TEST_CASE("Ball does not collide with inactive brick") {
    Brick brick(100.0f, 100.0f, {255, 0, 0, 255});
    brick.hit();  // deactivate

    Ball b;
    b.setPosition({brick.getX() + brick.getWidth() / 2.0f,
                   brick.getY() + brick.getHeight() / 2.0f});

    CollisionAxis axis = CollisionDetector::ballVsBrick(b, brick);
    CHECK(axis == CollisionAxis::None);
}

// AC-03-f: exit bottom
TEST_CASE("Ball exiting bottom returns true") {
    Ball b;
    b.setPosition({400.0f, static_cast<float>(SCREEN_HEIGHT) + 10.0f});

    CHECK(CollisionDetector::ballExitedBottom(b, static_cast<float>(SCREEN_HEIGHT)));
}

TEST_CASE("Ball inside screen does not trigger exit bottom") {
    Ball b;
    b.setPosition({400.0f, static_cast<float>(SCREEN_HEIGHT) - 50.0f});

    CHECK_FALSE(CollisionDetector::ballExitedBottom(b, static_cast<float>(SCREEN_HEIGHT)));
}
