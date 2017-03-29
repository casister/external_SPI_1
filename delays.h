/*
 * delays.h
 *
 *  Created on: Mar 7, 2017
 *      Author: cristian
 */

#ifndef SRC_DELAYS_H_
#define SRC_DELAYS_H_



#endif /* SRC_DELAYS_H_ */

#include "stdio.h"

#define ABOUT_ONE_SECOND 74067512      //!< approx 1 second delay when used as argument with function delay(numberCyclesToDelay)
// Update this if uBlaze/Zynq CPU core frequency is changed, or if the external memory timing changes.
// Although emprirically tested to 1.0000003 seconds, it is not meant to be used for precise timing purposes

void delay(int nStopValue);
