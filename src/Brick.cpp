#include "Brick.h"
#include "Constants.h"

Brick::Brick()
    : m_x(0), m_y(0), m_width(BRICK_WIDTH), m_height(BRICK_HEIGHT),
      m_color({255, 255, 255, 255}), m_hp(1), m_active(false)
{}

Brick::Brick(float x, float y, BrickColor color)
    : m_x(x), m_y(y), m_width(BRICK_WIDTH), m_height(BRICK_HEIGHT),
      m_color(color), m_hp(1), m_active(true)
{}

bool  Brick::isActive()  const { return m_active; }
float Brick::getX()      const { return m_x; }
float Brick::getY()      const { return m_y; }
float Brick::getWidth()  const { return m_width; }
float Brick::getHeight() const { return m_height; }
BrickColor Brick::getColor() const { return m_color; }

void Brick::hit()
{
    if (!m_active) return;
    --m_hp;
    if (m_hp <= 0) m_active = false;
}
