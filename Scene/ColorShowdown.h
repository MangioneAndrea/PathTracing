
#include "Scene.h"
#include <iostream>
class ColorShowdown : public Scene {
public:
    ColorShowdown() {
        width = 400;
        height = 400;
        pixels = (uint32_t *) malloc(sizeof(uint32_t) * width * height);
    }
    void GetPixels() override {

        // no gamma
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height / 2; y++) {
                double r = (double) x / width;
                double g = 1. - r;
                double b = (double) y / (height / 2);
                pixels[x + y * width] = Color(r, g, b).ToInt();
            }
        }
        // with gamma
        for (int x = 0; x < width; x++) {
            for (int y = height / 2; y < height; y++) {
                double r = (double) x / width;
                double g = 1. - r;
                double b = ((double) y - (width) / 2) / ((height) / 2);
                pixels[(int) x + (int) y * width] = Color(r, g, b).GammaCorrect().ToInt();
            }
        }
    }
};