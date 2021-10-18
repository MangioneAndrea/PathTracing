//
// Created by Andrea Mangione on 27/09/2021.
//
#include "../Ray/Ray.h"
#include "glm/vec3.hpp"
#include <Color/Color.h>
#include <cstdint>

#ifndef PATHTRACING_SPHERE_H
#define PATHTRACING_SPHERE_H

class Sphere {
private:
public:
    glm::dvec3 center;
    double radius;
    Color color;
    Color emission;
    double reflectivity = 0;
    Color BRDF;


    glm::dvec3 normalVectorAt(glm::dvec3 vector);

    Sphere(glm::dvec3 center, double radius, uint32_t color);
    Sphere(glm::dvec3 center, double radius, uint32_t color, double reflectivity);
    Sphere(glm::dvec3 center, double radius, uint32_t color, Color emission);
    glm::dvec3 *ClosestIntersection(glm::dvec3 origin, glm::dvec3 direction);
};

#endif// PATHTRACING_SPHERE_H
