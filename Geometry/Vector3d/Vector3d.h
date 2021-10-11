//
// Created by andre on 27/09/2021.
//

#ifndef EMPTY_VERTEX3D_H
#define EMPTY_VERTEX3D_H

#include <iostream>
struct Vector3d {
    double x, y, z;

    Vector3d(double x, double y, double z);

    double length();
    double squaredLength();
    Vector3d normalize();
    double dot(Vector3d *other);
    Vector3d dividedBy(double div);
    Vector3d times(double div);

    Vector3d minus(Vector3d *in);
    Vector3d plus(Vector3d *in);
    Vector3d crossProduct(Vector3d *pD);

    double distanceTo(Vector3d *in);

    void print() {
        std::cout << x << " - " << y << " - " << z << std::endl;
    }
};

#endif// EMPTY_VERTEX3D_H
