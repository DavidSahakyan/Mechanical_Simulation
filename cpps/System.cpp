#include <math.h>
#include <iostream>

#include "System.h"

static System* s = nullptr;

System::System()
{}

void System::add_body(object* obj)
{   
    if(check_validity(obj))
    {
        this -> object_list.push_back(obj);
    }
}

void System::add_body(object& obj)
{
    if(check_validity(&obj))
    {
        this -> add_body(&obj);
    }
}

void System::add_bodies(const std::vector<object*>& obj_list)
{
    for(auto i: obj_list)
    {
        if(check_validity(i))
        {
            this -> add_body(i);
        }
    }
} 

void System::set_global_acceleration(const Vector2& acceleration)
{
    this -> global_acceleration = acceleration;
}

void System::set_global_acceleration(const float& acceleration_x, const float& acceleration_y)
{
    this -> global_acceleration = Vector2{acceleration_x, acceleration_y};
}


System* System::get_singleton()
{
    if(s == nullptr)
    {
        s = new System();
    }
    return s;
} 

void System::calculate_global_gravitational_forces()
{
    for(int i = 0; i < this -> object_list.size(); ++i)
    {
        this -> object_list[i] -> set_force(this -> object_list[i] -> get_force().x + this -> object_list[i] -> get_mass() * this -> global_acceleration.x, 
                                            this -> object_list[i] -> get_force().y + this -> object_list[i] -> get_mass() * this -> global_acceleration.y);
    }
}

void System::calculate_local_gravitational_forces()
{
    float grav_force = 0;
   
    for(int i = 0; i < this -> object_list.size() - 1; ++i)
    {
        for(int j = i + 1; j < this -> object_list.size(); ++j)
        {
            grav_force = (this -> object_list[i] -> get_mass() * this -> object_list[j] -> get_mass()) /
                         (this -> calculate_distance(this -> object_list[i] -> get_coord(), this -> object_list[j] -> get_coord()) * 
                          this -> calculate_distance(this -> object_list[i] -> get_coord(), this -> object_list[j] -> get_coord()));
            
            float grav_force_x_comp = grav_force * this -> calculate_cos_with_horizon(this -> object_list[i] -> get_coord(), 
                                                                                      this -> object_list[j] -> get_coord());
                                                                              
            float grav_force_y_comp = grav_force * this -> calculate_sin_with_horizon(this -> object_list[i] -> get_coord(), 
                                                                                      this -> object_list[j] -> get_coord());

            if(this -> object_list[i] -> get_coord().x <= this -> object_list[j] -> get_coord().x &&
               this -> object_list[i] -> get_coord().y <= this -> object_list[j] -> get_coord().y)
            {
                this -> object_list[i] -> set_force(this -> object_list[i] -> get_force().x + grav_force_x_comp,
                                                    this -> object_list[i] -> get_force().y + grav_force_y_comp);

                this -> object_list[j] -> set_force(this -> object_list[j] -> get_force().x - grav_force_x_comp,
                                                    this -> object_list[j] -> get_force().y - grav_force_y_comp);
            }
            else if(this -> object_list[i] -> get_coord().x >= this -> object_list[j] -> get_coord().x &&
                    this -> object_list[i] -> get_coord().y <= this -> object_list[j] -> get_coord().y)
            {
                this -> object_list[i] -> set_force(this -> object_list[i] -> get_force().x - grav_force_x_comp, 
                                                    this -> object_list[i] -> get_force().y + grav_force_y_comp);
            
                this -> object_list[j] -> set_force(this -> object_list[j] -> get_force().x + grav_force_x_comp, 
                                                    this -> object_list[j] -> get_force().y - grav_force_y_comp);    
            }
            else if(this -> object_list[i] -> get_coord().x <= this -> object_list[j] -> get_coord().x &&
                    this -> object_list[i] -> get_coord().y >= this -> object_list[j] -> get_coord().y)
            {
                this -> object_list[i] -> set_force(this -> object_list[i] -> get_force().x + grav_force_x_comp, 
                                                    this -> object_list[i] -> get_force().y - grav_force_y_comp);
            
                this -> object_list[j] -> set_force(this -> object_list[j] -> get_force().x - grav_force_x_comp, 
                                                    this -> object_list[j] -> get_force().y + grav_force_y_comp);    
            }
            else if(this -> object_list[i] -> get_coord().x >= this -> object_list[j] -> get_coord().x &&
                    this -> object_list[i] -> get_coord().y >= this -> object_list[j] -> get_coord().y)
            {
                this -> object_list[i] -> set_force(this -> object_list[i] -> get_force().x - grav_force_x_comp, 
                                                    this -> object_list[i] -> get_force().y - grav_force_y_comp);
        
                this -> object_list[j] -> set_force(this -> object_list[j] -> get_force().x + grav_force_x_comp, 
                                                    this -> object_list[j] -> get_force().y + grav_force_y_comp);    
            }                                                                 
        }
    }
}

