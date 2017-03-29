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
#include "spi_utilities.h"
//#include "maximPMOD.h"

int SpiRW( u32 unPeripheralAddressSPI, unsigned int unCPHA, unsigned int unCPOL,
		u8* auchWriteBuf, u8* auchReadBuf, int unNumBytes, u8 uchCsActiveHigh )
/**
* \brief       Perform a SPI read or write.
* \par         Details
*              This function provides a combination SPI Read and Write to the chosen SPI port in the design
*              CPHA and CPOL can be set to 0 or 1
*              Pointers are provided to u8 buffers containing the data to be written and received
*              Data in the auchWriteBuf will be clocked out (MSB first) onto the MOSI pin
*              Data from the MISO pin will be placed into the auchReadBuf
*              uchCsActiveHigh==TRUE allows SS configurations to be used
*              uchCsActiveHigh==FALSE allows SS# configurations to be used
*
* \param[in]   unPeripheralAddressSPI         - @help
* \param[in]   unCPHA             - phase of SCK (edge to trigger on). 0=Leading edge, 1=Trailing edge
* \param[in]   unCPOL             - polarity of SCK. 0=Active high, 1=Active low  
* \param[in]   auchWriteBuf       - pointer to write data buffer
* \param[in]   auchReadBuf        - pointer to read data buffer
* \param[in]   unNumBytes         - number of bytes to transfer
* \param[in]   uchCsActiveHigh    - polarity of slave select 0=active low, 1=active high
*
* \retval      Always returns 0
*/
{
	int i;
	unsigned int unControlData = 0x00000186;

	//If CPHA or CPOL = 1, we need to set the corresponding bits in the control register
	unControlData = unControlData | (unCPHA << 4);
	unControlData = unControlData | (unCPOL << 3);

	//Write config data to SPICR.  We need the inhibit bit=1.
	XSpi_WriteReg(unPeripheralAddressSPI, 0x60, unControlData);

	//Deassert CS to 1 to ensure SPI slave is inactive
	if( uchCsActiveHigh )
		XSpi_WriteReg(unPeripheralAddressSPI, 0x70, 0xFFFFFFFE);
	else
		XSpi_WriteReg(unPeripheralAddressSPI, 0x70, 0xFFFFFFFF);

	for( i = 0; i < unNumBytes; i++)
	{
		if( auchWriteBuf != 0 )
		{
			//Write data to SPIDTR.  This is the data that will be transferred to the SPI slave.
			XSpi_WriteReg(unPeripheralAddressSPI, 0x68, auchWriteBuf[ i ]);
			//Debug//printf( "Write %02x; ", auchWriteBuf[i]);
		}
		else
		{
			//Write data to SPIDTR.  This is the data that will be transferred to the SPI slave.
			XSpi_WriteReg(unPeripheralAddressSPI, 0x68, 0x00);
		}

		//Write config data to SPICR.  We need the inhibit bit=1.
		XSpi_WriteReg(unPeripheralAddressSPI, 0x60, unControlData);

		//Assert CS for our PMOD part
		if( uchCsActiveHigh )
			XSpi_WriteReg(unPeripheralAddressSPI, 0x70, 0xFFFFFFFF);
		else
			XSpi_WriteReg(unPeripheralAddressSPI, 0x70, 0xFFFFFFFE);

		//Un-inhibit our SPI master to transfer the data
		XSpi_WriteReg(unPeripheralAddressSPI, 0x60, unControlData & 0xFFFFFEFF);

		//Wait for transaction to complete.  Check to see if Tx_Empty flag is set before proceeding.
		while( !(XSpi_ReadReg( unPeripheralAddressSPI, 0x64 ) & 0x00000004) )
			;

		//Inhibit SPI master to prevent further action
		XSpi_WriteReg(unPeripheralAddressSPI, 0x60, unControlData);

		//Read received data
		if( (auchReadBuf != 0) )
		{
			auchReadBuf[ i ] = XSpi_ReadReg(unPeripheralAddressSPI, 0x6C);
			//Debug//printf( "Read %02x\r\n", auchReadBuf[i]);
		}
	}
	if( uchCsActiveHigh )
		XSpi_WriteReg(unPeripheralAddressSPI, 0x70, 0xFFFFFFFE);
	else
		XSpi_WriteReg(unPeripheralAddressSPI, 0x70, 0xFFFFFFFF);
	return 0;
}










