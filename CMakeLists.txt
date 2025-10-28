cmake_minimum_required(VERSION 3.14)
project(simulator)

set(CMAKE_CXX_STANDARD 17)

add_executable(simulator cpps/simulation.cpp 
                         cpps/Sphere.cpp 
                         cpps/Object.cpp  
                         cpps/System.cpp
                         cpps/Connection.cpp
                         cpps/Rect.cpp)


target_compile_definitions(simulator PUBLIC CONFIG_FILE_PATH=\"${CMAKE_SOURCE_DIR}/config_file.ini\")
target_include_directories(simulator PUBLIC headers)
target_link_libraries(simulator raylib)





