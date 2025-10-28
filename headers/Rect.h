#ifndef RECT
#define RECT

#include "Object.h"

class Rect : public object
{

public:
    Rect(const float& x,  const float& y, 
         const float& vx, const float& vy,
         const float& m,  
         const float& width, const float& height);

    Rect(const Vector2& coord, 
         const Vector2& vel, 
         const float& m, 
         const float& width, const float& height);
    
    float get_width() override;
    float get_height() override;
    void display();
    
private:
    float width = 0.0; 
    float height = 0.0; 
};

#endif 