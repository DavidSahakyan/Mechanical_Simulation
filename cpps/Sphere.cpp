#include "Sphere.h"

void sphere::display() 
{   
    Vector2 temp = object::get_coord();
    DrawCircle(temp.x, temp.y, rad, object::get_color());
    if(!this -> get_charge())
    {
        return;
    }

    DrawLine(temp.x - this -> rad / 3, temp.y, 
             temp.x + this -> rad / 3, temp.y, 
             BLACK);
    
    if(this -> get_charge() > 0)
    {
        DrawLine(temp.x, temp.y - this -> rad / 3, 
                 temp.x, temp.y + this -> rad / 3, 
                 BLACK);
        
    }
}

float sphere::get_width() 
{
    return 2 * this -> rad;
}

float sphere::get_height()
{
    return 2 * this -> rad;
}