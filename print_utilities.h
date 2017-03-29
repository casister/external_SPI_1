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
#include "xgpio.h"          
#include "xgpio_l.h"        
//#include "maximPMOD.h"
#endif /* UTILITIES_H_ */

#pragma once

//====================================================================================================
// Function:  SpiRW()
//
// @param unBaseAddr:  			Base address of SPI master.  Find it in "xparameters.h".
// @param unCPHA:				CPHA setting for SPI port.  See IC datasheet.
// @param unCPOL:				CPOL setting for SPI port.  See IC datasheet.
// @param auchWriteBuf:			Buffer containing write data for transfer.  Set to 0 if not writing data.
// @param auchReadBuf:			Buffer into which read data will be placed.  Set to 0 if not reading data.
// @param unNumBytes:			Number of bytes to be read/written.
// @param uchCsActiveHigh:		1 if CS is active high, 0 if CS is active low.
//
// @return:						0 if successful, nonzero otherwise.  Note -- error checking not completed.
//====================================================================================================

void print_asterisks(int nQuantity);
void printf_temp(float fTemp, u8 uchPrintCelsius, u8 uchAddCarriageReturn);

void menu_cls();
void menu_print_maxim_banner();
void menu_print_maxim_banner_big();
void menu_print_prompt();
void menu_print_line();
