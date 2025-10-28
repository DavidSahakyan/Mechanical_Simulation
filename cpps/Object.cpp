#include "Object.h"
#include <iostream>

object::object(const float& x, const float& y, 
               const float& vx,const float& vy, 
               const float& m) : coord({x, y}),
                                 speed({vx, vy}), 
                                 mass(m)
{
}

object::object(const Vector2& coord, 
               const Vector2& vel, 
               const float& m) : coord(coord),
                                 speed(vel), 
                                 mass(m) 
{
}

void object::calculate_acceleration()
{
    this -> acceleration.x = this -> force.x / this -> mass;
    this -> acceleration.y = this -> force.y / this -> mass;
}

void object::set_force(const float& fx, const float& fy)
{
    this -> set_force({fx, fy});
}

void object::set_force(const Vector2& force)
{
    this -> force.x = force.x;
    this -> force.y = force.y;
}

void object::set_vel(const float& vx, const float& vy)
{
    this -> speed.x = vx;
    this -> speed.y = vy;
}

void object::set_vel(const Vector2& vel)
{
    this -> set_vel({vel.x, vel.y});
}

void object::move(float dt)
{
    this -> speed.x += this -> acceleration.x * dt;
    this -> speed.y += this -> acceleration.y * dt;
    
    this -> calculate_acceleration();
    this -> coord.x += this -> speed.x * dt;
    this -> coord.y += this -> speed.y * dt;
}

const Vector2& object::get_force()
{
    return this -> force;
}

const Vector2& object::get_coord()
{
    return this -> coord;
}

const Color& object::get_color()
{
    return this -> col;
}

const float& object::get_mass()
{
    return this -> mass;
}

const Vector2& object::get_vel()
{
    return this -> speed;
}

void object::set_color(unsigned char r, unsigned char g, unsigned char b, unsigned char alpha)
{
    this -> col = {r, g, b, alpha};
}