float System::calculate_cos_with_horizon(const Vector2& point1, const Vector2& point2)
{
    float x_dist = abs(point1.x - point2.x);

    return (x_dist / this -> calculate_distance(point1, point2));
}

float System::calculate_sin_with_horizon(const Vector2& point1, const Vector2& point2)
{
    float y_dist = abs(point1.y - point2.y);

    return (y_dist / this -> calculate_distance(point1, point2));
}

float System::calculate_distance(const Vector2& point1, const Vector2& point2)
{
    float dist = sqrt(abs(((point1.x - point2.x) * (point1.x - point2.x)) + 
                          ((point1.y - point2.y) * (point1.y - point2.y))));
    
    return dist;
}

void System::update_system()
{
    for(int i = 0; i < this -> object_list.size(); ++i)
    {
        this -> object_list[i] -> set_force(0, 0);
    }
    
    this -> handle_collisions();
    this -> calculate_local_gravitational_forces();
    this -> calculate_global_gravitational_forces();
    this -> handle_connections();
    this -> set_gradient();
    this -> draw_display_bar();
    
    for(int i = 0; i < this -> object_list.size(); ++i)
    {
        this -> object_list[i] -> move(this -> dt);
        this -> object_list[i] -> display();
    }
    
    for(int i = 0; i < this -> connection_list.size(); ++i)
    {
        this -> connection_list[i] -> display();
    }
}

uint64_t System::get_collision_number()
{
    return this -> collision_number;    
}

