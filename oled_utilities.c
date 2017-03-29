/** \file utilities.c ********************************************************
 * 
 *             Project: Maxim Plug-in Peripheral Modules
 *            Filename: utilities.c
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

#include "oled_utilities.h"
#include "delays.h"
#include "max31723.h"
//#include "maximPMOD.h"

#define OLED_VBAT 0x20
#define OLED_VDD 0x10;
#define OLED_RESET_B 0x08
#define OLED_DATA_COMMAND_B 0x04
#define OLED_SDIN 0x02
#define OLED_SCLK 0x01

void sendOLEDSPI(u8 uchDataToWrite)
/**
* \brief       Bit bang routine for Zedboard SPI OLED interface
* \par         Details
*              This function sends an (unsigned) byte (msb first) to the OLED SPI port
*
* \param[in]   uchDataToWrite          - u8 value to be written to OLED SPI
* \retval      None
*/
{
	int i;

	for(i=7;i>=0;i--)
	{
		// Set the OLED_SDIN bit
		if(((uchDataToWrite >> i) & 0x01)==0x01)
			g_structureOLED.portStatus |= OLED_SDIN;
		else
			g_structureOLED.portStatus &= ~OLED_SDIN;
		XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, g_structureOLED.portStatus);
		delay(100);

		// Clock in the data via a rising edge of SCLK
		g_structureOLED.portStatus |= OLED_SCLK;
		XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, g_structureOLED.portStatus);
		delay(100);
		g_structureOLED.portStatus &= ~OLED_SCLK;
		XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, g_structureOLED.portStatus);
		delay(100);
	}
}

void initializeOLED(u8 *pFont)
/**
* \brief       Initializes the OLED display
* \par         Details
*              This function initializes the OLED display and the global data structure that holds the OLED display buffer and font.
*
* \param[in]   *pFont          - pointer to an u8 array containing the ASCII display font
* \retval      None
*/
{
	// First, initialize the global structure that represents the OLED device

	//GPIO that manages the bit-banged SPI interface to the OLED
	//XGpio_Initialize(&g_structureOLED.xgpioPort, XPAR_AXI_GPIO_OLED_DEVICE_ID);
	XGpio_Initialize(&g_structureOLED.xgpioPort, XPAR_AXI_GPIO_OLED_DEVICE_ID);
	XGpio_SetDataDirection(&g_structureOLED.xgpioPort, 1, 0x00);	// Set the OLED peripheral to outputs
	XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, 0x00);		// Set all values to zero
	delay(100);

	// Set the font in the OLED structure
	g_structureOLED.font = pFont;

	// Init the global variable that represents the OLED bit-bang pins
	g_structureOLED.portStatus = 0x00;

	// Init and clear the display buffer and the invertered/flipped buffer
	clearOLEDBuffer(g_structureOLED.writeBuffer);
	clearOLEDBuffer(g_structureOLED.flippedBuffer);

	// Now, Initialize the OLED display (hardware)

	// Disable VBAT power supply to the OLED integrated switcher
	// Note:  These bits control the Gate voltage on a P-Channel MOSFET
	g_structureOLED.portStatus |= OLED_VBAT;
	XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, g_structureOLED.portStatus);		// Set all values to zero
	delay(ABOUT_ONE_SECOND/5);

	//Enable VDD power supply to digital logic within OLED
	g_structureOLED.portStatus |= OLED_VDD;
	XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, g_structureOLED.portStatus);		// Set all values to zero
	delay(ABOUT_ONE_SECOND/5);

	// Disable reset
	g_structureOLED.portStatus |= OLED_RESET_B;
	XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, g_structureOLED.portStatus);		// Set all values to zero
	delay(ABOUT_ONE_SECOND/5);

	// Set Display Off
	sendOLEDSPI(0xAE);
	delay(100);

	// Set Display Clock Divide Ratio.  Oscillator Frequency
	sendOLEDSPI(0xD5);
	delay(100);
	sendOLEDSPI(0x80);
	delay(100);

	// Set Multiplex Ratio
	sendOLEDSPI(0xA8);
	delay(100);
	sendOLEDSPI(0x1F);
	delay(100);

	// Set Display Offset
	sendOLEDSPI(0xD3);
	delay(100);
	sendOLEDSPI(0x00);
	delay(100);

	// Set Display Start Line
	sendOLEDSPI(0x40);
	delay(100);

	// Set Charge Pump
	sendOLEDSPI(0x8D);
	delay(100);
	sendOLEDSPI(0x14);
	delay(100);

	// Set Segment Re-Map
	sendOLEDSPI(0xA1);
	delay(100);

	// Set COM Output Scan Direction
	sendOLEDSPI(0xC8);
	delay(100);

	// Set Com Pins Hardware Configuration
	sendOLEDSPI(0xDA);
	delay(100);
	sendOLEDSPI(0x02);
	delay(100);

	// Set Contrast Control
	sendOLEDSPI(0x81);
	delay(100);
	sendOLEDSPI(0x8F);
	delay(100);

	// Set Pre-Charge Period
	sendOLEDSPI(0xD9);
	delay(100);
	sendOLEDSPI(0xF1);
	delay(100);

	// Set VCOMH Deselect Level
	sendOLEDSPI(0xDB);
	delay(100);
	sendOLEDSPI(0x40);
	delay(100);

	// Set Entire Display On
	sendOLEDSPI(0xA4);
	delay(100);

	// Set Normal/Inverse Display
	sendOLEDSPI(0xA6);
	delay(100);

	// Enable the VBAT power supply (which is used for the OLED)
	g_structureOLED.portStatus &= ~OLED_VBAT;
	XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, g_structureOLED.portStatus);		// Set all values to zero
	delay(ABOUT_ONE_SECOND/2);	// Long delay for internal OLED switcher to stabilize

	// Set Display On
	sendOLEDSPI(0xAF);
	delay(100);
}

