//
// Created by kukuv on 28.06.2024.
//

#ifndef PENDOS_MATH_H
#define PENDOS_MATH_H

#include <stdint.h>

double fabs(double x) {
    return (x < 0.0)? -x : x;
}

uint32_t clamp(uint32_t value, uint32_t min, uint32_t max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

int sqrt_int(int num) {
    int x = num;
    int y = 1;

    while (x - y > 1) {
        x = (x + y) / 2;
        y = num / x;
    }

    return x;
}

double sqrt_double(double num) {
    double x = num;
    double y = 1.0;

    while (fabs(x - y) > 0.00001) {
        x = (x + y) / 2.0;
        y = num / x;
    }

    return x;
}

#endif //PENDOS_MATH_H
