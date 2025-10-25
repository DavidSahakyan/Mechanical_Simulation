#include "Connection.h"
#include <math.h>

Connection::Connection(object* obj1, object* obj2, float max_len) : obj1(obj1), 
                                                                    obj2(obj2), 
                                                                    to_body(true),
                                                                    is_hard(!max_len)

{
    if(max_len == -1)
    {
        this -> max_len = this -> calculate_distance(obj1 -> get_coord(), 
                                                     obj2 -> get_coord());
    }
}

Connection::Connection(object* obj1, Vector2 connection_point, float max_len) : obj1(obj1), 
                                                                                point(connection_point),
                                                                                to_body(false),
                                                                                is_hard(!max_len) 
{
    if(max_len == -1)
    {
        this -> max_len = this -> calculate_distance(obj1 -> get_coord(), 
                                                     point);
    }
}

Connection::Connection(object* obj1, float connection_point_x, float connection_point_y, float max_len) : 
            Connection(obj1, Vector2{connection_point_x, connection_point_y}, max_len)
{}

float Connection::calculate_distance(const Vector2& point1, const Vector2& point2)
{
    float dist = sqrt(abs(((point1.x - point2.x) * (point1.x - point2.x)) + 
                          ((point1.y - point2.y) * (point1.y - point2.y))));
    
    return dist;
}

void Connection::display()
{
    if(this -> to_body)
    {
        DrawLine(this -> obj1 -> get_coord().x, this -> obj1 -> get_coord().y,
                 this -> obj2 -> get_coord().x, this -> obj2 -> get_coord().y, 
                 RED);
    }
    else
    {
        DrawLine(this -> obj1 -> get_coord().x, this -> obj1 -> get_coord().y,
                 this -> point.x, this -> point.y, RED);
    }
}