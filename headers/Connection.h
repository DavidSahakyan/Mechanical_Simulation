#ifndef CONNECTION
#define CONNECTION

#include "Object.h"

class Connection
{
public:
    // max_len 0 MEANS HARD CONNECTION, 
    // -1 MEAN THE MAXIMUM LENGTH WILL BE THE DISTANCE BETWEEN OBJECT-OBJECT OR OBJECT-POINT 
    // AT THE MOMENT OF THE INITIALIZATION
    Connection(object* obj1, object* obj2, float max_len);
    Connection(object* obj1, Vector2 connection_point, float max_len);
    Connection(object* obj1, float connection_point_x, float connection_point_y, float max_len);
    
    void display();
    float calculate_distance(const Vector2& point1, const Vector2& point2);
    
    ~Connection();

private:
    object* obj1 = nullptr;
    object* obj2 = nullptr;
    Vector2 point = {};
    bool is_hard = true;
    bool to_body = false;
    float max_len;
};

#endif