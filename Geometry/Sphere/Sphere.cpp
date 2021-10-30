//
// Created by Andrea Mangione on 27/09/2021.
//

#include "Sphere.h"
#include <SDL2/SDL_main.h>
#include <cmath>
#include <glm/ext.hpp>

Sphere::Sphere(glm::dvec3 center, double radius, uint32_t color, double reflectivity)
    : center(center), radius(radius), color(color), emission(0) {
    this->reflectivity = reflectivity;
}

Sphere::Sphere(glm::dvec3 center, double radius, uint32_t color)
    : center(center), radius(radius), color(color), emission(0) {}

Sphere::Sphere(glm::dvec3 center, double radius, uint32_t color,
               Color emission)
    : center(center), radius(radius), color(color),
      emission(emission) {}

Sphere::Sphere(glm::dvec3 center, double radius, uint32_t *texture, uint16_t textureWidth, uint16_t textureHeight, bool emissive)
    : center(center), radius(radius), texture(texture), textureWidth(textureWidth), textureHeight(textureHeight), emission(emissive ? 0xffffff : 0x000000) {
}


glm::dvec3 *Sphere::ClosestIntersection(glm::dvec3 origin, glm::dvec3 direction) {// geometric solution
    // 1 * delta2 + 2CE*d * delta + |CE|2 -r2=0
    // intersects= sqrt ( (2CE*d)2 - 4 * 1 * (|CE|-r2)2)
    auto u = glm::normalize(direction);
    glm::dvec3 CE = origin - (this->center);

    auto a = 1;
    auto b = 2 * glm::dot(u, CE);
    auto c = glm::dot(CE, CE) - radius * radius;

    double delta = (b * b - 4 * a * c);

    if (delta < 0.0001)
        return nullptr;

    double d = b > 0 ? (-b + std::sqrt(delta)) : (-b - std::sqrt(delta));


    if (d < 0.000001)
        return nullptr;

    auto best = d / (2 * a);
    glm::dvec3 res = u * glm::dvec1(best) + (origin);
    return new glm::dvec3{res.x, res.y, res.z};
}
Color Sphere::BRDF(int x, int y, Color *specular) {
    if (specular != nullptr) {
       return (this->color * (1 - reflectivity) + ((*specular) * reflectivity)) * (1. / M_PI);
    }

    return this->color * (1. / M_PI);
}
Color Sphere::BRDF(int x, int y) {

    if (texture == nullptr) {
        return this->color * (1. / M_PI);
    }
    auto b = x % textureWidth;
    auto a = (y % textureHeight) * textureWidth;
    auto res = Color(texture[a + b]);
    return res;
}
Color Sphere::emissionF(int x, int y) {
    if (texture == nullptr || emission.ToInt() == 0) {
        return emission;
    }
    auto b = x % textureWidth;
    auto a = (y % textureHeight) * textureWidth;
    auto res = Color(texture[a + b]);
    return res * 4;
}
