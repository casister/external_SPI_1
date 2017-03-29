/** \file menu.h ***********************************************************
 * 
 *             Project: Maxim Plug-in Peripheral Modules
 *            Filename: menu.h
 *         Description: This module contains all the functions used to
 *                      generate the menus and menu options used to run the
 *                      various demos and examples for the individual modules.
 * 
 *    Revision History:
 *\n      7-20-12 Rev 1.4 Nathan Young    Initial release
 *\n       9-04-12 Rev 1.5 Nathan Young    Multiple PMOD port support
 *\n       10-15-12 Rev 1.6 Nathan Young	OLED display support
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

#ifndef MENU_H_
#define MENU_H_

#include "xbasic_types.h"
#include "xspi_l.h"
#include "stdio.h"
//#include "maximPMOD.h"

#define NUMBER_SUPPORTED_PERIPHERAL_MODULES 15  //!< The total number of PMODs that have menus

#define MAIN_MENU 0                 //!< // Menu state machine state
#define WAIT_KEYPRESS 1             //!< // Menu state machine state
#define BASE_FUNCTION_STATE 10      //!< // Menu state machine state

#define MENU_MAX31723 66            //!< MAX31723 menu selected by pressing "B"
#define MENU_SET_ACTIVE_PMOD 69		//!< Change active PMOD Port by pressing "E"

#define KEYPRESS_ARROW_UP 240       //!< Assign up-arrow an extended ascii code which won't be used elsewhere
#define KEYPRESS_ARROW_DOWN 241     //!< Assign up-arrow an extended ascii code which won't be used elsewhere
#define KEYPRESS_ARROW_LEFT 242     //!< Assign up-arrow an extended ascii code which won't be used elsewhere
#define KEYPRESS_ARROW_RIGHT 243    //!< Assign up-arrow an extended ascii code which won't be used elsewhere
#define KEYPRESS_END 244            //!< Assign up-arrow an extended ascii code which won't be used elsewhere

// Function Prototypes

void menu_MAX31723();

#endif /* MENU_H_ */
