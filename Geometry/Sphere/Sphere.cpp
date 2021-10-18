//
// Created by Andrea Mangione on 27/09/2021.
//

#include "Sphere.h"
#include <SDL2/SDL_main.h>
#include <cmath>
#include <glm/ext.hpp>

Sphere::Sphere(glm::dvec3 center, double radius, uint32_t color, double reflectivity)
    : center(center), radius(radius), color(color), emission(0),
      BRDF(this->color * (1. / M_PI)) {
    this->reflectivity = reflectivity;
}

Sphere::Sphere(glm::dvec3 center, double radius, uint32_t color)
    : center(center), radius(radius), color(color), emission(0),
      BRDF(this->color * (1. / M_PI)) {}

Sphere::Sphere(glm::dvec3 center, double radius, uint32_t color,
               Color emission)
    : center(center), radius(radius), color(color),
      BRDF(this->color * (1. / M_PI)), emission(emission) {}


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
glm::dvec3 Sphere::normalVectorAt(glm::dvec3 vector) {
    return glm::normalize(vector - this->center);
}
