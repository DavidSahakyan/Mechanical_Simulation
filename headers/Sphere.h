#ifndef SPHERE
#define SPHERE
#include "Object.h"

class sphere : public object
{
public:
    inline sphere(const float& x,  const float& y, 
                  const float& vx, const float& vy, 
                  const float& fx, const float& fy, 
                  const float& m,  const Color& color,
                  const float& radius) : object(x, y, vx, vy, fx, fy, m, color), rad(radius)
    {}

    inline sphere(const Vector2& coord, 
                  const Vector2& speed, 
                  const Vector2& force, 
                  const float& m, const Color& color,
                  const float& radius) : object(coord, speed, force, m, color), rad(radius)
    {}    

    void display() override;
    float get_characteristic_size() override;

private:
    float rad = 0.0;
};

#endif