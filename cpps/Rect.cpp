#include "Rect.h"

void Rect::display() 
{   
    Vector2 temp = object::get_coord();
    DrawRectangle(temp.x - this -> width / 2, 
                  temp.y - this -> height / 2, 
                  this -> width, 
                  this -> height,
                  object::get_color());

    if(!this -> get_charge())
    {
        return;
    }

    DrawLine(temp.x - this -> width / 3, temp.y, 
             temp.x + this -> width / 3, temp.y, 
             BLACK);

    if(this -> get_charge() > 0)
    {
        DrawLine(temp.x, temp.y - this -> width / 3, 
                 temp.x, temp.y + this -> width / 3, 
                 BLACK);
    }
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
           const float& m,  const int& charge,
           const float& width, const float& height) : object(x, y, vx , vy, m, charge), width(width), height(height)
{

}

Rect::Rect(const Vector2& coord, 
           const Vector2& vel, 
           const float& m, const int& charge,
           const float& width, const float& height) : object(coord, vel, m, charge), width(width), height(height)
{

}