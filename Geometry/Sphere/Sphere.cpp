//
// Created by Andrea Mangione on 27/09/2021.
//

#include "Sphere.h"
#include <SDL2/SDL_main.h>
#include <cmath>

Sphere::Sphere(Vector3d *center, double radius, uint32_t color)
    : center(center), radius(radius), color(color), emission(0),
      BRDF(this->color * (1. / M_PI)) {}

Sphere::Sphere(Vector3d *center, double radius, uint32_t color,
               uint32_t emission)
    : center(center), radius(radius), color(color),
      BRDF(this->color * (1. / M_PI)), emission(emission) {}

bool Sphere::Intersects(Ray *r) {// geometric solution
    // 1 * delta2 + 2CE*d * delta + |CE|2 -r2=0
    // intersects= sqrt ( (2CE*d)2 - 4 * 1 * (|CE|-r2)2)
    Vector3d CE = r->origin->minus(this->center);
    double a = r->direction->dot(r->direction);
    double b = 2 * CE.dot(r->direction);
    double c = CE.dot(&CE) - radius * radius;
    return (b * b - 4 * a * c) >= 0;
}

Vector3d *Sphere::ClosestIntersection(Ray *r) {// geometric solution
    // 1 * delta2 + 2CE*d * delta + |CE|2 -r2=0
    // intersects= sqrt ( (2CE*d)2 - 4 * 1 * (|CE|-r2)2)
    if (r->origin == this->center)
        return nullptr;

    Vector3d CE = r->origin->minus(this->center);
    double a = r->direction->dot(r->direction);
    double b = 2 * CE.dot(r->direction);
    double c = CE.dot(&CE) - radius * radius;

    double d = (b * b - 4 * a * c);

    if (d < 0)
        return nullptr;

    double delta =
            b > 0 ? (-b + std::sqrt(d)) / (2) : (-b - std::sqrt(d)) / (2);

    auto best = std::min(delta / a, c / delta);

    Vector3d res = r->direction->times(best).plus(r->origin);

    return new Vector3d(res);
}
Vector3d Sphere::normalVectorAt(Vector3d *vector) {
    return vector->minus(this->center).normalize();
}