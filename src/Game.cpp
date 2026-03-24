// raylib.h must come first so its Vector2 definition wins over Types.h stubs
#ifndef BREAKOUT_HEADLESS
#  include "raylib.h"
#endif

#include "Game.h"
#include "CollisionDetector.h"
#include "Constants.h"

// Row colours: Red, Orange, Yellow, Green, Blue
static const BrickColor ROW_COLORS[BRICK_ROWS] = {
    {230,  41,  55, 255},  // Red
    {255, 161,   0, 255},  // Orange
    {253, 249,   0, 255},  // Yellow
    { 0,  228,  48, 255},  // Green
    {  0, 121, 241, 255},  // Blue
};

Game::Game() : m_state(GameState::START)
{
    initBricks();
}

void Game::initBricks()
{
    m_bricks.clear();
    m_bricks.reserve(BRICK_ROWS * BRICK_COLS);

    for (int row = 0; row < BRICK_ROWS; ++row) {
        for (int col = 0; col < BRICK_COLS; ++col) {
            float x = BRICK_SIDE_MARGIN + col * (BRICK_WIDTH + BRICK_H_GAP);
            float y = BRICK_TOP_MARGIN  + row * (BRICK_HEIGHT + BRICK_V_GAP);
            m_bricks.emplace_back(x, y, ROW_COLORS[row]);
        }
    }
}

void Game::handleInput()
{
    // Mute toggle available in all states
    if (isMutePressed()) m_audio.toggleMute();

    switch (m_state) {
    case GameState::START:
        if (isSpacePressed()) {
            m_state = GameState::PLAYING;
            m_ball.reset();
            m_paddle.reset();
        }
        break;

    case GameState::PLAYING:
        // Paddle movement handled in update() with real dt
        break;

    case GameState::GAME_OVER:
    case GameState::WIN:
        if (isRestartPressed()) reset();
        break;
    }
}

void Game::update(float dt)
{
    if (m_state != GameState::PLAYING) return;

    if (isLeftPressed())  m_paddle.moveLeft(dt);
    if (isRightPressed()) m_paddle.moveRight(dt);

    m_ball.update(dt);
    if (m_ball.wallHitOccurred()) m_audio.playWallHit();
    processCollisions();

    // Win check
    if (activeBrickCount() == 0) {
        m_audio.playWin();
        m_state = GameState::WIN;
        return;
    }

    // Game over check
    if (CollisionDetector::ballExitedBottom(m_ball, static_cast<float>(SCREEN_HEIGHT))) {
        m_audio.playBallLost();
        m_state = GameState::GAME_OVER;
    }
}

void Game::processCollisions()
{
    // Ball vs paddle
    CollisionAxis axis = CollisionDetector::ballVsPaddle(m_ball, m_paddle);
    if (axis == CollisionAxis::Y && m_ball.getVelocity().y > 0.0f) {
        m_ball.reflectY();
        m_audio.playPaddleHit();

        // Adjust X velocity based on hit offset from paddle centre
        float paddleCentreX = m_paddle.getX() + m_paddle.getWidth() / 2.0f;
        float offset = (m_ball.getPosition().x - paddleCentreX) / (m_paddle.getWidth() / 2.0f);
        Vector2 vel = m_ball.getVelocity();
        vel.x = BALL_SPEED * offset;
        m_ball.setVelocity(vel);
    } else if (axis == CollisionAxis::X) {
        m_ball.reflectX();
        m_audio.playPaddleHit();
    }

    // Ball vs bricks
    for (auto& brick : m_bricks) {
        if (!brick.isActive()) continue;
        CollisionAxis bAxis = CollisionDetector::ballVsBrick(m_ball, brick);
        if (bAxis == CollisionAxis::None) continue;

        brick.hit();
        m_score.addBrickScore();
        m_audio.playBrickHit();

        if (bAxis == CollisionAxis::X) m_ball.reflectX();
        else                            m_ball.reflectY();
        break; // handle one brick per frame to avoid tunnelling artefacts
    }

}

void Game::reset()
{
    m_state = GameState::START;
    m_ball.reset();
    m_paddle.reset();
    m_score.reset();
    initBricks();
    // m_audio mute state intentionally preserved across resets (AC-10-i)
}

GameState                 Game::getState()          const { return m_state; }
const Ball&               Game::getBall()           const { return m_ball; }
const Paddle&             Game::getPaddle()         const { return m_paddle; }
const std::vector<Brick>& Game::getBricks()         const { return m_bricks; }
const ScoreManager&       Game::getScoreManager()   const { return m_score; }
const AudioManager&       Game::getAudioManager()   const { return m_audio; }

int Game::activeBrickCount() const
{
    int count = 0;
    for (const auto& b : m_bricks) if (b.isActive()) ++count;
    return count;
}

// ── Input — real raylib implementation ───────────────────────────────────────
#ifndef BREAKOUT_HEADLESS
bool Game::isLeftPressed()    const { return IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A); }
bool Game::isRightPressed()   const { return IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D); }
bool Game::isSpacePressed()   const { return IsKeyPressed(KEY_SPACE); }
bool Game::isRestartPressed() const { return IsKeyPressed(KEY_R); }
bool Game::isMutePressed()    const { return IsKeyPressed(KEY_M); }
#else
// Headless stubs — overridden in tests / BDD
bool Game::isLeftPressed()    const { return false; }
bool Game::isRightPressed()   const { return false; }
bool Game::isSpacePressed()   const { return false; }
bool Game::isRestartPressed() const { return false; }
bool Game::isMutePressed()    const { return false; }
#endif
