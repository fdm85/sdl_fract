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
	printf("%s \n", stdOutBuf);
	fflush(stdout);

	f.orbitConf = &conf;

	if (!loadCfgSucceedes(&f)) {
		return EXIT_FAILURE;
	}
	allocFrame(&f);
	distributeNumbers(&f);
	fOrbit(&f);

	doSwColoring(&f);

	time(&end);
	tmp = *localtime(&end);
	double rtInSec = difftime(end, start);
	printf("Total Runtime in seconds: %01.4f \n", rtInSec);
	fflush(stdout);

	bool quit = false;
	Sint32 mouseXd;
	Sint32 mouseYd;
	Sint32 mouseXDiff;
	Sint32 mouseYDiff;
	SDL_Event event;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *window = SDL_CreateWindow("SDL2 Pixel Drawing",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, (int) f.pWidth,
			(int) f.pHeight, 0);

	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING, (int) f.pWidth, (int) f.pHeight);

	while (!quit) {
		SDL_UpdateTexture(texture, NULL, f.fRect, f.pWidth * sizeof(RGB2));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		if (SDL_PollEvent(&event)) {

			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEWHEEL:
				if (event.wheel.y) {
					double zoom = event.wheel.y;
					zoom /= 50.0f;
					updateWidthAndHeight(&f, 1.0f - zoom, 0.0f, 0.0f);
					distributeNumbers(&f);
					SDL_Thread *thread = SDL_CreateThread(fOrbit, "fOrbit",
							(void*) &f);
					SDL_WaitThread(thread, NULL);
					doSwColoring(&f);
				}
				break;
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT) {
					mouseXDiff = event.button.x - mouseXd;
					mouseYDiff = event.button.y - mouseYd;
					printf("moved mouse, x: %4d, y: %4d \n", mouseXDiff,
							mouseYDiff);
					updateWidthAndHeight(&f, 1.0f, mouseXDiff, mouseYDiff);
					distributeNumbers(&f);
					SDL_Thread *thread = SDL_CreateThread(fOrbit, "fOrbit",
							(void*) &f);
					SDL_WaitThread(thread, NULL);
					doSwColoring(&f);
				}
				if (event.button.button == SDL_BUTTON_MIDDLE)
					quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT) {
					mouseXd = event.button.x;
					mouseYd = event.button.y;
				}
				break;
			}
		} else {
			cycleColoring(&f);
		}
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);

	SDL_DestroyWindow(window);
	SDL_Quit();

	freeFrame(&f);

	return EXIT_SUCCESS;
}
