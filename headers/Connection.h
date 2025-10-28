#ifndef CONNECTION
#define CONNECTION

#include "Object.h"

class Connection
{
public:
    Connection(object* obj, Vector2 connection_point);
    Connection(object* obj, float connection_point_x, float connection_point_y);
    
    float distance();
    float cos_with_x();
    float sin_with_x();

    void display();
    object* get_object();
        
    ~Connection();
    
private:
    object* obj = nullptr;
    Vector2 point = {};
};

#endif