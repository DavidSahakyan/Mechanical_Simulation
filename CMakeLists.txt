cmake_minimum_required(VERSION 4.1.2)
project(simulator)

set(CMAKE_CXX_STANDARD 17)


add_executable(simulator cpps/simulation.cpp 
                         cpps/Sphere.cpp 
                         cpps/Object.cpp  
                         cpps/System.cpp)

target_include_directories(simulator PUBLIC headers)
target_link_libraries(simulator raylib)


