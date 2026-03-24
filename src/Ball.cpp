#include "Ball.h"
#include "Constants.h"
#include <cmath>

Ball::Ball() : m_radius(BALL_RADIUS)
{
    reset();
}

void Ball::reset()
{
    // Centre horizontally, positioned above the paddle start position
    float paddleY = static_cast<float>(SCREEN_HEIGHT) - PADDLE_Y_OFFSET - PADDLE_HEIGHT;
    m_pos = { static_cast<float>(SCREEN_WIDTH) / 2.0f,
              paddleY - BALL_Y_ABOVE_PADDLE };

    // 45-degree angle, up-right  (vx > 0, vy < 0)
    float component = BALL_SPEED / std::sqrt(2.0f);
    m_vel = { component, -component };
}

void Ball::update(float dt)
{
    m_wallHit = false;

    m_pos.x += m_vel.x * dt;
    m_pos.y += m_vel.y * dt;

    // Wall collisions
    if (m_pos.x - m_radius <= 0.0f && m_vel.x < 0.0f) {
        m_pos.x = m_radius;
        reflectX();
        m_wallHit = true;
    }
    if (m_pos.x + m_radius >= static_cast<float>(SCREEN_WIDTH) && m_vel.x > 0.0f) {
        m_pos.x = static_cast<float>(SCREEN_WIDTH) - m_radius;
        reflectX();
        m_wallHit = true;
    }
    if (m_pos.y - m_radius <= 0.0f && m_vel.y < 0.0f) {
        m_pos.y = m_radius;
        reflectY();
        m_wallHit = true;
    }
}

bool Ball::wallHitOccurred() const { return m_wallHit; }

void Ball::setPosition(Vector2 pos) { m_pos = pos; }
void Ball::setVelocity(Vector2 vel) { m_vel = vel; }
Vector2 Ball::getPosition() const   { return m_pos; }
Vector2 Ball::getVelocity() const   { return m_vel; }
float   Ball::getRadius()   const   { return m_radius; }

float Ball::getSpeed() const
{
    return std::sqrt(m_vel.x * m_vel.x + m_vel.y * m_vel.y);
}

void Ball::reflectX() { m_vel.x = -m_vel.x; }
void Ball::reflectY() { m_vel.y = -m_vel.y; }
