#ifndef SPHERE
#define SPHERE
#include "Object.h"

class sphere : public object
{
public:
    inline sphere(const float& x,  const float& y, 
                  const float& vx, const float& vy, 
                  const float& m,  const float& radius) : object(x, y, vx, vy, m), rad(radius)
    {}

    inline sphere(const Vector2& coord, 
                  const Vector2& speed, 
                  const float& m, const float& radius) : object(coord, speed, m), rad(radius)
    {}    

    void display() override;
    float get_width() override;
    float get_height() override;

private:
    float rad = 0.0;
};

#endif