#include "Connection.h"
#include <math.h>

Connection::Connection(object* obj, Vector2 connection_point) : obj(obj), 
                                                                point(connection_point)
{}

Connection::Connection(object* obj, float connection_point_x, float connection_point_y) : 
            Connection(obj, Vector2{connection_point_x, connection_point_y})
{}

float Connection::distance()
{
    Vector2 point1 = this -> obj -> get_coord();
    Vector2 point2 = this -> point;
    
    return sqrt(((point1.x - point2.x) * (point1.x - point2.x)) + 
                ((point1.y - point2.y) * (point1.y - point2.y)));

    
}

void Connection::display()
{
    DrawLine(this -> obj -> get_coord().x, this -> obj -> get_coord().y,
             this -> point.x, this -> point.y, RED);
}

float Connection::cos_with_x()
{
    return (this -> obj -> get_coord().x - this -> point.x) / this -> distance();
}

float Connection::sin_with_x()
{   
    return (this -> point.y - this -> obj -> get_coord().y) / this -> distance();
}

object* Connection::get_object()
{
    return  this -> obj;
}