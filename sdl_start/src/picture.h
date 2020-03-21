/*
 * picture.h
 *
 *  Created on: 27.04.2019
 *      Author: Gruenberger
 */

#ifndef PICTURE_H_
#define PICTURE_H_
#include "prj_types.h"
void allocPicture(Frame * f, RGB2* pixBuff);
void freePicture(Frame * f);
void doTestColoring(Frame* f);
void doSwColoring(Frame* f);
#endif /* PICTURE_H_ */
