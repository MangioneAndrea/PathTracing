cmake_minimum_required(VERSION 3.19)
project(PathTracing)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_THREAD_LIBS_INIT "-lpthread")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
# file(COPY ./lib/SDL2/SDL2.dll DESTINATION .)

include_directories(./inc)
include_directories(.)
link_directories(./lib/SDL2)
set(CMAKE_BUILD_TYPE Release)

add_executable(PathTracing main.cpp Scene/Raytracing2.h ./inc/stb/stb_image.h ./Scene/Scene.h ./Scene/ColorShowdown.h ./Scene/Texture.h ./Scene/Raytracing2.h ./Geometry/Sphere/Sphere.cpp ./Geometry/Sphere/Sphere.h  ./Color/Color.h)

target_link_libraries(PathTracing SDL2main SDL2)