#include "raylib.h"
#include "Sphere.h"
#include "System.h"

#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include <chrono>
#include <thread>
#include <fstream>
#include <algorithm>
#include <cctype>

void init_system_from_config_file(std::string path = CONFIG_FILE_PATH)
{
    System* system = System::get_singleton();
    std::fstream conf_file(path);
    
    Vector2 acceleration{0, 0};

    bool body_created = false;
    bool body_point_connection_created = false;
    bool body_body_connection_created = false;
    
    float mass = 1;
    float initial_x  = 0;
    float initial_y  = 0;
    float initial_vx = 0;
    float initial_vy = 0;
    float characteristic_size = 10;
    Color c = BLUE;        
    
    float connection_max_len = 0; // 0 MEAN HARD CONNECTION, -1 MEAN DISTANCE AT THE INNITIALIZATION MOMENT   
    unsigned int connection_body_num_1 = -1;
    unsigned int connection_body_num_2 = -1;
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

        if(line == "BODY" || line == "BODYCONNECTION" || line == "POINTCONNECTION")
        {
            if(body_created)
            {
                system -> add_body(new sphere(initial_x, initial_y, 
                                              initial_vx, initial_vy, 
                                              mass, c, characteristic_size));
                mass = 1;
                initial_x  = 0;
                initial_y  = 0;
                initial_vx = 0;
                initial_vy = 0;
                characteristic_size = 10;
                c = BLUE;    
            } 

            if(body_body_connection_created)
            {
                system -> add_connection(connection_body_num_1, 
                                         connection_body_num_2, 
                                         connection_max_len);
                
                connection_max_len = 0;    
                connection_body_num_1 = -1;
                connection_body_num_2 = -1;
            }

            if(body_point_connection_created)
            {
                system -> add_connection(connection_body_num_1, 
                                         connection_point_x, 
                                         connection_point_y, 
                                         connection_max_len);
                
                connection_max_len = 0;    
                connection_body_num_1 = -1;
                connection_point_x = 0;    
                connection_point_y = 0;    
            }

            body_created = (line == "BODY");
            body_body_connection_created = (line == "BODYCONNECTION");
            body_point_connection_created = (line == "POINTCONNECTION");
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
        else if(variable == "CHARACTERISTIC_SIZE")
        {
            characteristic_size = std::stof(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "COLOR_R")
        {
            c.r = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "COLOR_G")
        {
            c.g = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "COLOR_B")
        {
            c.b = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "COLOR_A")
        {
            c.a = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "OBJECT_NUM_1")
        {
            connection_body_num_1 = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "OBJECT_NUM_2")
        {
            connection_body_num_2 = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "CONNECTION_POINT_X")
        {
            connection_point_x = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "CONNECTION_POINT_Y")
        {
            connection_point_y = std::stoi(line.substr(sign_pos + 1, line.size()));
        }
        else if(variable == "MAX_LEN")
        {
            connection_max_len = std::stof(line.substr(sign_pos + 1, line.size()));
        }
        else
        {
            std::cout << "CANT INTERPRET THE VARIABLE NAME ON LINE " << line_counter << '\n';
            std::cout << "UNINTERPRETED VARIABLE NAME: " << variable << '\n';
        }
    }

    if(body_created)
    {
        system -> add_body(new sphere(initial_x, initial_y, 
                                      initial_vx, initial_vy, 
                                      mass, c, characteristic_size));
    }

    if(body_body_connection_created)
    {
        system -> add_connection(connection_body_num_1, 
                                 connection_body_num_2, 
                                 connection_max_len);
    }

    if(body_point_connection_created)
    {
        system -> add_connection(connection_body_num_1, 
                                 connection_point_x, 
                                 connection_point_y, 
                                 connection_max_len);  
    }
    
    system -> set_global_acceleration(acceleration);
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
