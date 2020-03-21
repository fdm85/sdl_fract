/*
 * orbit.cpp
 *
 *  Created on: 21.03.2020
 *      Author: goblin
 */


#include "orbit.h"
#include <complex.h>

static void fDo(Pixel* p, OrbitConf* conf)
{
    clx Zn = p->Z0;
    for(uint32_t n = 0uL; n < conf->Nmax; ++n)
    {
        Zn = Zn * Zn + p->Z0;
        double abs = cabs(Zn);
        if(abs > conf->M)
        {
            p->Zn = Zn;
            p->n = n;
            break;
        }
    }
}


void fOrbit(Frame* f)
{
    printf ("fOrbit \n");
    fflush(stdout);
    for (uint32_t i = 0u; i < f->pWidth; ++i)
    {
        if(!(i%100) )
        {
            printf("current at I: %d | %d \n", i, f->pWidth);
            fflush(stdout);
        }
        for (uint32_t j = 0u; j < f->pHeight; ++j)
        {

            Pixel *k = &(f->p[(i * f->pWidth) +j]);
            fDo(k, f->orbitConf);
        }
    }
}
