#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "bdd_helpers.h"
#include "CollisionDetector.h"
#include <cmath>

// Mirrors features/collision.feature

SCENARIO("Correct axis reversed for brick hit (AC-05-a)") {
    GIVEN("the ball approaches a brick from the top") {
        Brick brick(100.0f, 100.0f, {255, 0, 0, 255});
        Ball ball;
        ball.setPosition({brick.getX() + brick.getWidth() / 2.0f,
                          brick.getY() - BALL_RADIUS + 1.0f});
        ball.setVelocity({0.0f, 300.0f});
        WHEN("the ball's circle intersects the brick AABB") {
            CollisionAxis axis = CollisionDetector::ballVsBrick(ball, brick);
            THEN("a Y-axis collision is detected") {
                REQUIRE(axis == CollisionAxis::Y);
            }
        }
    }
}

SCENARIO("Paddle hit reflects Y velocity (AC-05-b)") {
    GIVEN("the ball approaches the paddle from above") {
        Paddle paddle;
        Ball ball;
        ball.setPosition({paddle.getX() + paddle.getWidth() / 2.0f,
                          paddle.getY() - BALL_RADIUS + 1.0f});
        ball.setVelocity({0.0f, 300.0f});
        WHEN("the ball's circle intersects the paddle AABB") {
            CollisionAxis axis = CollisionDetector::ballVsPaddle(ball, paddle);
            THEN("a Y-axis collision is detected") {
                REQUIRE(axis == CollisionAxis::Y);
            }
            AND_WHEN("the Y velocity is reflected") {
                ball.reflectY();
                THEN("the ball moves upward") {
                    REQUIRE(ball.getVelocity().y < 0.0f);
                }
            }
        }
    }
}

SCENARIO("Speed magnitude preserved after wall reflection (AC-05-c)") {
    GIVEN("the ball is in motion") {
        Ball ball;
        ball.setPosition({BALL_RADIUS, 300.0f});
        ball.setVelocity({-300.0f, -150.0f});
        float speedBefore = ball.getSpeed();
        WHEN("it contacts the left wall") {
            ball.update(0.016f);
            THEN("the ball speed magnitude is unchanged") {
                REQUIRE(ball.getSpeed() == Catch::Approx(speedBefore));
            }
        }
    }
}
