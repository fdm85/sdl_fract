/*
 * orbit.c
 *
 *  Created on: 27.04.2019
 *      Author: Gruenberger
 */

#include "orbit.h"
#include <stdio.h>
#include <complex.h>
#include <SDL.h>

SDL_atomic_t threads;
SDL_atomic_t iA;

static void fDo(Pixel *p, OrbitConf *conf) {
	double complex Zn = p->Z0;
	double abs;
	uint32_t n = 0uL;
	for (; n < conf->Nmax; ++n) {
		Zn = Zn * Zn + p->Z0;
		abs = cabs(Zn);
		if (abs > conf->M) {
			break;
		}
	}
	p->Zn = Zn;
	p->n = n;
	p->abs = abs;
}

int fOrbitRow(void *p) {
	Frame *f = (Frame*) p;
	uint16_t i = SDL_AtomicAdd(&iA, 1);
	for (uint32_t j = 0u; j < f->pHeight; ++j) {
		Pixel *k = &(f->p[(i * f->pWidth) + j]);
		fDo(k, f->orbitConf);
	}
	SDL_AtomicAdd(&threads, -1);
	return 0;
}

int fOrbit(void* p) {
	Frame *f = (Frame *)p;
	printf("fOrbit \n");
	fflush(stdout);
	SDL_AtomicSet(&threads, 0);
	SDL_AtomicSet(&iA, 0);
	while(SDL_AtomicGet(&iA) < (int)(f->pWidth - 1)) {
		if (SDL_AtomicGet(&threads) < 9) {
			SDL_AtomicAdd(&threads, 1);
			SDL_Thread *thread = SDL_CreateThread(fOrbitRow, "fOrbitRow", (void*) f);
			/* Simply create a thread */
			SDL_DetachThread(thread);
		}
	}
	while(SDL_AtomicGet(&threads) != 0);
	return 0;
}

