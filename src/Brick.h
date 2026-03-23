#pragma once

// Colour stored as RGBA bytes so Brick is usable without raylib in tests
struct BrickColor { unsigned char r, g, b, a; };

class Brick {
public:
    Brick();
    Brick(float x, float y, BrickColor color);

    bool  isActive() const;
    void  hit();              // decrement HP; deactivates when HP reaches 0

    float      getX()      const;
    float      getY()      const;
    float      getWidth()  const;
    float      getHeight() const;
    BrickColor getColor()  const;

private:
    float      m_x, m_y;
    float      m_width, m_height;
    BrickColor m_color;
    int        m_hp;
    bool       m_active;
};
