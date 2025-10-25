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
    
    handle_collisions();
    calculate_local_gravitational_forces();
    calculate_global_gravitational_forces();
    
    for(int i = 0; i < this -> object_list.size(); ++i)
    {
        this -> object_list[i] -> move(this -> dt);
        this -> object_list[i] -> display();
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
        if(this -> object_list[i] -> get_coord().x + this -> object_list[i] -> get_characteristic_size() >= GetMonitorWidth(0))
        {
            this -> object_list[i] -> set_vel(-abs(this -> object_list[i] -> get_vel().x), 
                                                   this -> object_list[i] -> get_vel().y);
            ++this -> collision_number;
        } 
        else if(this -> object_list[i] -> get_coord().x - this -> object_list[i] -> get_characteristic_size() <= 0)
        {
            this -> object_list[i] -> set_vel(abs(this -> object_list[i] -> get_vel().x), 
                                                  this -> object_list[i] -> get_vel().y);
            ++this -> collision_number;
        }

        if(this -> object_list[i] -> get_coord().y + this -> object_list[i] -> get_characteristic_size() >= GetMonitorHeight(0))
        {
            this -> object_list[i] -> set_vel(this -> object_list[i] -> get_vel().x, 
                                         -abs(this -> object_list[i] -> get_vel().y));
            ++this -> collision_number;
        }
        else if(this -> object_list[i] -> get_coord().y - this -> object_list[i] -> get_characteristic_size() <= 0)
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
            float dist = calculate_distance(this -> object_list[i] -> get_coord(), 
                                            this -> object_list[j] -> get_coord()); 
            
            if(dist <= this -> object_list[i] -> get_characteristic_size() + 
                       this -> object_list[j] -> get_characteristic_size())
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
        std::cerr << "Body has negative mass parameter\n";
        return false;
    }

    if(obj -> get_coord().x < obj -> get_characteristic_size() || 
       obj -> get_coord().y < obj -> get_characteristic_size() || 
       obj -> get_coord().x > GetScreenWidth() - obj-> get_characteristic_size() || 
       obj -> get_coord().x > GetScreenWidth() - obj-> get_characteristic_size())
    {
        std::cerr << "Body No " << this -> object_list.size() << " is initialized in the wall\n";
        return false;
    }

    for(int i = 0; i < this -> object_list.size(); ++i)
    {
        if(calculate_distance(this -> object_list[i] -> get_coord(), obj -> get_coord()) < 
                              this -> object_list[i] -> get_characteristic_size() + obj -> get_characteristic_size())
        {
            std::cerr << "Body No " << this -> object_list.size() << " is initialized inside the body No " << i << '\n';
            return false;
        }
    }

    return true;
}

void System::add_hard_connection(object* obj_1, object* obj_2)
{
    if(this -> body_body_hard_connections.find(obj_1) == this -> body_body_hard_connections.end())
    {
        this -> body_body_hard_connections.insert({obj_1, {}});
    }
    this -> body_body_hard_connections.at(obj_1).insert(obj_2);

    if(this -> body_body_hard_connections.find(obj_2) == this -> body_body_hard_connections.end())
    {
        this -> body_body_hard_connections.insert({obj_2, {}});
    }
    this -> body_body_hard_connections.at(obj_2).insert(obj_1);
}

void System::add_hard_connection(object* obj_1, Vector2 point)
{
    if(this -> body_point_hard_connections.find(obj_1) != this -> body_point_hard_connections.end())
    {
        this -> body_point_hard_connections.insert({obj_1, {}});
    }
    this -> body_point_hard_connections.at(obj_1).push_back(point);
}

void System::add_soft_connection(object* obj_1, object* obj_2)
{
    if(this -> body_body_soft_connections.find(obj_1) == this -> body_body_soft_connections.end())
    {
        this -> body_body_soft_connections.insert({obj_1, {}});
    }
    this -> body_body_soft_connections.at(obj_1).insert(obj_2);

    if(this -> body_body_soft_connections.find(obj_2) == this -> body_body_soft_connections.end())
    {
        this -> body_body_soft_connections.insert({obj_2, {}});
    }
    this -> body_body_soft_connections.at(obj_2).insert(obj_1);
}

void System::add_soft_connection(object* obj_1, Vector2 point)
{
    if(this -> body_point_soft_connections.find(obj_1) == this -> body_point_soft_connections.end())
    {
        this -> body_point_soft_connections.insert({obj_1, {}});
    }
    this -> body_point_soft_connections.at(obj_1).push_back(point);
}

void System::add_connection(unsigned int body_num_1, unsigned int body_num_2, bool hard_connection = true)
{
    if(hard_connection && object_list.size() >= body_num_1 && 
                          object_list.size() >= body_num_2)
    {
        this -> add_hard_connection(this -> object_list[body_num_1 - 1], 
                                    this -> object_list[body_num_2 - 1]);
            
        return;    
    }

    if(!hard_connection && object_list.size() >= body_num_1 && 
                           object_list.size() >= body_num_2)
    {
        this -> add_soft_connection(this -> object_list[body_num_1 - 1], 
                                    this -> object_list[body_num_2 - 1]);
    
        return;
    }

    std::cerr << "Connection is set between at least one non-existant element\n";
}

void System::add_connection(unsigned int body_num, float point_x, float point_y, bool hard_connection = true)
{
    if(hard_connection && object_list.size() >= body_num)
    {
        this -> add_hard_connection(this -> object_list[body_num - 1], Vector2{point_x, point_y});
    }

    if(!hard_connection && object_list.size() >= body_num)
    {
        this -> add_soft_connection(this -> object_list[body_num - 1], Vector2{point_x, point_y});
    }
}