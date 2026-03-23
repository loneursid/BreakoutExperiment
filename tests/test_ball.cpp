#include <doctest/doctest.h>
#include "../src/Ball.h"
#include "../src/Constants.h"
#include <cmath>

// AC-03-a: correct start position and velocity
TEST_CASE("Ball resets to correct position and 45-degree velocity") {
    Ball ball;
    ball.reset();

    float paddleY = static_cast<float>(SCREEN_HEIGHT) - PADDLE_Y_OFFSET - PADDLE_HEIGHT;
    float expectedY = paddleY - BALL_Y_ABOVE_PADDLE;

    CHECK(ball.getPosition().x == doctest::Approx(SCREEN_WIDTH / 2.0f));
    CHECK(ball.getPosition().y == doctest::Approx(expectedY));

    // 45 degrees up-right: vx > 0, vy < 0, |vx| == |vy|
    CHECK(ball.getVelocity().x > 0.0f);
    CHECK(ball.getVelocity().y < 0.0f);
    CHECK(ball.getVelocity().x == doctest::Approx(ball.getVelocity().y * -1.0f));

    CHECK(ball.getSpeed() == doctest::Approx(BALL_SPEED));
}

// AC-03-b: reflects off left wall
TEST_CASE("Ball reflects off left wall") {
    Ball ball;
    ball.setPosition({BALL_RADIUS, 300.0f});
    ball.setVelocity({-300.0f, -300.0f});

    ball.update(0.016f);

    CHECK(ball.getVelocity().x > 0.0f);
    CHECK(ball.getVelocity().y == doctest::Approx(-300.0f));
}

// AC-03-c: reflects off right wall
TEST_CASE("Ball reflects off right wall") {
    Ball ball;
    ball.setPosition({static_cast<float>(SCREEN_WIDTH) - BALL_RADIUS, 300.0f});
    ball.setVelocity({300.0f, -300.0f});

    ball.update(0.016f);

    CHECK(ball.getVelocity().x < 0.0f);
    CHECK(ball.getVelocity().y == doctest::Approx(-300.0f));
}

// AC-03-d: reflects off top wall
TEST_CASE("Ball reflects off top wall") {
    Ball ball;
    ball.setPosition({400.0f, BALL_RADIUS});
    ball.setVelocity({300.0f, -300.0f});

    ball.update(0.016f);

    CHECK(ball.getVelocity().y > 0.0f);
    CHECK(ball.getVelocity().x == doctest::Approx(300.0f));
}

// AC-05-c: speed magnitude preserved after wall reflection
TEST_CASE("Ball speed is preserved after wall reflection") {
    Ball ball;
    ball.setPosition({BALL_RADIUS, 300.0f});
    ball.setVelocity({-300.0f, -300.0f});
    float speedBefore = ball.getSpeed();

    ball.update(0.016f);

    CHECK(ball.getSpeed() == doctest::Approx(speedBefore));
}

// reflectX / reflectY helpers
TEST_CASE("reflectX negates x velocity") {
    Ball ball;
    ball.setVelocity({-200.0f, 100.0f});
    ball.reflectX();
    CHECK(ball.getVelocity().x == doctest::Approx(200.0f));
    CHECK(ball.getVelocity().y == doctest::Approx(100.0f));
}

TEST_CASE("reflectY negates y velocity") {
    Ball ball;
    ball.setVelocity({200.0f, -100.0f});
    ball.reflectY();
    CHECK(ball.getVelocity().y == doctest::Approx(100.0f));
    CHECK(ball.getVelocity().x == doctest::Approx(200.0f));
}
