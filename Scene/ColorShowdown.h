
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
                int r = x * 255 / width;
                int g = 255 - r;
                int b = y * 255 / (height / 2);
                pixels[x + y * width] = (r << 16) + (g << 8) + b;
            }
        }
        // with gamma
        for (float x = 0; x < width; x++) {
            for (float y = ((float) height) / 2; y < height; y++) {
                float r = x * 255 / width;
                float g = 255 - r;
                float b = (y - ((float) width) / 2) * 255 / (((float) height) / 2);
                r = std::pow(r / 255, 1 / GAMMA) * 255;
                g = std::pow(g / 255, 1 / GAMMA) * 255;
                b = std::pow(b / 255, 1 / GAMMA) * 255;
                pixels[(int) x + (int) y * width] = ((int) r << 16) + ((int) g << 8) + (int) b;
            }
        }
    }
};