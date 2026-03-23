#pragma once

// Window
constexpr int SCREEN_WIDTH  = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int TARGET_FPS    = 60;

// Paddle
constexpr float PADDLE_WIDTH   = 100.0f;
constexpr float PADDLE_HEIGHT  = 16.0f;
constexpr float PADDLE_SPEED   = 400.0f;
constexpr float PADDLE_Y_OFFSET = 40.0f;   // distance above bottom edge

// Ball
constexpr float BALL_RADIUS        = 8.0f;
constexpr float BALL_SPEED         = 300.0f;
constexpr float BALL_Y_ABOVE_PADDLE = 60.0f; // starting offset above paddle

// Bricks
constexpr int   BRICK_COLS          = 10;
constexpr int   BRICK_ROWS          = 5;
constexpr float BRICK_WIDTH         = 70.0f;
constexpr float BRICK_HEIGHT        = 20.0f;
constexpr float BRICK_H_GAP         = 10.0f;  // horizontal gap between bricks
constexpr float BRICK_V_GAP         = 8.0f;   // vertical gap between rows
constexpr float BRICK_SIDE_MARGIN   = 20.0f;  // left/right screen margin
constexpr float BRICK_TOP_MARGIN    = 60.0f;  // top screen margin

// Scoring
constexpr int SCORE_PER_BRICK = 10;
