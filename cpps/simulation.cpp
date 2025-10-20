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
    float mass = 1;
    float initial_x  = 0;
    float initial_y  = 0;
    float initial_vx = 0;
    float initial_vy = 0;
    float characteristic_size = 10;
    Color c = BLUE;        

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

        if(line == "NEWBODY" && body_created)
        {
            system -> add_body(new sphere(initial_x, initial_y, 
                                          initial_vx, initial_vy, 
                                          mass, c, characteristic_size));
            float mass = 1;
            float initial_x  = 0;
            float initial_y  = 0;
            float initial_vx = 0;
            float initial_vy = 0;
            float characteristic_size = 10;
            Color c = BLUE;    
            continue; 
        }
        else if(line == "NEWBODY" && !body_created)
        {
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
            body_created = true;
        }
        else if(variable == "INITIAL_X")
        {
            initial_x = std::stof(line.substr(sign_pos + 1, line.size()));
            body_created = true;
        }
        else if(variable == "INITIAL_Y")
        {
            initial_y = std::stof(line.substr(sign_pos + 1, line.size()));
            body_created = true;
        }
        else if(variable == "INITIAL_VX")
        {
            initial_vx = std::stof(line.substr(sign_pos + 1, line.size()));
            body_created = true;
        }
        else if(variable == "INITIAL_VY")
        {
            initial_vy = std::stof(line.substr(sign_pos + 1, line.size()));
            body_created = true;
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
