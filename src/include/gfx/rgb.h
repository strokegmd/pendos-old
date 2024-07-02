//
// Created by kukuv on 28.06.2024.
//

#ifndef PENDOS_RGB_H
#define PENDOS_RGB_H

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} pixel_t;

/*
Gets RGB pixel in HEX
*/
uint32_t from_rgb(pixel_t px) {
    return (px.r << 16) | (px.g << 8) | px.b;
}

pixel_t from_hex(uint32_t color) {
    pixel_t rgb;
    rgb.r = (color >> 16) & 0xFF;
    rgb.g = (color >> 8) & 0xFF;
    rgb.b = color & 0xFF;
    return rgb;
}

pixel_t darker(pixel_t px) {
    pixel_t pixel = {px.r-10, px.g-10, px.b-10};
    if (pixel.r < 0) pixel.r = 0;
    if (pixel.g < 0) pixel.g = 0;
    if (pixel.b < 0) pixel.b = 0;
    return pixel;
}

pixel_t brighter(pixel_t px) {
    pixel_t pixel = {px.r+10, px.g+10, px.b+10};
    if (pixel.r > 255) pixel.r = 255;
    if (pixel.g > 255) pixel.g = 255;
    if (pixel.b > 255) pixel.b = 255;
    return pixel;
}

#endif //PENDOS_RGB_H
