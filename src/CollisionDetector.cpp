#include "CollisionDetector.h"
#include "Constants.h"
#include <cmath>
#include <algorithm>

// Circle-vs-AABB: finds closest point on rect to circle centre,
// returns the axis with least overlap for a correct reflection.
CollisionAxis CollisionDetector::circleVsRect(
    float cx, float cy, float radius,
    float rx, float ry, float rw, float rh)
{
    float closestX = std::max(rx, std::min(cx, rx + rw));
    float closestY = std::max(ry, std::min(cy, ry + rh));

    float dx = cx - closestX;
    float dy = cy - closestY;
    float distSq = dx * dx + dy * dy;

    if (distSq > radius * radius) return CollisionAxis::None;

    // Determine dominant axis: whichever overlap is smaller => that axis bounces
    float overlapX = (rw / 2.0f) + radius - std::abs(cx - (rx + rw / 2.0f));
    float overlapY = (rh / 2.0f) + radius - std::abs(cy - (ry + rh / 2.0f));

    return (overlapX < overlapY) ? CollisionAxis::X : CollisionAxis::Y;
}

CollisionAxis CollisionDetector::ballVsPaddle(const Ball& ball, const Paddle& paddle)
{
    return circleVsRect(ball.getPosition().x, ball.getPosition().y, ball.getRadius(),
                        paddle.getX(), paddle.getY(),
                        paddle.getWidth(), paddle.getHeight());
}

CollisionAxis CollisionDetector::ballVsBrick(const Ball& ball, const Brick& brick)
{
    if (!brick.isActive()) return CollisionAxis::None;
    return circleVsRect(ball.getPosition().x, ball.getPosition().y, ball.getRadius(),
                        brick.getX(), brick.getY(),
                        brick.getWidth(), brick.getHeight());
}

bool CollisionDetector::ballExitedBottom(const Ball& ball, float screenHeight)
{
    return ball.getPosition().y - ball.getRadius() > screenHeight;
}
