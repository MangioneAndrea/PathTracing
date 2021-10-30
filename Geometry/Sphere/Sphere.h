//
// Created by Andrea Mangione on 27/09/2021.
//
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
    uint32_t *texture = nullptr;
    uint16_t textureWidth;
    uint16_t textureHeight;


    Sphere(glm::dvec3 center, double radius, uint32_t color);
    Sphere(glm::dvec3 center, double radius, uint32_t color, double reflectivity);
    Sphere(glm::dvec3 center, double radius, uint32_t color, Color emission);
    Sphere(glm::dvec3 center, double radius, uint32_t *texture, uint16_t textureWidth, uint16_t textureHeight, bool emissive);
    glm::dvec3 *ClosestIntersection(glm::dvec3 origin, glm::dvec3 direction);

    Color emissionF(int x, int y);

    Color BRDF(int x, int y, Color *specular);
    Color BRDF(int x, int y);
};

#endif// PATHTRACING_SPHERE_H
