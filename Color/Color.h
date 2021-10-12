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
    static uint32_t sum(uint32_t a, uint32_t b) {
        uint32_t tmp = a + b;
        return tmp;
    }
    static uint32_t multiply(uint32_t a, uint32_t b) {
        unsigned long tmp = a * b / 255;
        return (uint32_t) tmp;
    }

public:
    uint32_t r;
    uint32_t g;
    uint32_t b;

    explicit Color(uint32_t color) {
        b = color & 0x0000FF;
        g = (color & 0x00FF00) >> 8;
        r = (color & 0xFF0000) >> 16;
    }

    [[maybe_unused]] Color(uint32_t r, uint32_t g, uint32_t b) : r(r), g(g), b(b) {}

    uint32_t ToInt() const {
        Color c = Clamp();
        return (c.r << 16) + (c.g << 8) + c.b;
    }

    Color Clamp() const {
        return {
                r > 255 ? 255 : r,
                g > 255 ? 255 : g,
                b > 255 ? 255 : b,
        };
    }

    Color operator*(Color other) const {
        return {(uint32_t) (multiply(r, other.r)),
                (uint32_t) (multiply(g, other.g)),
                (uint32_t) (multiply(b, other.b))};
    };

    Color operator*(double m) const {
        return {(uint32_t) (r * m), (uint32_t) (g * m),
                (uint32_t) (b * m)};
    };

    Color operator+(Color other) const {
        return {sum(r, other.r), sum(g, other.g), sum(b, other.b)};
    };

    [[maybe_unused]] Color GammaCorrect() const {
        return {
                static_cast<uint32_t>(pow(((float) r) / 255, 1 / GAMMA) * 255),
                static_cast<uint32_t>(pow(((float) g) / 255, 1 / GAMMA) * 255),
                static_cast<uint32_t>(pow(((float) b) / 255, 1 / GAMMA) * 255),
        };
    }

    Color avg(Color other) {
        return {(uint32_t) (r / 2 + other.r / 2), (uint32_t) (g / 2 + other.g / 2),
                (uint32_t) (b / 2 + other.b / 2)};
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
        return {(uint32_t) (r / count), (uint32_t) (g / count),
                (uint32_t) (b / count)};
    }
};
[[maybe_unused]] static Color BLACK(0x000000);
[[maybe_unused]] static Color WHITE(0xFFFFFF);
#endif