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
void updateWidthAndHeight(Frame *const f, double zoom, double xOffset, double yOffset);
#endif /* CONFIG_H_ */
