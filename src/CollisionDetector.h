#pragma once

#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"

// Returns the axis of first overlap for circle-vs-AABB
enum class CollisionAxis { None, X, Y };

class CollisionDetector {
public:
    // Returns which axis to reflect on (None = no collision)
    static CollisionAxis ballVsPaddle(const Ball& ball, const Paddle& paddle);

    // Returns which axis to reflect on (None = no collision)
    static CollisionAxis ballVsBrick(const Ball& ball, const Brick& brick);

    // True if ball has exited through the bottom of the screen
    static bool ballExitedBottom(const Ball& ball, float screenHeight);

private:
    // Shared circle-vs-AABB helper
    static CollisionAxis circleVsRect(float cx, float cy, float radius,
                                      float rx, float ry, float rw, float rh);
};
