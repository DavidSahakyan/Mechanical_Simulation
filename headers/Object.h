#ifndef OBJECT
#define OBJECT

#include "raylib.h"

class object{
public:
    object(const float& x,  const float& y, 
           const float& vx, const float& vy,
           const float& m, const int& charge);
    
    object(const Vector2& coord, 
           const Vector2& vel, 
           const float& m, const int& charge);
    
    void move(float dt);
    void calculate_acceleration();
    
    void set_force(const float& fx, const float& fy);
    void set_force(const Vector2& force);
    
    void set_vel(const float& vx, const float& vy);
    void set_vel(const Vector2& velocity);

    void set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha);

    const Vector2& get_force();
    const Vector2& get_coord();
    const Color& get_color();
    const float& get_mass();
    const int& get_charge();
    const Vector2& get_vel();
    
    virtual void display() = 0;
    virtual float get_width() = 0;
    virtual float get_height() = 0;

private:
    Vector2 coord        = {0.0, 0.0};
    Vector2 speed        = {0.0, 0.0};
    Vector2 force        = {0.0, 0.0};
    Vector2 acceleration = {0.0, 0.0};
    Color col = BLUE;
    float mass = 1.0;
    int charge = 0;
};

#endif