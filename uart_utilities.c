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

#include <stdio.h>
#include "xbasic_types.h"
#include "xparameters.h"
#include "delays.h"
#include "print_utilities.h"
#include "max31723.h"
#include "max31723_utilities.h"
#include "uart_utilities.h"
#include "math.h"


void sendUartByte(u32 unUartAddress, u8 uchByte)
/**
* \brief       Send a byte to the UART.  //csEither directs to the full UART in the Zynq PS, or a Xilinx UartLite
*
* \param[in]   unUartAddress.  32 bit UART address
* \param[in]   uchByte.  The byte to be sent to the UART
*
* \retval      None
*/
{
	//csif(unUartAddress==XPAR_AXI_QUAD_SPI_0_BASEADDR)
	//cs{
		printf("%c",uchByte);
		fflush(stdout);
	//cs}
	//cs else
	//cs	XUartLite_SendByte(unUartAddress, uchByte);
}

// ----------------------------------------------------------------------------//
u8 checkUartEmpty(u32 unUartAddress)
/**
* \brief       Check if either Uart is empty.
*
* \param[in]   unUartAddress.  32 bit UART address
*
* \retval      True if empty (false if not)
*/
{
	u8 uchReturnVal=TRUE;
	u32 unUartStatusRegisterAddress;
	u32 unStatusRegisterValue;
	//cs if(unUartAddress==XPAR_PS7_UART_1_BASEADDR)
	//cs {
		unUartStatusRegisterAddress = unUartAddress + 0x0000002C;
		unStatusRegisterValue = Xil_In32(unUartStatusRegisterAddress);

		// Register UART BASE ADDR + 0x2C.  Bit 1 is a (1) when Rx FIFO is empty
		if((unStatusRegisterValue & 0x00000002)==0x00000002)
			uchReturnVal=TRUE;
		else
			uchReturnVal=FALSE;
	//cs}
	//cselse
	//cs{
		//csif(XUartLite_IsReceiveEmpty(unUartAddress)==TRUE)
			//csuchReturnVal=TRUE;
		//cselse
			//csuchReturnVal=FALSE;
	//cs}

	return(uchReturnVal);

}

// ----------------------------------------------------------------------------//
u8 getUartByte(u32 nUartAddress)
/**
* \brief       Get a byte from either the full UART in the Zynq PS,
*
* \param[in]  unUartAddress.  32 bit UART address
*
* \retval      Returns the next byte from the Uart Rx FIFO
*/
{
	u8 uchInput;

	// Check if the UART base address is the full UART located within the Zynq chip,
	// if not, assume it is an HDL instantiated AXI-UartLite
	//cs if(nUartAddress==XPAR_PS7_UART_1_BASEADDR)
		//csread(1, (char*)&uchInput, 1);	// Get a byte from stdin
		uchInput=Xil_In8(nUartAddress);
	//cs else
	//cs	uchInput = XUartLite_RecvByte(nUartAddress);
	return(uchInput);
}

// ----------------------------------------------------------------------------//
int receive_byte_with_timeout(u32 unUartAddress, int nTimeoutInTenthsOfSeconds, u8 *uchRxData)
/**
* \brief       Receive a byte from the UART located at *pUartAddress
*
* \param[in]   unUartAddress                - address of the UART peripheral in MicroBlaze memory map @help
* \param[in]   nTimeoutInTenthsOfSeconds    - amount of time to allow before TIMEOUT
* \param[out]  *uchRxData                   - received data is stored at uchRxData
*
* \retval      TRUE if operation succeeded
*/
{
	int j=0;
	int nReturnVal = TRUE;
	u8 uchInput = 0;

	// Check if there is a character in the UART Rx buffer
	// Continue checking every 10th of a second for nTimeoutInSeconds
	while(checkUartEmpty(unUartAddress) && (j < nTimeoutInTenthsOfSeconds))
	{
		j++;
		delay(ABOUT_ONE_SECOND/10);
	}

	if(checkUartEmpty(unUartAddress))
			nReturnVal = FALSE;
	else
	{
		uchInput = getUartByte(unUartAddress);
		// Check if it is an escape sequence
		if(uchInput==27)  // Escape sequence (likely an arrow key)
		{
			if(checkUartEmpty(unUartAddress))
				nReturnVal = FALSE;
			else
			{
				uchInput = getUartByte(unUartAddress);
				if(uchInput==91)  // Left bracket (part #2 of the 3 part escape sequence)
				{
					if(checkUartEmpty(unUartAddress))
						nReturnVal = FALSE;
					else
					{
						uchInput = getUartByte(unUartAddress);
						if(uchInput==75)
							uchInput = 244;  // We have defined KEYPRESS_END as 244
					}
				}
			}

		}
		*uchRxData = uchInput;
		nReturnVal = TRUE;
	}
	return(nReturnVal);
}

