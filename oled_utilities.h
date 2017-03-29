/** \file utilities.h ********************************************************
 * 
 *             Project: Maxim Plug-in Peripheral Modules
 *            Filename: utilities.h
 *         Description: This module contains a collection of general utility
 *                      functions which are not specific to any particular
 *                      module.
 * 
 *    Revision History:
 *\n       4-13-12 Rev 1.0 Seth Messimer   Initial Release
 *\n       7-20-12 Rev 1.4 Nathan Young    Additional functions
 *\n       9-04-12 Rev 1.5 Nathan Young    Multiple PMOD port support
  *\n       10-15-12 Rev 1.6 Nathan Young	OLED display support
 * 
 *  --------------------------------------------------------------------
 * 
 *    This code follows the following naming conventions.
 * 
 * 	  char                   chPmodValue
 * 	  char (array)           sPmodString[16]
 * 	  float                  fPmodValue
 * 	  int                    nPmodValue
 * 	  int (array)            anPmodValue[16]
 * 	  u16                    uPmodValue
 * 	  u8                     uchPmodValue
 * 	  u8 (array)             auchPmodBuffer[16]
 * 	  unsigned int           unPmodValue
 * 	  int *                  punPmodValue
 * 
 * ------------------------------------------------------------------------- */
/*
 * Copyright (C) 2012 Maxim Integrated Products, All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY,  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED PRODUCTS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated Products
 * shall not be used except as stated in the Maxim Integrated Products
 * Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products retains all ownership rights.
 *
 ***************************************************************************/

#ifndef UTILITIES_H_
#define UTILITIES_H_
#include "xbasic_types.h"   
#include "xspi_l.h"         
#include "stdio.h"          
//#include "xiic_l.h"
//#include "xuartlite_i.h"
#include "xparameters.h"    
#include "xgpio.h"          
#include "xgpio_l.h"        
//#include "maximPMOD.h"
#endif /* UTILITIES_H_ */

#pragma once

struct maximOLEDDisplay
{
	XGpio xgpioPort;
	u8 portStatus;
	u8 writeBuffer[512];
	u8 flippedBuffer[512];
	u8 *font;
};

struct maximOLEDDisplay g_structureOLED;
char g_tempString[32];

void initializeOLED(u8 *pFont);
void sendOLEDSPI(u8 uchDataToWrite);
void clearOLEDBuffer(u8 *pauchBuffer);
void displayOLEDBuffer(u8 *pauchBuffer);
void putCharOLED(int x, int y, char chCharacter);
void printfToBufferOLED(int x, int y,char *chString);
void printfToOLED(int x, int y,char *chString);
void flipAndCopyDisplayBuffer(u8 *pauchSourceBuffer, u8 *pauchDestinationBuffer);
void printOLED_31855(float fInternalTemp,float fProbeTemp, int displayCelsius);
void printOLED_31723(float fTemp);
//void printOLED_3231M(struct maximDateTime t, float fTemp);
void printOLED_44000Lux(float fLuxReading);
void printOLED_44000Prox(float fProxReading);
