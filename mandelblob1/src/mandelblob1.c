/*
 ============================================================================
 Name        : mandelblob1.c
 Author      : cg
 Version     :
 Copyright   : none
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <time.h>
#include "bmp.h"
#include "orbit.h"
#include "picture.h"
#include "prj_types.h"
#include "config.h"


int main(void) {
	Frame f;
	OrbitConf conf;
	puts("first steps with SDL + fractls"); /* prints first steps with SDL + fractls */
    time_t start, end;
    struct tm tmp;
    time(&start);
    tmp = *localtime(&start);
    char stdOutBuf[80];
    strftime(stdOutBuf, sizeof(stdOutBuf), "Starting at %H:%M:%S \n", &tmp);
    printf ("%s \n", stdOutBuf);
    fflush(stdout);

    f.orbitConf = &conf;

    if(!loadCfgSucceedes(&f))
    {
        return EXIT_FAILURE;
    }
    allocPicture(&f);

    fOrbit(&f);

    doSwColoring(&f);

    time(&end);
    tmp = *localtime(&end);
    double rtInSec = difftime(end, start);
    printf ("Total Runtime in seconds: %.f \n", rtInSec);
    fflush(stdout);

    bool leftMouseButtonDown = false;
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window * window = SDL_CreateWindow("SDL2 Pixel Drawing",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int)f.pWidth, (int)f.pHeight, 0);

    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture * texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, (int)f.pWidth, (int)f.pHeight);

    while (!quit)
    {
    	SDL_UpdateTexture(texture, NULL, f.fRect, f.pWidth * sizeof(RGB2));

        SDL_WaitEvent(&event);

        switch (event.type)
        {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                    leftMouseButtonDown = false;
                if (event.button.button == SDL_BUTTON_MIDDLE)
                	quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    leftMouseButtonDown = true;
                break;
            case SDL_MOUSEMOTION:
                if (leftMouseButtonDown)
                {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;
                    puts("moved mouse");
                }
                break;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    SDL_Quit();


    freePicture(&f);

	return EXIT_SUCCESS;
}
