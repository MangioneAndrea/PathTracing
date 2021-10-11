//
// Created by andre on 10/10/2021.
//
#ifndef COLOR_H
#define COLOR_H

#include <cmath>
#include <cstdint>
#define GAMMA 2.2f

class Color {
private:
    static unsigned char sum(unsigned char a, unsigned char b) {
        unsigned char tmp = a + b;
        if (tmp < a || tmp < b)
            return 255;
        return tmp;
    }
    static unsigned char multiply(unsigned char a, unsigned char b) {
        unsigned long tmp = a * b;
        if (tmp > 255)
            return 255;
        return (unsigned char) tmp;
    }

public:
    unsigned char r;
    unsigned char g;
    unsigned char b;

    Color(uint32_t color) {
        b = (unsigned char) color;
        g = (unsigned char) (color >> 8);
        r = (unsigned char) (color >> 16);
    }

    Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}

    uint32_t ToInt() const { return (r << 16) + (g << 8) + b; }

    Color operator*(Color other) const {

        return {(unsigned char) (multiply(r, other.r)),
                (unsigned char) (multiply(g, other.g)),
                (unsigned char) (multiply(b, other.b))};
    };

    Color operator*(double m) const {
        return {(unsigned char) (r * m), (unsigned char) (g * m),
                (unsigned char) (b * m)};
    };

    Color operator+(Color other) const {
        return {sum(r, other.r), sum(g, other.g), sum(b, other.b)};
    };

    Color GammaCorrect() const {
        return {
                static_cast<unsigned char>(pow(((float) r) / 255, 1 / GAMMA) * 255),
                static_cast<unsigned char>(pow(((float) g) / 255, 1 / GAMMA) * 255),
                static_cast<unsigned char>(pow(((float) b) / 255, 1 / GAMMA) * 255),
        };
    }

    Color avg(Color other) {
        return {(unsigned char) (r / 2 + other.r / 2), (unsigned char) (g / 2 + other.g / 2),
                (unsigned char) (b / 2 + other.b / 2)};
    }
    static Color avg(int count, Color *colors) {
        double r = 0;
        double g = 0;
        double b = 0;
        for (int i = 0; i < count; i++) {
            Color c = colors[i];
            r += (double) c.r;
            g += (double) c.g;
            b += (double) c.b;
        }
        return {(unsigned char) (r / count), (unsigned char) (g / count),
                (unsigned char) (b / count)};
    }
};
static Color BLACK(0xffffff);
static Color WHITE(0x000000);
#endif