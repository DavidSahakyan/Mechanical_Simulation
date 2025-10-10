#ifndef OBJECT
#define OBJECT

#include "raylib.h"

class object{
public:
    object(const float& x,  const float& y, 
           const float& vx, const float& vy,
           const float& fx, const float& fy,
           const float& m,  const Color& color);
    
    object(const Vector2& coord, 
           const Vector2& vel, 
           const Vector2& force,
           const float& m, 
           const Color& color);
    
    void move(float dt);
    void calculate_acceleration();
    
    void set_force(const float& fx, const float& fy);
    void set_force(const Vector2& force);
    
    void set_vel(const float& vx, const float& vy);
    void set_vel(const Vector2& velocity);

    const Vector2& get_force();
    const Vector2& get_coord();
    const Color& get_color();
    const float& get_mass();
    const Vector2& get_vel();
    
    virtual void display() = 0;
    virtual float get_characteristic_size() = 0;

private:
    Vector2 coord        = {0.0, 0.0};
    Vector2 speed        = {0.0, 0.0};
    Vector2 force        = {0.0, 0.0};
    Vector2 acceleration = {0.0, 0.0};
    Color col;
    float mass = 1.0;
};

#endif