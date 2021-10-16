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
    static double sum(double a, double b) {
        double tmp = a + b;
        return tmp;
    }
    static double multiply(double a, double b) {
        return (double) a * b;
    }

public:
    double r;
    double g;
    double b;

    explicit Color(uint32_t color) {
        b = (double) (color & 0x0000FF) / 255;
        g = (double) ((color & 0x00FF00) >> 8) / 255;
        r = (double) ((color & 0xFF0000) >> 16) / 255;
    }

    [[maybe_unused]] Color(double r, double g, double b) : r(r), g(g), b(b) {}

    uint32_t ToInt() const {
        Color c = Clamp();
        return (((uint32_t) (c.r * 255)) << 16) + (static_cast<uint32_t>(c.g * 255) << 8) + c.b * 255;
    }

    Color Clamp() const {
        return {
                r > 1 ? 1 : r,
                g > 1 ? 1 : g,
                b > 1 ? 1 : b,
        };
    }

    Color operator*(Color other) const {
        return {(double) (multiply(r, other.r)),
                (double) (multiply(g, other.g)),
                (double) (multiply(b, other.b))};
    };

    Color operator*(double m) const {
        return {(double) (r * m), (double) (g * m),
                (double) (b * m)};
    };

    Color operator+(Color other) const {
        return {sum(r, other.r), sum(g, other.g), sum(b, other.b)};
    };

    [[maybe_unused]] Color GammaCorrect() const {
        return {
                static_cast<double>(pow((r), 1 / GAMMA)),
                static_cast<double>(pow((g), 1 / GAMMA)),
                static_cast<double>(pow((b), 1 / GAMMA)),
        };
    }

    Color avg(Color other) {
        return {(double) (r / 2 + other.r / 2), (double) (g / 2 + other.g / 2),
                (double) (b / 2 + other.b / 2)};
    }
    static Color avg(int count, Color *colors) {
        double r = 0;
        double g = 0;
        double b = 0;
        for (int i = 0; i < count; i++) {
            Color c = colors[i];
            r += c.r;
            g += c.g;
            b += c.b;
        }
        return {r / count, g / count, b / count};
    }
};
#define BLACK Color(0x000000);
#define WHITE Color(0xFFFFFF);
#endif