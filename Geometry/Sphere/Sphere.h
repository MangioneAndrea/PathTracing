//
// Created by Andrea Mangione on 27/09/2021.
//
#include "../Ray/Ray.h"
#include "Geometry/Vector3d/Vector3d.h"
#include <Color/Color.h>
#include <cstdint>

#ifndef PATHTRACING_SPHERE_H
#define PATHTRACING_SPHERE_H

class Sphere {
private:
public:
  Vector3d *center;
  double radius;
  Color color;
  Color emission;
  Color BRDF;

  bool Intersects(Ray *r);

  Vector3d normalVectorAt(Vector3d *vector);

  Sphere(Vector3d *center, double radius, uint32_t color);
  Sphere(Vector3d *center, double radius, uint32_t color, Color emission);
  Vector3d *ClosestIntersection(Ray *r);
};

#endif // PATHTRACING_SPHERE_H
