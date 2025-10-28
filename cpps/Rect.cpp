#include "Rect.h"

void Rect::display() 
{   
    Vector2 temp = object::get_coord();
    DrawRectangle(temp.x - this -> width / 2, 
                  temp.y - this -> height / 2, 
                  this -> width, 
                  this -> height, object::get_color());
}

float Rect::get_width() 
{
    return this -> width;
}

float Rect::get_height()
{
    return this -> height;
}

Rect::Rect(const float& x,  const float& y, 
           const float& vx, const float& vy,
           const float& m,  
           const float& width, const float& height) : object(x, y, vx , vy, m), width(width), height(height)
{

}

Rect::Rect(const Vector2& coord, 
           const Vector2& vel, 
           const float& m, 
           const float& width, const float& height) : object(coord, vel, m), width(width), height(height)
{

}