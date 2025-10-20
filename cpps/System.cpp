#include <math.h>
#include <iostream>

#include "System.h"

static System* s = nullptr;

System::System()
{}

void System::add_body(object* obj)
{   
    this -> object_list.push_back(obj);
}

void System::add_body(object& obj)
{
    this -> add_body(&obj);
}

void System::add_bodies(const std::vector<object*>& obj_list)
{
    for(auto i: obj_list)
    {
        this -> add_body(i);
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
        for(int j = i + 1; j < this -> object_list.size(); ++j)
        {
            if(calculate_distance(this -> object_list[i] -> get_coord(), this -> object_list[j] -> get_coord()) <=
                                  this -> object_list[i] -> get_characteristic_size() + this -> object_list[j] -> get_characteristic_size())
            {             
                float vx_i = ((this -> object_list[i] -> get_mass() - this -> object_list[j] -> get_mass()) / 
                              (this -> object_list[i] -> get_mass() + this -> object_list[j] -> get_mass())) * 
                               this -> object_list[i] -> get_vel().x +
                              (this -> object_list[j] -> get_mass() * 2 / 
                              (this -> object_list[i] -> get_mass() + 
                               this -> object_list[j] -> get_mass())) * 
                               this -> object_list[j] -> get_vel().x;

                float vx_j = ((this -> object_list[j] -> get_mass() - this -> object_list[i] -> get_mass()) / 
                              (this -> object_list[j] -> get_mass() + this -> object_list[i] -> get_mass())) * 
                               this -> object_list[j] -> get_vel().x +
                              (this -> object_list[i] -> get_mass() * 2 / 
                              (this -> object_list[j] -> get_mass() + 
                               this -> object_list[i] -> get_mass())) * 
                               this -> object_list[i] -> get_vel().x;

                float vy_i = ((this -> object_list[i] -> get_mass() - this -> object_list[j] -> get_mass()) / 
                              (this -> object_list[i] -> get_mass() + this -> object_list[j] -> get_mass())) * 
                               this -> object_list[i] -> get_vel().y +
                              (this -> object_list[j] -> get_mass() * 2 / 
                              (this -> object_list[i] -> get_mass() + 
                               this -> object_list[j] -> get_mass())) * 
                               this -> object_list[j] -> get_vel().y;


                float vy_j = ((this -> object_list[j] -> get_mass() - this -> object_list[i] -> get_mass()) / 
                              (this -> object_list[j] -> get_mass() + this -> object_list[i] -> get_mass())) * 
                               this -> object_list[j] -> get_vel().y +
                              (this -> object_list[i] -> get_mass() * 2 / 
                              (this -> object_list[j] -> get_mass() + 
                               this -> object_list[i] -> get_mass())) * 
                               this -> object_list[i] -> get_vel().y;

                this -> object_list[i] -> set_vel(vx_i, vy_i);
                this -> object_list[j] -> set_vel(vx_j, vy_j);
                ++this -> collision_number;
            }
        }        
    }
}

