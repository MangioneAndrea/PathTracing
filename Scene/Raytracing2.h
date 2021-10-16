//
// Created by  Andrea Mangione  on 27/09/2021.
//

#include "Geometry/Ray/Ray.h"
#include "Scene.h"
#include <cmath>

#define iterations 128

class Raytracing2 : public Scene {
public:
    Raytracing2(int width, int height) {
        this->width = width;
        this->height = height;
        objects = new std::vector<Sphere *>{
                new Sphere{new Vector3d{-0.6, -0.7, -0.6}, 0.3, 0xDDDD11},
                new Sphere{new Vector3d{0.3, -0.4, 0.3}, 0.6, 0xDDDDDD},
                new Sphere{
                        new Vector3d{0, 0, 1001},
                        1000,
                        0x888888,
                },
                new Sphere{
                        new Vector3d{-1001, 0, 0},
                        1000,
                        0xff1111,
                },
                new Sphere{
                        new Vector3d{1001, 0, 0},
                        1000,
                        0x1111dd,
                },
                new Sphere{new Vector3d{0, 1001, 0}, 1000, 0xffffff, Color(0xffffff)},
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

        auto colors = (Color *) malloc(iterations * sizeof(Color));
        double fovScale = std::tan(fov / 2);
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
            pixels[i] = 0;
            for (int it = 0; it <= iterations; it++) {
                if (i % (width * 40) == 0) {
                    it = it;
                }
                colors[it] = ComputeColor(eye, &d);
            }
            pixels[i] = Color::avg(iterations, colors).Clamp().GammaCorrect().ToInt();
        }
        delete camera;
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

#define fRand() ((double) rand() / (RAND_MAX / 2) - 1)
#define p 0.2
    Color ComputeColor(Vector3d *o, Vector3d *d) {

        auto r = Ray(o, d);

        Sphere *closest = nullptr;
        Vector3d *hp = ClosestVectorFrom(&r, closest);


        if (closest == nullptr) {
            delete hp;
            return BLACK;
        }

        auto rnd = rand();
        if ((rnd % 1000) <= p * 1000) {
            delete hp;
            return closest->emission;
        }

        Vector3d randomDir = Vector3d(0, 0, 0);
        Vector3d n = closest->normalVectorAt(hp);
        do {
            randomDir.x = fRand();
            randomDir.y = fRand();
            randomDir.z = fRand();
        } while (randomDir.squaredLength() > 1);
        randomDir = randomDir.normalize();

        if (randomDir.dot(&n) < 0) {
            randomDir = randomDir.times(-1);
        }

        auto nextEmission = ComputeColor(hp, &randomDir);
        delete hp;
        auto ownColor = closest->BRDF * (n.dot(&randomDir) * ((2 * PI) / (1 - p)));
        Color other = nextEmission * ownColor;
        auto res = closest->emission + other;
        if (res.r != 0) {
            res = res;
        }
        return res;
    }
};