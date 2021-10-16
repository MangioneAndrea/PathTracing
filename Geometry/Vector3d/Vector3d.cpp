//
// Created by andre on 27/09/2021.
//

#include "Vector3d.h"
#include <cmath>

#define ZERO_VECTOR Vector3d(0, 0, 0)
Vector3d::Vector3d(double x, double y, double z) : x(x), y(y), z(z) {}


double Vector3d::length() { return std::sqrt(squaredLength()); }

double Vector3d::squaredLength() { return x * x + y * y + z * z; }

double Vector3d::dot(Vector3d *other) {
    return x * other->x + y * other->y + z * other->z;
}

Vector3d Vector3d::normalize() {
    auto l = this->length();
    return l == 0 ? ZERO_VECTOR : this->dividedBy(this->length());
}
Vector3d Vector3d::dividedBy(double div) {
    return {x / div, y / div, z / div};
}

Vector3d Vector3d::minus(Vector3d *in) {
    return {this->x - in->x, this->y - in->y, this->z - in->z};
}
Vector3d Vector3d::plus(Vector3d *in) {
    return {this->x + in->x, this->y + in->y, this->z + in->z};
}
Vector3d Vector3d::crossProduct(Vector3d *b) {
    return {this->y * b->z - this->z * b->y,
            this->z * b->x - this->x * b->z,
            this->x * b->y - this->y * b->x};
}
Vector3d Vector3d::times(double div) {
    return {x * div, y * div, z * div};
}
double Vector3d::distanceTo(Vector3d *in) {
    return in->minus(this).length();
}
