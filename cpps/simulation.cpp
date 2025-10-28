#include "raylib.h"
#include "Sphere.h"
#include "System.h"
#include "Rect.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <cctype>

void init_system_from_config_file(std::string path = CONFIG_FILE_PATH)
{
    System* system = System::get_singleton();
    std::fstream conf_file(path);
    
    Vector2 acceleration{0, 0};
    char display_bar = 1;

    bool sphere_created = false;
    bool rect_created = false;
    bool connection_created = false;
    
    float mass = 1;
    float initial_x  = 0;
    float initial_y  = 0;
    float initial_vx = 0;
    float initial_vy = 0;
    float radius = 10;
    float width = 10;
    float height = 10;
    
    unsigned int connection_body_num = -1;
    float connection_point_x = 0;
    float connection_point_y = 0;

    uint line_counter = 0;
    std::string line;
    while(getline(conf_file, line))
    {
        ++line_counter;
        if(line.empty() ||
           line.find(';') != std::string::npos)
        {
            continue;
        }

        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

        if(line == "SPHERE" || 
           line == "CONNECTION" || 
           line == "RECTANGLE")
        {
            if(sphere_created)
            {
                system -> add_body(new sphere(initial_x, initial_y, 
                                              initial_vx, initial_vy, 
                                              mass, radius));
                float mass = 1;
                float initial_x  = 0;
                float initial_y  = 0;
                float initial_vx = 0;
                float initial_vy = 0;
                float radius = 10;
                float width = 10;
                float height = 10;
    
                unsigned int connection_body_num = -1;
                float connection_point_x = 0;
                float connection_point_y = 0;
            } 

            else if(connection_created)
            {
                system -> add_connection(connection_body_num, 
                                         connection_point_x, 
                                         connection_point_y);
                 
                float mass = 1;
                float initial_x  = 0;
                float initial_y  = 0;
                float initial_vx = 0;
                float initial_vy = 0;
                float radius = 10;
                float width = 10;
                float height = 10;
    
                unsigned int connection_body_num = -1;
                float connection_point_x = 0;
                float connection_point_y = 0;    
            }

            else if(rect_created)
            {
                system -> add_body(new Rect(initial_x, initial_y, 
                                            initial_vx, initial_vy, 
                                            mass, width, height));
                float mass = 1;
                float initial_x  = 0;
                float initial_y  = 0;
                float initial_vx = 0;
                float initial_vy = 0;
                float radius = 10;
                float width = 10;
                float height = 10;
    
                unsigned int connection_body_num = -1;
                float connection_point_x = 0;
                float connection_point_y = 0;
            }

            sphere_created = (line == "SPHERE");
            rect_created = (line == "RECTANGLE"); 
            connection_created = (line == "CONNECTION");
            continue;
        }

        int sign_pos = line.find('=');
        if(sign_pos == std::string::npos)
        {
            std::cerr << "SMTH WENT WRONG WITH LINE  " << line_counter<<'\n'; 
        }
        std::string variable = line.substr(0, sign_pos);
        if(variable.empty())
        {
            std::cerr << "SMTH WENT WRONG WITH LINE  " << line_counter <<'\n';
        }
        
        if(variable == "GLOBAL_ACCELERATION_X")
        {
            acceleration.x = std::stof(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "GLOBAL_ACCELERATION_Y")
        {
            acceleration.y = std::stof(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "DISPLAY_BAR")
        {
            display_bar = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "MASS")
        {
            mass = std::stof(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "INITIAL_X")
        {
            initial_x = std::stof(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "INITIAL_Y")
        {
            initial_y = std::stof(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "INITIAL_VX")
        {
            initial_vx = std::stof(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "INITIAL_VY")
        {
            initial_vy = std::stof(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "RADIUS")
        {
            radius = std::stof(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "WIDTH")
        {
            width = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "HEIGHT")
        {
            height = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "OBJECT_NUM")
        {
            connection_body_num = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "CONNECTION_POINT_X")
        {
            connection_point_x = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "CONNECTION_POINT_Y")
        {
            connection_point_y = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else
        {
            std::cout << "CANT INTERPRET THE VARIABLE NAME ON LINE " << line_counter << '\n';
            std::cout << "UNINTERPRETED VARIABLE NAME: " << variable << '\n';
        }
    }

    if(sphere_created)
    {
        system -> add_body(new sphere(initial_x, initial_y, 
                                      initial_vx, initial_vy, 
                                      mass, radius));
    }
    else if(rect_created)
    {
        system -> add_body(new Rect(initial_x, initial_y, 
                                    initial_vx, initial_vy, 
                                    mass, width, height));
    }
    else if(connection_created)
    {
        system -> add_connection(connection_body_num, 
                                 connection_point_x, 
                                 connection_point_y);  
    }

    system -> set_global_acceleration(acceleration);
    system -> set_gradient_system(display_bar);
}


int main() {
    InitWindow(1280, 720, "Shapes");
    ToggleFullscreen();
    
    System* system = System::get_singleton();
    init_system_from_config_file();

    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        DrawText(std::to_string(system -> get_collision_number()).c_str(), 50, 50, 100, RED);

        system -> update_system();

        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}
