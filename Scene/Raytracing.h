//
// Created by  Andrea Mangione  on 27/09/2021.
//

#include "Scene.h"
#include "glm/ext.hpp"
#include <cmath>
#define iterations 1
#define fRand() ((double) rand() / (RAND_MAX / 2) - 1)
#define p 0.4

class Raytracing : public Scene {
public:
    Raytracing(int width, int height) {
        srand((unsigned int) time(NULL));
        this->width = width;
        this->height = height;
        objects = new std::vector<Sphere *>{
                new Sphere{glm::vec3{-0.6, -0.7, -0.6}, 0.3, 0xDDDD11},
                new Sphere{glm::vec3{0.3, -0.4, 0.3}, 0.6, 0xDDDDDD},
                new Sphere{
                        glm::vec3{0, 0, 101},
                        100,
                        0x11ff11,
                },
                new Sphere{
                        glm::vec3{-101, 0, 0},
                        100,
                        0xff1111,
                },
                new Sphere{
                        glm::vec3{101, 0, 0},
                        100,
                        0x1111dd,
                },
                new Sphere{glm::vec3{0, 101, 0}, 100, 0xffffff, Color(0xffffff)},
                new Sphere{
                        glm::vec3{0, -101, 0},
                        100,
                        0x888811,
                }};
        pixels = (uint32_t *) malloc(sizeof(uint32_t) * width * height);
        aspectRatio = (double) width / (double) height;
        eye = glm::dvec3(0, 0, -14);
        lookAt = glm::dvec3(0, 0, 6);
    }


    void GetPixels() override {
        glm::dvec3 up = glm::vec3(0, 1, 0);

        auto r = glm::normalize(glm::cross(up, lookAt));
        auto u = glm::normalize(glm::cross(lookAt, r));

        double fovScale = std::tan(fov / 2);
        for (auto i = 0; i < width * height; i++) {
            pixels[i] = 0;
            auto x = (float) (i % width);
            auto y = (float) std::floor(i / width);
            // x,y from -1 to 1
            x = (x / ((float) width / 2) - 1) * fovScale * aspectRatio;
            y = (y / ((float) height / 2) - 1) * fovScale;
            auto tmp = r * glm::dvec1(fovScale * x);
            auto tmp2 = u * (-fovScale * y);
            glm::dvec3 d = glm::normalize(lookAt) + tmp + tmp2;//  lookAt.no.plus(&tmp).plus(&tmp2);

            if (i == 240050) {
                i = i;
            }
            pixels[i] = ComputeColor(eye, d).ToInt();
        }
    }


    glm::dvec3 *ClosestVectorFrom(glm::dvec3 o, glm::dvec3 d, Sphere *&closestObj) {
        glm::dvec3 *closest = nullptr;
        for (auto &elem : *objects) {
            glm::dvec3 *intersection = elem->ClosestIntersection(o, d);
            if (intersection != nullptr) {
                double d1 = glm::distance(o, *intersection);
                if (d1 == 0) {
                    delete intersection;
                    continue;
                }
                if (closest != nullptr) {
                    double d2 = glm::distance(o, *closest);
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


    Color ComputeColor(glm::dvec3 origin, glm::dvec3 direction) {

        // auto r = Ray(origin, direction);

        Sphere *closest = nullptr;
        glm::dvec3 *hp_ = ClosestVectorFrom(origin, direction, closest);

        if (closest == nullptr) {
            return BLACK;
        }
        return closest->color;
    }
};