#ifndef SYSTEM 
#define SYSTEM

#include "Object.h"
#include "Connection.h"

#include <vector>
#include <cstdint>

class System{
public:
    static System* get_singleton();
    
    void add_body(object* object);
    void add_body(object& object);
    void add_bodies(const std::vector<object*>& object_list);
    
    void add_connection(unsigned int body_num, float point_x, float point_y);     
    
    void set_global_acceleration(const Vector2& acceleration);
    void set_global_acceleration(const float& acceleration_x, const float& acceleration_y);
    
    void set_gradient_system(char);
    
    void update_system();    
    uint64_t get_collision_number();
    
    private:
    
    void add_connection(object* obj_1, Vector2 point);
    
    void calculate_forces();
    void calculate_local_gravitational_forces();
    void calculate_local_electromagnetic_forces();
    void calculate_global_gravitational_forces();
    void calculate_global_electromagnetic_field_forces();
    
    void handle_collisions();    
    void handle_connections();
    
    bool check_validity(object* obj);
    
    float calculate_distance(const Vector2& point1, const Vector2& point2);
    float calculate_cos_with_horizon(const Vector2& point1, const Vector2& point2);
    float calculate_sin_with_horizon(const Vector2& point1, const Vector2& point2);

    System();
    System(const System&) = delete;
    ~System();
    
    std::vector<object*> object_list = {};
    std::vector<Connection*> connection_list = {};
    
    char display_bar = 1;
    void draw_display_bar();
    void set_gradient();
    float min_vel_sqr = 0.0;
    float max_vel_sqr = 0.0;
    float min_force_sqr = 0.0;
    float max_force_sqr = 0.0;

    Vector2 global_acceleration = {0, 0};
    const float dt = 0.001;
    uint64_t collision_number = 0;
};

#endif