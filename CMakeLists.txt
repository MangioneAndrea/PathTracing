cmake_minimum_required(VERSION 3.19)
project(PathTracing)
set(CMAKE_CXX_STANDARD 20)
file(COPY ./lib/SDL2/SDL2.dll DESTINATION .)

include_directories(./inc)
include_directories(.)
link_directories(./lib/SDL2)

add_executable(PathTracing main.cpp Scene/Raytracing2.h ./inc/stb/stb_image.h ./Scene/Scene.h ./Scene/ColorShowdown.h ./Scene/Texture.h ./Scene/Raytracing2.h ./Geometry/Sphere/Sphere.cpp ./Geometry/Sphere/Sphere.h  ./Color/Color.h)
target_link_libraries(PathTracing SDL2main SDL2)