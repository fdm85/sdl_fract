/*
 * orbit.h
 *
 *  Created on: 21.03.2020
 *      Author: goblin
 */

#ifndef ORBIT_H_
#define ORBIT_H_

#include <stdint.h>
#include <complex>
#include "prj_types.h"

class orbitCfg {
public:
	double M;
	double Msquare;
	uint32_t Nmax;
	std::complex<double> C;
private:

};
void fOrbit(Frame* f);
#endif /* ORBIT_H_ */
