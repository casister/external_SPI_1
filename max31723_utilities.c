/** \file maximDeviceSpecificUtilities.c ****************************************
 * 
 *             Project: Maxim Plug-in Peripheral Modules
 *            Filename: maximDeviceSpecificUtilities.c
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

//#include "maximPMOD.h"
#include "max31723_utilities.h"
#include "max31723.h"
#include "delays.h"
#include "spi_utilities.h"
//#include "math.h"




int max_MAX31723_set_alarm(float fTemp, u32 unPeripheralAddressSPI, u8 uchAlarmType)
/**
* \brief       Set one of the MAX31723 temperature alarms.  
* \par         Details
*              This function sets either the high or low temperature alarms in the MAX31723.  The
*              units for these settings are degrees centigrade.
*
* \param[in]   fTemp                    - set point temperature in degrees C
* \param[in]   unPeripheralAddressSPI   - address of the SPI peripheral in Microblaze memory map
* \param[in]   uchAlarmType             - True = PSAVE mode on, False = PSAVE mode off
*
* \retval      Always True
*/
{
	u8 auchOutputBuffer[3];
	u8 auchReadBuffer[3];
	int nTemp=0;
	float fTempValue=0.0f;
	int nReturnVal=TRUE;
	int nValueToWrite=0;

	auchOutputBuffer[0]=0;
	auchOutputBuffer[1]=0;
	auchOutputBuffer[2]=0;
	auchReadBuffer[0]=0;
	auchReadBuffer[1]=0;
	auchReadBuffer[2]=0;

	// The upper and lower boundaries for the part are -55.0 to 125.0 degC
	fTempValue = fTemp;
	if(fTempValue >= 125.0f)
		fTempValue = 125.0f;
	else if(fTempValue <= -55.0f)
		fTempValue = -55.0f;

	// The 31723 temperature readings are signed 12 bits long, and each bit represents 1/16th of a degree C
	// bits[11:4] fill the MSB byte
	// bits[3:0] fill the LSB byte upper nibble
	//fTempValue = fTempValue  * 16.0f;
	nValueToWrite = (int)fTempValue;
	nValueToWrite = nValueToWrite << 4;

	// #define MAX31723_LOW_ALARM_WRITE 0x85
	// #define MAX31723_HIGH_ALARM_WRITE 0x83

	if(uchAlarmType==MAX31723_LOW_ALARM_WRITE)
		auchOutputBuffer[0] = MAX31723_LOW_ALARM_WRITE;
	else
		auchOutputBuffer[0] = MAX31723_HIGH_ALARM_WRITE;

	nTemp = (nValueToWrite & 0xFF0);
	nTemp = nTemp >> 4;
	auchOutputBuffer[2] = (u8)nTemp;  // MSB byte is 2nd

	nTemp = (nValueToWrite & 0x000F);			//  mask off the LSB bits
	nTemp = nTemp << 4;  						//  We only have 4 bits remaining of our 12 bit word,
												//  the lower nibble needs to be shifted to the upper nibble
	auchOutputBuffer[1] = (u8)nTemp;

	SpiRW(unPeripheralAddressSPI,1,0,(u8*)&auchOutputBuffer,(u8*)&auchReadBuffer,3,1);  // send

	return(nReturnVal);
}

int max_MAX31723_get_temp(float *fTemp, u32 unPeripheralAddressSPI, u8 uchTemperatureRegister)
/**
* \brief       Retrieve one of the the 12-bit temperature registers from the MAX31723 as a floating point (deg C)  
* \par         Details
*              This function reads any of the three temperature registers in the MAX31723.  These registers are
*              the Temperature, Thigh Alarm and Tlow Alarm.  the register to be read is specified buy the input
*              parameter uchTemperatureRegister which should be set to one of the three constants:
* \n           MAX31723_LOW_ALARM_READ, MAX31723_HIGH_ALARM_READ or MAX31723_TEMP_READ
*
* \param[out]  *fTemp                   - temperature reading is stored at fTemp
* \param[in]   unPeripheralAddressSPI   - address of the SPI peripheral in Microblaze memory map
* \param[in]   uchTemperatureRegister   - The register to be read
*
* \retval      Always True
*/
{
	u8 auchOutputBuffer[3];
	u8 auchReadBuffer[3];
	int nTemp=0;
	float fTempValue=0.0f;
	int nReturnVal=TRUE;

	auchReadBuffer[0]=0;
	auchReadBuffer[1]=0;
	auchReadBuffer[2]=0;

	// Setup the 31723 to continuously make temp readings
	auchOutputBuffer[0] = 0x80; // Configuration/status register
	auchOutputBuffer[1] = 0x06; // Default settings, except use 12 bit resolution (instead of 9) and continuously make temp conversions
	SpiRW(unPeripheralAddressSPI,1,0,(u8*)&auchOutputBuffer,(u8*)&auchReadBuffer,2,1);  // send

	// wait a half a second so that the (now 12-bit) temp sensor readings propagate to the SPI interface
	delay(ABOUT_ONE_SECOND/2);

	// Read the LSB and MSB temperature registers
	if(uchTemperatureRegister == MAX31723_LOW_ALARM_READ)
		auchOutputBuffer[0] = MAX31723_LOW_ALARM_READ; 	// 0x05 = (low) alarm register
	else if(uchTemperatureRegister == MAX31723_HIGH_ALARM_READ)
		auchOutputBuffer[0] = MAX31723_HIGH_ALARM_READ; // 0x03 = (high) alarm register
	else
		auchOutputBuffer[0] = MAX31723_TEMP_READ; 		// 0x01 = temperature register

	auchOutputBuffer[1] = 0x00; // Since this is a read register address, these extra bytes are are ignored by the 31723
	auchOutputBuffer[2] = 0x00; // Since this is a read register address, these extra bytes are are ignored by the 31723
	SpiRW(unPeripheralAddressSPI,1,0,(u8*)&auchOutputBuffer,(u8*)&auchReadBuffer,3,1);  // send

	// convert to approximate floating point
	nTemp = 0;
	nTemp = (int)auchReadBuffer[2];
	nTemp = nTemp << 4;
	nTemp |= (int)((auchReadBuffer[1] & 0xF0)>>4);
	if((nTemp & 0x00000800)==0x00000800) // is the 12bit bit set?  If so, set the other bits for the 2s complement negative value
		nTemp |= 0xFFFFF000;

	fTempValue = (float)nTemp;
	fTempValue = fTempValue / 16.0f;
	*fTemp = fTempValue;
	return(nReturnVal);
}


int number_raised_to_power(int nBase, int nExponent)
/**
* \brief       Raise nBase to the nExponent power (operates with integers only).
* \par         Details
*              Many Microblaze applications will not have math.h included due to limited memory space.
*              This is a simple functions to implement (nBase ^ nExponent)
*              Some Maxim devices (such as MAX44009) return values in mantissa + (power of 2) exponent format.
*
* \param[in]   nBase             - base
* \param[in]   nExponent         - exponent
*
* \retval      Base^Exponent
*/
{
	int i=0;
	int nValue=0;
	if(nExponent==0)
		nValue=1;
	else
	{
		nValue = nBase;
		for(i=1;i<nExponent;i++)
		{
			nValue = nValue * nBase;
		}
	}
	return(nValue);
}