void clearOLEDBuffer(u8 *pauchBuffer)
/**
* \brief       Clears the OLED display buffer
* \par         Details
*              This function clears the display buffer by setting all pixels to zero.
*
* \param[in]   *pauchBuffer          - pointer to an u8 array used for the OLED pixel buffer
* \retval      None
*/
{
	int i;

	for(i=0;i<512;i++)
		pauchBuffer[i] = 0x00;

}

void displayOLEDBuffer(u8 *pauchBuffer)
/**
* \brief       Copies the display buffer into the OLED display
* \par         Details
*              This function writes the the display buffer into the OLED display using the page mode setting.
* \n           Note:  The Zedboard OLED display contains a Solomon SSD1306 display controller.
*
* \param[in]   *pauchBuffer          - pointer to an u8 array used for the OLED pixel buffer
* \retval      None
*/
{
	int column=0;
	int page=0;
	u8 *p;
	u8 uchPixelValue=0;

	// Set the display mode to page based transfers
	sendOLEDSPI(0x20);
	delay(100);
	sendOLEDSPI(0x02);
	delay(100);

	// Set the page pointer lower nibble to 0x0
	sendOLEDSPI(0x00);
	delay(100);

	// Set the page pointer upper nibble to 0x0
	sendOLEDSPI(0x10);
	delay(100);

	// Write 4 pages worth of data
	// Note that the SSD1306 controller can handle 128x64 displays, but the OLED
	// used on zedboard is only 128x32.  Data is provided to the display as (4) pages of 128 bytes each.
	p = pauchBuffer;
	for(page=0;page<4;page++)
	{
		// Enable Command Mode (shut off data mode)
		g_structureOLED.portStatus &= ~OLED_DATA_COMMAND_B;
		XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, g_structureOLED.portStatus);		// Set all values to zero
		delay(100);	// Long delay for internal OLED switcher to stabilize

		// Set the page command by issuing 0xB0 (page0), 0xB1 (page1) etc.
		sendOLEDSPI((0xB0+page));

		// Enable the Data mode (shut off command mode)
		g_structureOLED.portStatus |= OLED_DATA_COMMAND_B;
		XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, g_structureOLED.portStatus);		// Set all values to zero
		delay(100);	// Long delay for internal OLED switcher to stabilize

		for(column=0;column<128;column++)
		{
			uchPixelValue = *p;
			sendOLEDSPI(uchPixelValue); // change this to zero
			p++;
		}
	}

	// Enable Command Mode (shut off data mode)
	g_structureOLED.portStatus &= ~OLED_DATA_COMMAND_B;
	XGpio_DiscreteWrite(&g_structureOLED.xgpioPort, 1, g_structureOLED.portStatus);		// Set all values to zero
	delay(100);	// Long delay for internal OLED switcher to stabilize
}

void putCharOLED(int x, int y, char chCharacter)
/**
* \brief       Places a single ASCII character into the OLED display buffer
* \par         Details
*              This function copies an ASCII character from the font table into the OLED display buffer (configured as a 16x4 character display).
*
* \param[in]   x          - The x position of the character location
* \param[in]   y          - The y position of the character location
* \param[in]   chCharacter          - ASCII character to be printed (ex:  0x41 = 'A')
* \retval      None
*/
{
	int i;
	int nDisplayBufferIndex=0;
	int nFontIndex=0;

	// Create the index location for the character into the display memory
	nDisplayBufferIndex = (x * 8) + (y * 16 * 8);

	// Make sure the character has a valid value, else ignore
	if(chCharacter >=0 && chCharacter<128)
	{
		nFontIndex = chCharacter * 8;

		for(i=nDisplayBufferIndex;i<nDisplayBufferIndex+8;i++)
		{
			g_structureOLED.writeBuffer[i] = g_structureOLED.font[nFontIndex];
			nFontIndex++;
		}
	}
}

