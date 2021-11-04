//
// Created by  Andrea Mangione  on 27/09/2021.
//

#include "Scene.h"
#include <cmath>
#define iterations 160

class Texture : public Scene {
public:
    Color colors[iterations];
    uint32_t *result;
    Texture(int width, int height, unsigned char *data, int fireWidth, int fireHeight, int channels) {

        result = static_cast<uint32_t *>(malloc(fireWidth * fireHeight * sizeof(uint32_t)));
        for (int i = 0; i < fireHeight * fireWidth; i++) {
            if (channels == 3) {
                auto r = data[i * 3];
                auto g = (data[i * 3 + 1]);
                auto b = (data[i * 3 + 2]);
                auto res = b + (g << 8) + (r << 16);

                result[i] = Color(res).UndoGammaCorrect().ToInt();
            } else {
                auto r = data[i * 3];
                auto g = (data[i * 3 + 1]);
                auto b = (data[i * 3 + 2]);
                auto opacity = data[i * 3 + 3];
                auto res = b + (g << 8) + (r << 16);
                result[i] = Color(res).UndoGammaCorrect().ToInt();
            }
        }
        this->width = width;
        this->height = height;
        objects = new std::vector<Sphere *>{
                new Sphere{glm::vec3{0, -0, 0}, 0.6, result, static_cast<uint16_t>(fireHeight), static_cast<uint16_t>(fireWidth), true},
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
                new Sphere{glm::vec3{0, 101, 0}, 100, 0xffffff},
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

        auto direction = glm::normalize(lookAt - eye);
        auto r = glm::normalize(glm::cross(up, direction));
        auto u = glm::normalize(glm::cross(direction, r));

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
            glm::dvec3 d = direction + tmp + tmp2;//  lookAt.no.plus(&tmp).plus(&tmp2);

            pixels[i] = 0;

            for (int it = 0; it <= iterations; it++) {
                if (i % (width * 40) == 0) {
                    it = it;
                }
                colors[it] = ComputeColor(eye, d, (i % width), std::floor(i / width));
            }
            pixels[i] = Color::avg(iterations, colors).Clamp().GammaCorrect().ToInt();
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
    Color ComputeColor(glm::dvec3 origin, glm::dvec3 direction, int x, int y) {

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
        Color txture = closest->BRDF(x, y);

        auto nextEmission = ComputeColor(hp, randomDir, x, y);

        Color ownColor = txture * (glm::dot(n, randomDir) * ((2 * PI) / (1 - p)));
        Color res = closest->emissionF((hp.x + 1) * 300, (hp.y + 1) * 188) + nextEmission * ownColor;
        delete hp_;
        return res;
    }
};

#undef p