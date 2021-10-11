//
// Created by andre on 27/09/2021.
//
#include "Geometry/Vector3d/Vector3d.h"

#ifndef PATHTRACING_RAY_H
#define PATHTRACING_RAY_H

struct Ray {
  Vector3d *origin;
  Vector3d *direction;
  Ray(Vector3d *origin, Vector3d *direction);
};

#endif // PATHTRACING_RAY_H
