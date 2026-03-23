#pragma once
#include "Types.h"

class Ball {
public:
    Ball();

    void reset();
    void update(float dt);

    void        setPosition(Vector2 pos);
    void        setVelocity(Vector2 vel);
    Vector2     getPosition() const;
    Vector2     getVelocity() const;
    float       getRadius()   const;
    float       getSpeed()    const;

    // Wall reflection helpers (used by Game / tests)
    void reflectX();
    void reflectY();

private:
    Vector2 m_pos;
    Vector2 m_vel;
    float   m_radius;
};
