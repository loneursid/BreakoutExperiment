#include "Paddle.h"
#include "Constants.h"

Paddle::Paddle()
    : m_width(PADDLE_WIDTH), m_height(PADDLE_HEIGHT), m_speed(PADDLE_SPEED)
{
    reset();
}

void Paddle::reset()
{
    m_x = (static_cast<float>(SCREEN_WIDTH) - m_width) / 2.0f;
    m_y = static_cast<float>(SCREEN_HEIGHT) - PADDLE_Y_OFFSET - m_height;
}

void Paddle::moveLeft(float dt)
{
    m_x -= m_speed * dt;
    if (m_x < 0.0f) m_x = 0.0f;
}

void Paddle::moveRight(float dt)
{
    m_x += m_speed * dt;
    float maxX = static_cast<float>(SCREEN_WIDTH) - m_width;
    if (m_x > maxX) m_x = maxX;
}

float Paddle::getX()      const { return m_x; }
float Paddle::getY()      const { return m_y; }
float Paddle::getWidth()  const { return m_width; }
float Paddle::getHeight() const { return m_height; }
