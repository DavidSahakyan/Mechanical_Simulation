#include "raylib.h"
#include "Sphere.h"
#include "System.h"

#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include <chrono>
#include <thread>

int main() {
    InitWindow(1280, 720, "Shapes");
    ToggleFullscreen();
    System* system = System::get_singleton();
        
    sphere* s1 = new sphere({600, 500},  {0, -20}, {0, 0}, 10000,  BLUE, 10);
    sphere* s2 = new sphere({600, 250},  {0, 0},   {0, 0}, 1,  BLUE, 10);
    // sphere* s3 = new sphere({900, 510}, {0, 0}, {0, 0}, 1,  BLUE, 10);
    // sphere* s4 = new sphere({750, 740}, {0, 0},  {0, 0}, 1,  BLUE, 10);
    
    system -> add_body(s1);
    system -> add_body(s2);
    // system -> add_body(s3);
    // system -> add_body(s4);

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
