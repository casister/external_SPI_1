/** \file maximDeviceSpecificUtilities.h ****************************************
 * 
 *             Project: Maxim Plug-in Peripheral Modules
 *            Filename: maximDeviceSpecificUtilities.h
 *         Description: This module contains the functions which are specific
 *                      to the invidiual modules.  These low level functions 
 *                      are not specific to the example/demo programs and could
 *                      be cut/pasted into the user's application as a starting
 *                      point for development of an end application.
 *
 *    Revision History:
 *\n       7-20-12 Rev 1.4 Nathan Young    Initial release
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

#ifndef MAXIMDEVICESPECIFICUTILITIES_H_
#define MAXIMDEVICESPECIFICUTILITIES_H_

#include "xbasic_types.h"
#include "xspi_l.h"
#include "stdio.h"
//#include "maximPMOD.h"

#define MAX31723_TEMP_READ 0x01         //!< Read address for Temperature LSB register (MSB=0x02)
#define MAX31723_LOW_ALARM_READ 0x05    //!< Read address for Low Temperature Alarm LSB register (MSB=0x06)
#define MAX31723_HIGH_ALARM_READ 0x03   //!< Read address for High Temperature Alarm LSB register (MSB=0x04)
#define MAX31723_LOW_ALARM_WRITE 0x85   //!< Write address for Low Temperature Alarm LSB register (MSB=0x86)
#define MAX31723_HIGH_ALARM_WRITE 0x83  //!< Write address for High Temperature Alarm LSB register (MSB=0x84)

//extern XGpio g_xGpioPmodPortC;

int max_MAX31723_get_temp(float *fTemp, u32 unPeripheralAddressSPI, u8 uchTemperatureRegister);
int max_MAX31723_set_alarm(float fTemp, u32 unPeripheralAddressSPI, u8 uchAlarmType);

int number_raised_to_power(int nBase, int nExponent);

struct maximDateTime                  //!< Structure to hold time, date, day of week
{
	u8 uchHour;
	u8 uchMinute;
	u8 uchSecond;
	u8 uchMonth;
	u8 uchDate;
	u8 uchYear;
	u8 uchDow;
};

#endif /* MAXIMDEVICESPECIFICUTILITIES_H_ */
