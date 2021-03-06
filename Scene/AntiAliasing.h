//
// Created by  Andrea Mangione  on 27/09/2021.
//

#include "Scene.h"
#include "glm/ext.hpp"
#include <chrono>
#include <cmath>

#define iterations 15
#define aa_iterations 10
#define aa_sigma 0.5f
#define fRand() (((float) ((int) rand())) / (RAND_MAX / 2) - 1)

class AntiAliasing : public Scene {
public:
    Color colors[iterations * aa_iterations];
    AntiAliasing(int width, int height) {
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
        eye = glm::dvec3(0, 0, -4);
        lookAt = glm::dvec3(0, 0, 6);
    }


    void GetPixels() override {
        glm::dvec3 up = glm::vec3(0, 1, 0);

        auto direction = glm::normalize(lookAt - eye);
        auto r = glm::normalize(glm::cross(up, direction));
        auto u = glm::normalize(glm::cross(direction, r));

        double fovScale = std::tan(fov / 2);
        for (auto i = 0; i < (width / 2) * (height / 2); i++) {
            auto i0 = i * 2 + (i / (width / 2)) * width;
            auto i1 = i0 + 1;
            auto i2 = i0 + width;
            auto i3 = i1 + width;

            auto x = (float) (i0 % width);
            auto y = (float) std::floor(i0 / width);

            for (int aa = 0; aa < iterations; aa++) {

                float xShifted = glm::gaussRand(x, aa_sigma);
                float yShifted = glm::gaussRand(y, aa_sigma);
                //pixel = (vertical + (i / width)) * width + ((horizontal + i) % width);
                // x,y from -1 to 1
                xShifted = (xShifted / ((float) width / 2) - 1) * aspectRatio;
                yShifted = (yShifted / ((float) height / 2) - 1);

                auto tmp = r * glm::dvec1(fovScale * xShifted);
                auto tmp2 = u * (-fovScale * yShifted);
                glm::dvec3 d = direction + tmp + tmp2;
                colors[aa] = ComputeColor(eye, d);
            }
            auto col = Color::avg(iterations, colors).Clamp().GammaCorrect().ToInt();

            pixels[i0] = col;
            pixels[i1] = col;
            pixels[i2] = col;
            pixels[i3] = col;
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

#define fRand() (((float) ((int) rand())) / (RAND_MAX / 2) - 1)
#define p 0.1
    Color ComputeColor(glm::dvec3 origin, glm::dvec3 direction) {

        // auto r = Ray(origin, direction);

        Sphere *closest = nullptr;
        glm::dvec3 *hp_ = ClosestVectorFrom(origin, direction, closest);

        if (closest == nullptr) {
            delete hp_;
            return BLACK;
        }
        glm::dvec3 hp = *hp_;

        auto rnd = rand();
        if ((rnd % 1000) <= p * 1000) {
            delete hp_;
            return closest->emission;
        }

        glm::dvec3 randomDir = glm::vec3(0, 0, 0);


        glm::dvec3 n = glm::normalize(hp - closest->center);
        do {
            randomDir.x = fRand();
            randomDir.y = fRand();
            randomDir.z = fRand();
        } while (glm::length(randomDir) > 1);
        randomDir = glm::normalize(randomDir);


        if (glm::dot(randomDir, n) < 0) {
            randomDir = randomDir * glm::dvec1(-1);
        }

        auto nextEmission = ComputeColor(hp, randomDir);


        Color ownColor = closest->BRDF(0, 0) * (glm::dot(n, randomDir) * ((2 * PI) / (1 - p)));
        Color res = closest->emission + nextEmission * ownColor;
        delete hp_;
        return res;
    }
};