void System::handle_collisions()
{
    for(int i = 0; i < this -> object_list.size(); ++i)
    {
        if(this -> object_list[i] -> get_coord().x + this -> object_list[i] -> get_width() / 2 >= GetMonitorWidth(0))
        {
            this -> object_list[i] -> set_vel(-abs(this -> object_list[i] -> get_vel().x), 
                                                   this -> object_list[i] -> get_vel().y);
            ++this -> collision_number;
        } 
        else if(this -> object_list[i] -> get_coord().x - this -> object_list[i] -> get_width() / 2 <= 0)
        {
            this -> object_list[i] -> set_vel(abs(this -> object_list[i] -> get_vel().x), 
                                                  this -> object_list[i] -> get_vel().y);
            ++this -> collision_number;
        }

        if(this -> object_list[i] -> get_coord().y + this -> object_list[i] -> get_height() / 2 >= GetMonitorHeight(0))
        {
            this -> object_list[i] -> set_vel(this -> object_list[i] -> get_vel().x, 
                                         -abs(this -> object_list[i] -> get_vel().y));
            ++this -> collision_number;
        }
        else if(this -> object_list[i] -> get_coord().y - this -> object_list[i] -> get_height() / 2 <= 0)
        {
            this -> object_list[i] -> set_vel(this -> object_list[i] -> get_vel().x, 
                                          abs(this -> object_list[i] -> get_vel().y));
            ++this -> collision_number;
        }

        if(i == this -> object_list.size() - 1)
        {
            break;
        }
        for (int j = i + 1; j < this -> object_list.size(); ++j)
        {
            float dist_x = abs(this -> object_list[i] -> get_coord().x - 
                               this -> object_list[j] -> get_coord().x); 
            
            float dist_y = abs(this -> object_list[i] -> get_coord().y - 
                               this -> object_list[j] -> get_coord().y);

            float dist = sqrt(dist_x * dist_x + dist_y * dist_y);
            
            if(dist_x <= (this -> object_list[i] -> get_width() / 2 + 
                          this -> object_list[j] -> get_width() / 2) &&
               dist_y <= (this -> object_list[i] -> get_height() / 2 + 
                          this -> object_list[j] -> get_height() / 2))
            {
                float cos_i = (this -> object_list[i] -> get_coord().x -
                               this -> object_list[j] -> get_coord().x) / dist;  
            
                float sin_i = (this -> object_list[i] -> get_coord().y -
                               this -> object_list[j] -> get_coord().y) / dist;

                float cos_j = (this -> object_list[j] -> get_coord().x -
                               this -> object_list[i] -> get_coord().x) / dist;
                
                float sin_j = (this -> object_list[j] -> get_coord().y -
                               this -> object_list[i] -> get_coord().y) / dist;

                float vn_i = this -> object_list[i] -> get_vel().x * cos_i + 
                             this -> object_list[i] -> get_vel().y * sin_i;   

                float vt_i = this -> object_list[i] -> get_vel().y * cos_i - 
                             this -> object_list[i] -> get_vel().x * sin_i;

                float vn_j = this -> object_list[j] -> get_vel().x * cos_j + 
                             this -> object_list[j] -> get_vel().y * sin_j;   

                float vt_j = this -> object_list[j] -> get_vel().y * cos_j - 
                             this -> object_list[j] -> get_vel().x * sin_j;

                float vn_i_after_collision = -(2 * (this -> object_list[j] -> get_mass()) / 
                                                   (this -> object_list[j] -> get_mass() + 
                                                   (this -> object_list[i] -> get_mass())) * vn_j) + 
                                                  ((this -> object_list[i] -> get_mass() - 
                                                    this -> object_list[j] -> get_mass()) /
                                                   (this -> object_list[i] -> get_mass() + 
                                                    this -> object_list[j] -> get_mass())) * vn_i;     

                float vn_j_after_collision = -(2 * (this -> object_list[i] -> get_mass()) / 
                                                   (this -> object_list[i] -> get_mass() + 
                                                   (this -> object_list[j] -> get_mass())) * vn_i) + 
                                                  ((this -> object_list[j] -> get_mass() - 
                                                    this -> object_list[i] -> get_mass()) /
                                                   (this -> object_list[j] -> get_mass() + 
                                                    this -> object_list[i] -> get_mass())) * vn_j;
                
                this -> object_list[i] -> set_vel(vn_i_after_collision * cos_i - vt_i * sin_i, 
                                                  vn_i_after_collision * sin_i + vt_i * cos_i);

                this -> object_list[j] -> set_vel(vn_j_after_collision * cos_j - vt_j * sin_j, 
                                                  vn_j_after_collision * sin_j + vt_j * cos_j);    
          
                ++this -> collision_number;
            }
        }        
    }
}

System::~System()
{
    for(auto i: this -> object_list)
    {
        delete(i);
    }
}


bool System::check_validity(object* obj)
{
    if(obj -> get_mass() < 0)
    {
        std::cerr << "Body No " << this -> object_list.size() << "has negative mass parameter\n";
        return false;
    }

    if(obj -> get_coord().x < obj -> get_width() / 2 || 
       obj -> get_coord().y < obj -> get_height()/ 2 || 
       obj -> get_coord().x > GetMonitorWidth(0)  - obj-> get_width() /2  || 
       obj -> get_coord().y > GetMonitorHeight(0) - obj-> get_height()/2)
    {
        std::cerr << "Body No " << this -> object_list.size() - 1 << " is initialized in the wall\n";
        return false;
    }

    for(int i = 0; i < this -> object_list.size(); ++i)
    {
        float dist_x = abs(this -> object_list[i] -> get_coord().x - 
                           obj -> get_coord().x); 
            
        float dist_y = abs(this -> object_list[i] -> get_coord().y - 
                           obj -> get_coord().y);

        float dist = sqrt(dist_x * dist_x + dist_y * dist_y);
            
        if(dist_x < (this -> object_list[i] -> get_width() / 2 + 
                                        obj -> get_width() / 2) &&
           dist_y < (this -> object_list[i] -> get_height() / 2 + 
                                        obj -> get_height() / 2))
        {
            std::cerr << "Body No " << this -> object_list.size() << " is initialized inside the body No " << i << '\n';
            return false;
        }
    }

    return true;
}

void System::add_connection(object* obj1, Vector2 point)
{
    Connection* temp_connection = new Connection(obj1, point);
    this -> connection_list.push_back(temp_connection);
}

