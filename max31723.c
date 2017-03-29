/** \file menu.c ***********************************************************
 * 
 *             Project: Maxim Plug-in Peripheral Modules
 *            Filename: menu.c
 *         Description: This module contains all the functions used to
 *                      generate the menus and menu options used to run the
 *                      various demos and examples for the individual modules.
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

#include <stdio.h>
#include "xgpio.h"
#include "xgpio_l.h"
#include "xparameters.h"
//#include "xspi_l.h"
#include "xspi.h"
#include "uart_utilities.h"
#include "print_utilities.h"
#include "spi_utilities.h"
#include "string.h"
#include "max31723_utilities.h"
#include "max31723.h"
#include "led_utilities.h"
//#include "oled_utilities.h"
#include "delays.h"

int main()

{
	int nMenuState = 0;
	u8 uchInput=0;
	int menuActive=TRUE;
	float fTemp=0.0f;
	int displayCelsius=TRUE;
	int i;
	float fHighAlarmSetting=0.0f;
	float fLowAlarmSetting=0.0f;
	int Status;
	//float temperature;
	//float previous_temperature;


	// ------- SPI related declarations
	XSpi_Config *SPI_ConfigPtr;
	static XSpi SpiInstance;	 /* The instance of the SPI device */

	//cs Set the active port type to SPI
	//cs max_set_PMOD_port(g_nActivePMODPort, PMOD_PORT_TYPE_SPI);

	// ------------------- SPI related functions --------------------- //
	// Initialize the SPI driver
	SPI_ConfigPtr = XSpi_LookupConfig(XPAR_AXI_QUAD_SPI_0_DEVICE_ID);
	if (SPI_ConfigPtr == NULL) return XST_DEVICE_NOT_FOUND;

	Status = XSpi_CfgInitialize(&SpiInstance, SPI_ConfigPtr, SPI_ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) return XST_FAILURE;

	// Reset the SPI peripheral
	XSpi_Reset(&SpiInstance);

	// Perform a self-test to ensure that the hardware was built correctly.
	Status = XSpi_SelfTest(&SpiInstance);
	if (Status != XST_SUCCESS) return XST_FAILURE;

	// Run loopback test only in case of standard SPI mode.
	if (SpiInstance.SpiMode != XSP_STANDARD_MODE) return XST_SUCCESS;
	
	// --------------------------------------------------------------------------//
	// Main Loop 
	// --------------------------------------------------------------------------//

	while(menuActive==TRUE)
	{
		switch(nMenuState)
		{
			case 0:
				menu_cls();
				printf("MAX31723 Digital Thermometer:\r\n\r\n");
				//max_MAX31723_get_temp(&fLowAlarmSetting, g_unActivePeripheralAddressSPI, MAX31723_LOW_ALARM_READ);
				max_MAX31723_get_temp(&fLowAlarmSetting, XPAR_AXI_QUAD_SPI_0_BASEADDR, MAX31723_LOW_ALARM_READ);
				//max_MAX31723_get_temp(&fHighAlarmSetting, g_unActivePeripheralAddressSPI, MAX31723_HIGH_ALARM_READ);
				max_MAX31723_get_temp(&fHighAlarmSetting, XPAR_AXI_QUAD_SPI_0_BASEADDR, MAX31723_HIGH_ALARM_READ);
				printf("Last temp reading = ");
				printf_temp(fTemp,displayCelsius,TRUE);
				printf("Low Alarm Setpoint = ");
				printf_temp(fLowAlarmSetting,displayCelsius,TRUE);
				printf("High Alarm Setpoint = ");
				printf_temp(fHighAlarmSetting,displayCelsius,TRUE);
				menu_print_line();

				printf("1.  Retrieve ( 1)    temp reading\r\n");
				printf("2.  Retrieve (20)    temp reading(s)\r\n");
				printf("3.  Retrieve temp indefinitely\r\n");
				printf("4.  Directly enter low alarm setpoint\r\n");
				printf("5.  Directly enter high alarm setpoint\r\n");

				printf("6.  Toggle display degC / degF\r\n");
				printf("\r\n9.  Return to main menu\r\n");
				menu_print_prompt();
				nMenuState = 1;
				break;
			case 1:
				//uchInput = menu_retrieve_keypress(DEFAULT_HYPERTERMINAL_UART_ADDRESS);
				uchInput = menu_retrieve_keypress(XPAR_XUARTPS_0_BASEADDR);
				nMenuState = uchInput+10;
				break;

			case 11:
				max_MAX31723_get_temp(&fTemp, XPAR_AXI_QUAD_SPI_0_BASEADDR,MAX31723_TEMP_READ);
				//print_seven_segment_temperature(fTemp,displayCelsius);
				//printOLED_31723(fTemp);
				nMenuState = 0;
				break;
			case 12:
				printf("\r\n");
				fflush(stdout);
				for(i=0;i<20;i++)
				{
					max_MAX31723_get_temp(&fTemp, XPAR_AXI_QUAD_SPI_0_BASEADDR,MAX31723_TEMP_READ);
					printf("%d of 20 samples = ",i+1);
					printf_temp(fTemp,displayCelsius,TRUE);
					//print_seven_segment_temperature(fTemp,displayCelsius);
					//printOLED_31723(fTemp);
					delay(ABOUT_ONE_SECOND / 2);
				}
				nMenuState=0;
				break;
			case 13:
				printf("Hit any key to exit:\r\n");
				menu_print_line();
				fflush(stdout);
				while(checkUartEmpty(XPAR_XUARTPS_0_BASEADDR))
				{
					max_MAX31723_get_temp(&fTemp, XPAR_AXI_QUAD_SPI_0_BASEADDR,MAX31723_TEMP_READ);
					printf_temp(fTemp,displayCelsius,TRUE);
					//print_seven_segment_temperature(fTemp,displayCelsius);
					//printOLED_31723(fTemp);
					delay(ABOUT_ONE_SECOND/2);
				}
				menu_retrieve_keypress(XPAR_XUARTPS_0_BASEADDR);
				nMenuState=0;
				break;
			case 14:
				fLowAlarmSetting = (float)menu_get_direct_entry(XPAR_XUARTPS_0_BASEADDR,7);
				max_MAX31723_set_alarm(fLowAlarmSetting,XPAR_AXI_QUAD_SPI_0_BASEADDR,MAX31723_LOW_ALARM_WRITE);
				nMenuState=0;
				break;
			case 15:
				fHighAlarmSetting = (float)menu_get_direct_entry(XPAR_XUARTPS_0_BASEADDR,7);
				max_MAX31723_set_alarm(fHighAlarmSetting,XPAR_AXI_QUAD_SPI_0_BASEADDR,MAX31723_HIGH_ALARM_WRITE);
				nMenuState=0;
				break;
			case 16:
				if(displayCelsius==TRUE)
					displayCelsius=FALSE;
				else
					displayCelsius=TRUE;
				//print_seven_segment_temperature(fTemp,displayCelsius);
				//printOLED_31723(fTemp);

				nMenuState = 0;
				break;
			case 19:
				menuActive=FALSE;
				break;
			default:
				nMenuState = 0;
				break;

		}
	}
}

