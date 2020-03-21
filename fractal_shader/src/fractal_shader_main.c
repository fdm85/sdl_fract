/*
 ============================================================================
 Name        : fractal_shader.c
 Author      : cg
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <time.h>
#include "bmp.h"
#include "orbit.h"
#include "picture.h"
#include "prj_types.h"
#include "config.h"

Frame f;
OrbitConf conf;
int main(int argc, char *argv[])
{
    time_t start, end;
    struct tm tmp;
    time(&start);
    tmp = *localtime(&start);
    char stdOutBuf[80];
    strftime(stdOutBuf, sizeof(stdOutBuf), "Starting at %H:%M:%S \n", &tmp);
    printf ("%s \n", stdOutBuf);
    fflush(stdout);

    f.orbitConf = &conf;

    if(!loadCfgSucceedes(&f, argc, argv))
    {
        return EXIT_FAILURE;
    }
    allocPicture(&f);

    fOrbit(&f);

    doSwColoring(&f);
    generateBitmapImage("Sw.bmp", &f);
    freePicture(&f);

    time(&end);
    tmp = *localtime(&end);
    double rtInSec = difftime(end, start);
    printf ("Total Runtime in seconds: %.f \n", rtInSec);
    fflush(stdout);
    return EXIT_SUCCESS;
}
