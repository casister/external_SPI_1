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

#include "stdio.h"
#include "xbasic_types.h"
//#include "maximPMOD.h"

void print_asterisks(int nQuantity)
/**
* \brief       Print nQuantity of asterisks to the default Hyperterminal UART
*
* \param[in]   nQuantity    - number of asterisks to print
*
* \retval      None
*/
{
	int i=0;
	for(i=0;i<nQuantity;i++)
		printf("*");
}

void printf_temp(float fTemp, u8 uchPrintCelsius, u8 uchAddCarriageReturn)
/**
* \brief       Print temperature as xx.x as degrees F or C
* \par         Details
*              This function prints the temperature with .1 degree resolution in either degrees
* \n           Celsius (uchPrintCelsius==TRUE) -or- Fahrenheit (uchPrintCelsius==FALSE)
*
* \param[in]   fTemp                   - temperature to print in celsius
* \param[in]   uchPrintCelsius         - true = print as celsius, false = print as fahrenheit
* \param[in]   uchAddCarriageReturn    - true = add a carriage return
*
* \retval      None
*/
{
	if(uchPrintCelsius==TRUE)
		printf("%.1f deg C",fTemp);
	else
		printf("%.1f deg F",((fTemp*1.8f)+32.0f));
	if(uchAddCarriageReturn==TRUE)
		printf("\r\n");
}

void menu_cls()
/**
* \brief       Function to clear the screen via Hyperterminal
*
* \param       None
*
* \retval      None
*/
{
	// The following code will cause a clear screen event on Hyperterminal and many other terminal emulators
	printf("\033[2J");
}


void menu_print_maxim_banner()
/**
* \brief       Print standard Maxim banner at top of Hyperterminal screen
*
* \param       None
*
* \retval      None
*/
{
	printf( "///////////////////////////////////////////////////////////////////\r\n");
	printf( "//         ##     ##     ##     #  #  #######  ##     ##         //\r\n");
	printf( "//         # #   # #    #  #     ##      #     # #   # #         //\r\n");
	printf( "//         #  # #  #   ######    ##      #     #  # #  #         //\r\n");
	printf( "//         #   #   #  #      #  #  #  #######  #   #   #         //\r\n");
	printf( "///////////////////////////////////////////////////////////////////\r\n");
	printf("\r\n");
}


void menu_print_maxim_banner_big()
/**
* \brief       Print large Maxim banner at top of Hyperterminal screen
*
* \param       None
*
* \retval      None
*/
{
	printf( "///////////////////////////////////////////////////////////////////\r\n");
	printf( "///////////////////////////////////////////////////////////////////\r\n");
	printf( "//                                                               //\r\n");
	printf( "//         ##     ##     ##     #  #  #######  ##     ##         //\r\n");
	printf( "//         # #   # #    #  #     ##      #     # #   # #         //\r\n");
	printf( "//         #  # #  #   ######    ##      #     #  # #  #         //\r\n");
	printf( "//         #   #   #  #      #  #  #  #######  #   #   #         //\r\n");
	printf( "//                                                               //\r\n");
	printf( "//           I N T E G R A T E D    P R O D U C T S              //\r\n");
	printf( "//                     INNOVATION DELIVERED                      //\r\n");
	printf( "//                                                               //\r\n");
	printf( "///////////////////////////////////////////////////////////////////\r\n");
	printf( "///////////////////////////////////////////////////////////////////\r\n");

	printf("\r\n\r\n");
}

void menu_print_prompt()
/**
* \brief       Print a standard prompt for keyboard input "  > "
*
* \param       None
*
* \retval      None
*/
{
	printf("\r\n>> ");
	fflush(stdout);
}


void menu_print_line()
/**
* \brief       Print one line of dashes across the screen via Hyperterminal
*
* \param       None
*
* \retval      None
*/
{
	printf("----------------------------------------------------\r\n\r\n");
}
