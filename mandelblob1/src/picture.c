/*
 * picture.c
 *
 *  Created on: 27.04.2019
 *      Author: Gruenberger
 */

#include "picture.h"
#include "stddef.h"
#include <stdlib.h>
#include <complex.h>
#include <stdio.h>
typedef void (colorFunc) (Frame* f, Pixel* p);
void freeFrame(Frame * f)
{
    free(f->p);
    free(f->fRect);
}
void allocFrame(Frame * f)
{
    printf ("alloc picture \n");
    fflush(stdout);
    f->pixelCount = f->pWidth * f->pHeight;
    f->p = calloc(f->pixelCount, sizeof(Pixel));
    f->fRect = calloc(f->pixelCount, sizeof(RGB2));
}

void resetFrame(Frame * f)
{
	memset(f->fRect, 255, f->pixelCount * sizeof(RGB2));
}

void distributeNumbers(Frame* f)
{
    printf ("distributeNumbers \n");
    fflush(stdout);
    double dx = (f->fWidth/f->pWidth);
    double dy = (f->fHeight/f->pHeight);
    for (uint32_t i = 0u; i < f->pHeight; ++i)
    {
        double im = f->yStart - i*dy;
        for (uint32_t j = 0u; j < f->pWidth; ++j)
        {
            double re = f->xStart + j*dx;
            Pixel *k = &(f->p[(i * f->pWidth) +j]);
            k->xPos = j;
            k->yPos = i;

            k->Z0 = re + im*I;
            k->color = &(f->fRect[(i * f->pWidth) +j]);
        }
    }
}

static void doIteration(Frame* f, colorFunc func)
{
    static uint32_t Nmax = 0;
    static uint8_t C = 0u ,Cmax = 0;
    for(uint32_t i = 0; i < f->pHeight; i++)
    {
        for(uint32_t j = 0; j < f->pWidth; j++)
        {
            Pixel *k = &(f->p[(i * f->pWidth) + j]);
            func(f, k);
        }
    }
}
static void tD(Frame* f, Pixel* k)
{
    k->color->red = (unsigned char) ((double) k->yPos/ f->pHeight * 255); ///red
    k->color->green = (unsigned char) ((double) k->xPos / f->pWidth * 255); ///green
    k->color->blue = (unsigned char) (((double) k->yPos + k->xPos) / (f->pHeight + f->pWidth) * 255); ///blue
//    k->color->red = k->yPos;
//    k->color->green = k->xPos;
//    k->color->blue = 0xFB;

}
void doTestColoring(Frame* f)
{
    doIteration(f, tD);
}

static void sW(Frame* f, Pixel* k)
{
    k->color->red = k->color->green = k->color->blue = (unsigned char) ( ((double)k->n/f->orbitConf->Nmax) * 255);
}
void doSwColoring(Frame* f)
{
    printf ("doSwColoring \n");
    fflush(stdout);
    doIteration(f, sW);
}
