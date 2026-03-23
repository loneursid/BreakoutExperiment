#pragma once

class Paddle {
public:
    Paddle();

    void reset();
    void moveLeft(float dt);
    void moveRight(float dt);

    float getX()      const;
    float getY()      const;
    float getWidth()  const;
    float getHeight() const;

private:
    float m_x;
    float m_y;
    float m_width;
    float m_height;
    float m_speed;
};