int GetLine( char* sInputString, unsigned int unMaxSize )
/**
* \brief       Retrieve a line of characters from the default Hyperterminal UART (DEFAULT_HYPERTERMINAL_UART).
* \par         Details
*              Maximum number of characters can be specified.  Function will timeout after 10 seconds.
*
* \param[in]   sInputString       - pointer to buffer for input string
* \param[in]   unMaxSize          - maximum number of characters to input
*
* \retval      TRUE if operation succeeded
*/
{
	u8 uchInputChar = 0;
	unsigned int unInputStringIndex = 0;

	receive_byte_with_timeout(XPAR_PS7_UART_1_BASEADDR, 10, &uchInputChar);
	while( (uchInputChar != '\r') && (uchInputChar != '\n') && (unInputStringIndex < unMaxSize-1) )
	{
		sInputString[ unInputStringIndex ] = (char)uchInputChar;
		unInputStringIndex++;
		receive_byte_with_timeout(XPAR_PS7_UART_1_BASEADDR, 10, &uchInputChar);
	}
	sInputString[ unInputStringIndex ] = '\0';

	return (unInputStringIndex < unMaxSize) ? unInputStringIndex : -1;
}

unsigned int menu_get_direct_entry(u32 nUartAddress, int nNumberBits)
/**
* \brief       Retrieve keyboard entry of a value via the Hyperterminal connected UART
* \par         Details
*              In most cases, this function is used to directly populate a register with a value.
*              int nNumberBits equals the number of bits in the register.  The value input by the user
*              is capped to the maximum allowable for the given number of bits (e.g. - 6 bits => '64' max)
*
* \param[in]   nUartAddress      - address of the UART peripheral in MicroBlaze memory map
* \param[in]   nNumberBits       - number of bits in register populate
* \param[in]   nOffset           - @help
*
* \retval      Value entered
*/
{
	int nTemp;
	int i;
	u8 uchInput=0;
	int nNumberCharactersReceived=0;
	u8 auchInput[8];
	unsigned int nReturnVal=0;

	menu_cls();
	nTemp = number_raised_to_power(2,nNumberBits) - 1;
	printf("Enter a value and then press enter (value = %d..%d)\r\n",0, nTemp);
	menu_print_prompt();
	fflush(stdout);

	while(uchInput!=13)
	{
		uchInput = getUartByte(nUartAddress);
		if(uchInput>=48 && uchInput <=57) // digits 0..9
		{
			uchInput -=48; // subtract 48 to get the real number converted from the ASCII digit
			auchInput[nNumberCharactersReceived] = uchInput;
			nNumberCharactersReceived++;
			printf("%d",uchInput);
			fflush(stdout);
		}
		else if(uchInput==13) // carriage return
		{
			for(i=0;i<nNumberCharactersReceived;i++)
			{
				nReturnVal += auchInput[i] * number_raised_to_power(10,nNumberCharactersReceived-i-1);
			}
			if(nReturnVal > nTemp)
				nReturnVal=nTemp;
		}
	}
	printf("\r\nValue is %d\r\n",nReturnVal);
	delay(ABOUT_ONE_SECOND * 2);
	return(nReturnVal);
}

u8 menu_retrieve_keypress(u32 nUartAddress)
/**
* \brief       Get a single keypress via Hyperterminal.
* \par         Details
*              Returns ascii character corresponding to keypress with some preprocessing.
* \n           Escape sequences (Arrow keys and END) are mapped to decimal 240-244 (see defines)
* \n           Characters "0"-"9" converted to numbers 0-9
* \n           Lower case "a"-"z" converted to uppercase "A"-"Z"
*
* \param[in]   nUartAddress        - address of the UART peripheral in MicroBlaze memory map
*
* \retval      Character, partially decoded.
*/
{
	u8 uchInput;

	uchInput = getUartByte(nUartAddress);

	if(uchInput==27)  // Escape sequence (likely an arrow key)
	{
		uchInput = getUartByte(nUartAddress);
		if(uchInput==91)  // Left bracket (part #2 of the 3 part escape sequence)
		{
			uchInput = getUartByte(nUartAddress);
			if(uchInput==65)
				uchInput = KEYPRESS_ARROW_UP-10;
			if(uchInput==66)
				uchInput = KEYPRESS_ARROW_DOWN-10;
			if(uchInput==67)
				uchInput = KEYPRESS_ARROW_RIGHT-10;
			if(uchInput==68)
				uchInput = KEYPRESS_ARROW_LEFT-10;
			if(uchInput==75)
				uchInput = KEYPRESS_END - 10;
		}
	}
	else if(uchInput>=48 && uchInput <=57) // digits 0..9
		uchInput -=48; // subtract 48 to get the real number converted from the ASCII digit
	else if(uchInput>=97 && uchInput<=122)
		uchInput -=32;  // convert lowercase to uppercase

	return(uchInput);
}


