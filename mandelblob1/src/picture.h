/*
 * picture.h
 *
 *  Created on: 27.04.2019
 *      Author: Gruenberger
 */

#ifndef PICTURE_H_
#define PICTURE_H_
#include "prj_types.h"
void allocFrame(Frame * frame);
void freeFrame(Frame * f);
void doTestColoring(Frame* f);
void doSwColoring(Frame* f);
void distributeNumbers(Frame* f);
void resetFrame(Frame * f);
void cycleColoring(Frame* f);
#endif /* PICTURE_H_ */