void printfToBufferOLED(int x, int y,char *chString)
/**
* \brief       Printf-like function to copy an ASCII string into the OLED display buffer
* \par         Details
*              This function copies a pre-formatted ASCII string into the OLED display buffer, but does not take any action to display the buffer
*
* \param[in]   x          - The starting position of string within the 16x4 character display
* \param[in]   y          - The starting position of string within the 16x4 character display
* \param[in]   *chString          - pointer to an null terminated character string.  Should be generated with sprintf (or similar).
* \retval      None
*/
{
	int nStringLength;
	int nInitialDisplayLocation;
	int nRemainingDisplayLength;
	int i;
	int xIndex=0;
	int yIndex=0;
	nStringLength = strlen(chString);

	// Will the string fit on the display?  If not, truncate the length
	nInitialDisplayLocation = y*16 + x;
	nRemainingDisplayLength = 64 - nInitialDisplayLocation;
	if(nStringLength > nRemainingDisplayLength)
		nStringLength = nRemainingDisplayLength;

	xIndex = x;
	yIndex = y;
	for(i=0;i<nStringLength;i++)
	{
		putCharOLED(xIndex,yIndex,chString[i]);
		xIndex++;
		if(xIndex==16)
		{
			xIndex=0;
			yIndex++;
		}
	}
}

void printfToOLED(int x, int y,char *chString)
/**
* \brief       Printf-like function to copy an ASCII string into the OLED display buffer
* \par         Details
*              This function copies a pre-formatted ASCII string into the OLED display buffer,
*              flips/rotates the display pixels, and then sends the buffer to the OLED display
*
* \param[in]   x          - The starting position of string within the 16x4 character display
* \param[in]   y          - The starting position of string within the 16x4 character display
* \param[in]   *chString          - pointer to an null terminated character string.  Should be generated with sprintf (or similar).
* \retval      None
*/
{
	printfToBufferOLED(x,y,chString);
	flipAndCopyDisplayBuffer(g_structureOLED.writeBuffer, g_structureOLED.flippedBuffer);
	displayOLEDBuffer(g_structureOLED.flippedBuffer);
}


void flipAndCopyDisplayBuffer(u8 *pauchSourceBuffer, u8 *pauchDestinationBuffer)
/**
* \brief       Copies the pixels from the source display buffer, into the destination buffer, flipping their location 180 degrees.
* \par         Details
*              The (0,0) pixel location on the OLED display is in the bottom right corner.
*              This function flips the display so that the (0,0) is in the top left corner and (131,31) is in the bottom right.
*
* \param[in]   *pauchSourceBuffer          - pointer to an u8 buffer used as the source display buffer
* \param[in]   *pauchDestinationBuffer     - pointer to an u8 buffer used as the destination display buffer
* \retval      None
*/
{
	int i,x,y;
	u8 uchTempWord1;
	u8 uchTempWord2;
	u8 uchTempWord3;

	for(y=0;y<4;y++)
	{
		for(x=0;x<128;x++)
		{
			uchTempWord1 = pauchSourceBuffer[511-((y*128)+x)];
			uchTempWord3 = 0;
			for(i=0;i<8;i++)
			{
				uchTempWord2 = (uchTempWord1 >> (7-i)) & 0x01;  // shift the bit to the bit0 location and mask off
				uchTempWord2 = uchTempWord2 << i;				// shift the bit back to the desired location.
				uchTempWord3 += uchTempWord2;
			}
			pauchDestinationBuffer[((y*128)+x)] = uchTempWord3;
		}
	}
}



void printOLED_44000Lux(float fLuxReading)
/**
* \brief       Prints the MAX44000 Lux data to the OLED
* \par         Details
*              Uses the printfToOLED function to print the MAX44000 lux data to the OLED
*
* \param[in]   fLuxReading - floating point value representing the lux reading
* \retval      None
*/
{
	clearOLEDBuffer(g_structureOLED.writeBuffer);
	sprintf(g_tempString,"Ambient Light:");
	printfToOLED(0,0,g_tempString);
	sprintf(g_tempString,"%.1f (Lux)",fLuxReading);
	printfToOLED(0,1,g_tempString);
}

void printOLED_44000Prox(float fProxReading)
/**
* \brief       Prints the MAX44000 Proximity data to the OLED
* \par         Details
*              Uses the printfToOLED function to print the MAX44000 prox data to the OLED
*
* \param[in]   fProxReading - floating point value representing the prox reading
* \retval      None
*/
{
	clearOLEDBuffer(g_structureOLED.writeBuffer);
	sprintf(g_tempString,"Proximity Val:");
	printfToOLED(0,0,g_tempString);
	sprintf(g_tempString,"%.1f",fProxReading);
	printfToOLED(0,1,g_tempString);
}


