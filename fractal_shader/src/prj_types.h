/*
 * pixel.h
 *
 *  Created on: 27.04.2019
 *      Author: Gruenberger
 */

#ifndef PRJ_TYPES_H_
#define PRJ_TYPES_H_

#include <stdbool.h>
#include <complex.h>
#include <stdint.h>
typedef struct _RGB
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
}RGB;

typedef union _RGB2
{
    struct
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    };
    uint32_t raw;
} RGB2;

typedef struct _Pixel
{
    uint32_t xPos;
    uint32_t yPos;
    double complex Z0;
    double complex Zn;
    uint32_t n;
    RGB color;
}Pixel;

typedef struct _OrbitConf
{
    double M;
    double Msquare;
    uint32_t Nmax;
    double complex C;
} OrbitConf;

typedef struct _Frame
{
    uint32_t pWidth;
    uint32_t pHeight;
    double fWidth;
    double fHeight;
    double xStart;
    double yStart;
    uint32_t pixelCount;
    OrbitConf* orbitConf;
    Pixel* p;
}Frame;

#endif /* PRJ_TYPES_H_ */
