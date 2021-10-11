//
// Created by  Andrea Mangione  on 27/09/2021.
//

#include "Geometry/Ray/Ray.h"
#include "Scene.h"
#include <cmath>
#define iterations 1
#define fRand() ((double) rand() / (RAND_MAX / 2) - 1)
#define p 0.4

class Raytracing : public Scene {
public:
    Raytracing() {
        width = 400;
        height = 400;
        objects = new std::vector<Sphere *>{
                new Sphere{new Vector3d{-0.6, -0.7, -0.6}, 0.3, 0xDDDD00},
                new Sphere{new Vector3d{0.3, -0.4, 0.3}, 0.6, 0x00DDDD},
                new Sphere{
                        new Vector3d{0, 0, 1001},
                        1000,
                        0x888888,
                },
                new Sphere{
                        new Vector3d{-1001, 0, 0},
                        1000,
                        0xff0000,
                },
                new Sphere{
                        new Vector3d{1001, 0, 0},
                        1000,
                        0x0000ff,
                },
                new Sphere{new Vector3d{0, 1001, 0}, 1000, 0xffffff, 0xffffff},
                new Sphere{
                        new Vector3d{0, -1001, 0},
                        1000,
                        0x888888,
                }};
        pixels = (uint32_t *) malloc(sizeof(uint32_t) * width * height);
        aspectRatio = (double) width / (double) height;
        eye = new Vector3d(0, 0, -14);
        lookAt = new Vector3d(0, 0, 6);
    }


    void GetPixels() override {
        Ray *camera = new Ray(eye, lookAt);
        Vector3d up = Vector3d(0, 1, 0);
        Vector3d r = up.crossProduct(camera->direction).normalize();
        Vector3d u = camera->direction->crossProduct(&r).normalize();

        double fovScale = std::tan(fov / 2);
        Ray *ray = new Ray(camera->origin, nullptr);
        for (auto i = 0; i < width * height; i++) {
            pixels[i] = 0;
            auto x = (double) (i % width);
            auto y = (double) std::floor(i / width);
            // x,y from -1 to 1
            x = (x / ((double) width / 2) - 1) * fovScale * aspectRatio;
            y = (y / ((double) height / 2) - 1) * fovScale;

            auto tmp = r.times(fovScale * x);
            auto tmp2 = u.times(-fovScale * y);
            Vector3d d = camera->direction->normalize().plus(&tmp).plus(&tmp2);
            if (i == 240050) {
                i = i;
            }
            pixels[i] = ComputeColor(eye, &d).ToInt();
        }
        free(camera);
        free(ray);
    }

    Vector3d *ClosestVectorFrom(Ray *ray, Sphere *&closestObj) {
        Vector3d *closest = nullptr;
        for (auto &elem : *objects) {
            Vector3d *intersection = elem->ClosestIntersection(ray);
            if (intersection != nullptr) {
                double d1 = ray->origin->distanceTo(intersection);
                if (d1 == 0) {
                    delete intersection;
                    continue;
                }
                if (closest != nullptr) {
                    double d2 = ray->origin->distanceTo(closest);
                    if (d1 < d2) {
                        delete closest;
                        closest = intersection;
                        closestObj = elem;
                    } else {
                        delete intersection;
                    }
                } else {
                    closest = intersection;
                    closestObj = elem;
                }
            }
        }
        return closest;
    }

    Color ComputeColor(Vector3d *o, Vector3d *d) {
        auto r = Ray(o, d);
        Sphere *closest = nullptr;
        Vector3d *hp = ClosestVectorFrom(&r, closest);
        delete hp;
        if (closest == nullptr) {
            return BLACK;
        }
        auto res = closest->color;
        return res;
    }
};