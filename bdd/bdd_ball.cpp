#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "bdd_helpers.h"
#include <cmath>

// Mirrors features/ball.feature

SCENARIO("Ball starts at correct position and velocity (AC-03-a)") {
    GIVEN("the game has transitioned to PLAYING state") {
        BddGame game;
        game.tapSpace();
        const Ball& ball = game.getBall();
        THEN("the ball is centred horizontally") {
            REQUIRE(ball.getPosition().x == Catch::Approx(SCREEN_WIDTH / 2.0f));
        }
        THEN("the ball is 60px above the paddle") {
            float paddleY = game.getPaddle().getY();
            REQUIRE(paddleY - ball.getPosition().y == Catch::Approx(BALL_Y_ABOVE_PADDLE));
        }
        THEN("the ball moves at 45 degrees up-right") {
            REQUIRE(ball.getVelocity().x > 0.0f);
            REQUIRE(ball.getVelocity().y < 0.0f);
            REQUIRE(std::abs(ball.getVelocity().x) ==
                    Catch::Approx(std::abs(ball.getVelocity().y)));
        }
        THEN("the ball speed is 300 px/sec") {
            REQUIRE(ball.getSpeed() == Catch::Approx(BALL_SPEED));
        }
    }
}

SCENARIO("Ball reflects off left wall (AC-03-b)") {
    GIVEN("the ball is moving left") {
        Ball ball;
        ball.setVelocity({-300.0f, -100.0f});
        AND_GIVEN("the left edge is at x = 0") {
            ball.setPosition({BALL_RADIUS, 300.0f});
            WHEN("the collision is detected") {
                ball.update(0.016f);
                THEN("the ball's x velocity is negated and it moves rightward") {
                    REQUIRE(ball.getVelocity().x > 0.0f);
                }
            }
        }
    }
}

SCENARIO("Ball reflects off right wall (AC-03-c)") {
    GIVEN("the ball is moving right") {
        Ball ball;
        ball.setVelocity({300.0f, -100.0f});
        AND_GIVEN("the right edge is at x = SCREEN_WIDTH") {
            ball.setPosition({SCREEN_WIDTH - BALL_RADIUS, 300.0f});
            WHEN("the collision is detected") {
                ball.update(0.016f);
                THEN("the ball's x velocity is negated and it moves leftward") {
                    REQUIRE(ball.getVelocity().x < 0.0f);
                }
            }
        }
    }
}

SCENARIO("Ball reflects off top wall (AC-03-d)") {
    GIVEN("the ball is moving upward") {
        Ball ball;
        ball.setVelocity({100.0f, -300.0f});
        AND_GIVEN("the top edge is at y = 0") {
            ball.setPosition({400.0f, BALL_RADIUS});
            WHEN("the collision is detected") {
                ball.update(0.016f);
                THEN("the ball's y velocity is negated and it moves downward") {
                    REQUIRE(ball.getVelocity().y > 0.0f);
                }
            }
        }
    }
}

SCENARIO("Ball reflects off paddle (AC-03-e)") {
    GIVEN("the ball is moving downward toward the paddle") {
        BddGame game;
        game.tapSpace();
        const Paddle& p = game.getPaddle();
        Ball& ball = const_cast<Ball&>(game.getBall());
        ball.setPosition({p.getX() + p.getWidth() / 2.0f,
                          p.getY() - BALL_RADIUS + 1.0f});
        ball.setVelocity({0.0f, 300.0f});
        WHEN("the ball overlaps the paddle") {
            game.update(0.016f);
            THEN("the ball's y velocity is negated and it moves upward") {
                REQUIRE(game.getBall().getVelocity().y < 0.0f);
            }
        }
    }
}

SCENARIO("Ball exits bottom triggers game over (AC-03-f)") {
    GIVEN("the ball is moving downward") {
        BddGame game;
        game.tapSpace();
        Ball& ball = const_cast<Ball&>(game.getBall());
        WHEN("the ball's bottom edge exceeds the screen height") {
            ball.setPosition({400.0f, static_cast<float>(SCREEN_HEIGHT) + 20.0f});
            game.update(0.016f);
            THEN("the game transitions to GAME_OVER") {
                REQUIRE(game.getState() == GameState::GAME_OVER);
            }
        }
    }
}
