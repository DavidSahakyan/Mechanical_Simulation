#include "Sphere.h"

void sphere::display() 
{   
    Vector2 temp = object::get_coord();
    DrawCircle(temp.x, temp.y, rad, object::get_color());
}

float sphere::get_width() 
{
    return 2 * this -> rad;
}

float sphere::get_height()
{
    return 2 * this -> rad;
}