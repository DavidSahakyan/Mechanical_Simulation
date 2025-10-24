#ifndef SYSTEM 
#define SYSTEM
#include "Sphere.h"
#include <vector>
#include <cstdint>

class System{
public:
    static System* get_singleton();
    void add_body(object* object);
    void add_body(object& object);
    void add_bodies(const std::vector<object*>& object_list);
    void set_global_acceleration(const Vector2& acceleration);
    void set_global_acceleration(const float& acceleration_x, const float& acceleration_y);

    void update_system();    
    uint64_t get_collision_number();
    
private:
    void calculate_forces();
    void calculate_local_gravitational_forces();
    void calculate_local_electromagnetic_forces();
    void calculate_global_gravitational_forces();
    void calculate_potential_electromagnetic_field_forces();
    void handle_collisions();    
    bool check_validity(object* obj);
    float calculate_distance(const Vector2& point1, const Vector2& point2);
    float calculate_cos_with_horizon(const Vector2& point1, const Vector2& point2);
    float calculate_sin_with_horizon(const Vector2& point1, const Vector2& point2);
    System();
    System(const System&) = delete;
    ~System();
    std::vector<object*> object_list = {};
    Vector2 global_acceleration = {0, 0};
    const float dt = 0.001;
    uint64_t collision_number = 0;
};

#endif