void System::add_connection(unsigned int body_num, float point_x, float point_y)
{
    if(object_list.size() >= body_num)
    {
        this -> add_connection(this -> object_list[body_num - 1], 
                               Vector2{point_x, point_y});
        
        return;
    }
    
    std::cerr << "Connection is set between at least one non-existant element\n";
}

void System::set_gradient()
{
    this -> max_force_sqr = -std::numeric_limits<float>::infinity();
    this -> max_vel_sqr   = -std::numeric_limits<float>::infinity();
    this -> min_force_sqr =  std::numeric_limits<float>::infinity();
    this -> min_vel_sqr   =  std::numeric_limits<float>::infinity();


    for(auto i : object_list)
    {

        float temp_vel_sqr = (i -> get_vel().x * i -> get_vel().x) + 
                             (i -> get_vel().y * i -> get_vel().y);

        float temp_force_sqr = (i -> get_force().x * i -> get_force().x) + 
                               (i -> get_force().y * i -> get_force().y);
                             
        if(temp_vel_sqr > max_vel_sqr)
        {
            max_vel_sqr = temp_vel_sqr;
        }
        if(temp_vel_sqr < min_vel_sqr)
        {
            min_vel_sqr = temp_vel_sqr;
        }

        if(temp_force_sqr > max_force_sqr)
        {
            max_force_sqr = temp_force_sqr;
        }
        if(temp_force_sqr < min_force_sqr)
        {
            min_force_sqr = temp_force_sqr;
        }
    }

    for(auto i : object_list)
    {
        if(this -> display_bar == 1)
        {
            float vel = ((i -> get_vel().x * i -> get_vel().x) + 
                         (i -> get_vel().y * i -> get_vel().y));
                            
            unsigned char color = ((vel - min_vel_sqr) / (max_vel_sqr - min_vel_sqr)) * 254;  
            
            i -> set_color(color, 228, 48, 255);
        }
        else if(this -> display_bar == 2)
        {
            float f = ((i -> get_force().x * i -> get_force().x) + 
                       (i -> get_force().y * i -> get_force().y));
                            
            unsigned char color = ((f - min_force_sqr) / (max_force_sqr - min_force_sqr)) * 254;
            
            i -> set_color(color, 228, 48, 255);
        }
    }
}

void System::set_gradient_system(char system_name)
{
    this -> display_bar = system_name;
}

void System::draw_display_bar()
{
    DrawRectangleGradientV(GetMonitorWidth(0) * 0.9,  GetMonitorHeight(0) * 0.1, 
                           GetMonitorWidth(0) * 0.005, GetMonitorHeight(0) * 0.7, 
                           {255, 228, 48, 255},  {0, 228, 48, 255});

    if(this -> display_bar == 1)
    {
        DrawText("|VELOCITY|", GetMonitorWidth(0) * 0.84,  GetMonitorHeight(0) * 0.05, 50, RED);
        DrawText(std::to_string(sqrt(this-> max_vel_sqr)).c_str(), GetMonitorWidth(0) * 0.91, GetMonitorHeight(0) * 0.1, 20, {255, 228, 48, 255});
        DrawText(std::to_string(sqrt(this-> min_vel_sqr)).c_str(), GetMonitorWidth(0) * 0.91, GetMonitorHeight(0) * 0.78, 20, {0, 228, 48, 255});
    }
    else if(this -> display_bar == 2)
    {
        DrawText("|FORCE|", GetMonitorWidth(0) * 0.84,  GetMonitorHeight(0) * 0.05, 50, RED);
        DrawText(std::to_string(sqrt(this-> max_force_sqr)).c_str(), GetMonitorWidth(0) * 0.91, GetMonitorHeight(0) * 0.1, 20, {255, 228, 48, 255});
        DrawText(std::to_string(sqrt(this-> min_force_sqr)).c_str(), GetMonitorWidth(0) * 0.91, GetMonitorHeight(0) * 0.78, 20, {0, 228, 48, 255});
    }
}

void System::handle_connections()
{
    for(auto i : this -> connection_list)
    {   
        float v_tang = i -> get_object() -> get_vel().x * i -> sin_with_x() +
                       i -> get_object() -> get_vel().y * i -> cos_with_x();
        
        i -> get_object() -> set_vel(v_tang * i -> sin_with_x(),         
                                     v_tang * i -> cos_with_x());
         
    }
}