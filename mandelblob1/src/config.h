/*
 * config.h
 *
 *  Created on: 01.05.2019
 *      Author: Gruenberger
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include "prj_types.h"

bool loadCfgSucceedes(Frame *const f);
void updateWidthAndHeight(Frame *const f, double zoom, int32_t xOffset, int32_t yOffset);
#endif /* CONFIG_H_ */
