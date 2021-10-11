//
// Created by Andrea Mangione  on 27/09/2021.
//

#include "./Geometry/Sphere/Sphere.h"
#include <vector>

#ifndef PATHTRACING_SCENE_H
#define PATHTRACING_SCENE_H

#ifndef PI
#define PI 3.14
#endif


class Scene {
public:
    Vector3d *eye;
    Vector3d *lookAt;
    double fov = 0.628319;// 36 deg
    uint32_t *pixels;


    virtual void GetPixels() = 0;

protected:
    int width;
    int height;
    double aspectRatio;

    std::vector<Sphere *> *objects;
};

#endif// PATHTRACING_SCENE_H
