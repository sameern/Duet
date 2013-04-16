/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
Code from wiring-digital.c and from variant.cpp from the arduino software
This allows access to the pins on the SAM3X8E that are not defined in the Arduino
pin description.

At this point it only implements pinMode and digitalWrite on pin PA5 and PC27
(also on PA0,PA1,PA7 as a further example, ahtough these are defined by the Arduino software)
Note the pin numbers of "0" and "1"
*/

#include "UdefDuePin.h"

//Example from the variant.cpp file
/*
 * DUET "undefined" pin  |  PORT  | Label
 * ----------------------+--------+-------
 *  0                    |  PA5   | "E0_EN"
 *  1                    |  PC27  | "Z_EN"


 */

/*
 * Pins descriptions
 */
extern const PinDescription unDefPinDescription[]=
{
  { PIOA, PIO_PA5,         ID_PIOA, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN X0
  { PIOC, PIO_PC27,        ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN X1
  { PIOA, PIO_PA0,         ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN X2
  { PIOA, PIO_PA1,         ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN X3
  { PIOA, PIO_PA7,         ID_PIOC, PIO_OUTPUT_0, PIO_DEFAULT, PIN_ATTR_DIGITAL,                  NO_ADC, NO_ADC, NOT_ON_PWM,  NOT_ON_TIMER }, // PIN X4
  { NULL, 0, 0, PIO_NOT_A_PIN, PIO_DEFAULT, 0, NO_ADC, NO_ADC, NOT_ON_PWM, NOT_ON_TIMER }
} ;


/*
pinModeUndefined
copied from the pinMode function within wiring-digital.c file, part of the arduino core.
Allows a non "Arduino Due" PIO pin to be setup.
*/
extern void pinModeUndefined( uint32_t ulPin, uint32_t ulMode )
{
    if ( unDefPinDescription[ulPin].ulPinType == PIO_NOT_A_PIN )
    {
        return ;
    }

    switch ( ulMode )
    {
        case INPUT:
            /* Enable peripheral for clocking input */
            pmc_enable_periph_clk( unDefPinDescription[ulPin].ulPeripheralId ) ;
            PIO_Configure(
            	unDefPinDescription[ulPin].pPort,
            	PIO_INPUT,
            	unDefPinDescription[ulPin].ulPin,
            	0 ) ;
        break ;

        case INPUT_PULLUP:
            /* Enable peripheral for clocking input */
            pmc_enable_periph_clk( unDefPinDescription[ulPin].ulPeripheralId ) ;
            PIO_Configure(
            	unDefPinDescription[ulPin].pPort,
            	PIO_INPUT,
            	unDefPinDescription[ulPin].ulPin,
            	PIO_PULLUP ) ;
        break ;

        case OUTPUT:
            PIO_Configure(
            	unDefPinDescription[ulPin].pPort,
            	PIO_OUTPUT_1,
            	unDefPinDescription[ulPin].ulPin,
            	unDefPinDescription[ulPin].ulPinConfiguration ) ;

            /* if all pins are output, disable PIO Controller clocking, reduce power consumption */
            if ( unDefPinDescription[ulPin].pPort->PIO_OSR == 0xffffffff )
            {
                pmc_disable_periph_clk( g_APinDescription[ulPin].ulPeripheralId ) ;
            }
        break ;

        default:
        break ;
    }
}

/*
digitalWriteUndefined
copied from the digitalWrite function within wiring-digital.c file, part of the arduino core.
Allows digital write to a non "Arduino Due" PIO pin that has been setup as output with pinModeUndefined
*/

extern void digitalWriteUndefined( uint32_t ulPin, uint32_t ulVal )
{
  /* Handle */
  if ( unDefPinDescription[ulPin].ulPinType == PIO_NOT_A_PIN ) 
 {
    return ;
  }

  if ( PIO_GetOutputDataStatus( unDefPinDescription[ulPin].pPort, unDefPinDescription[ulPin].ulPin ) == 0 )
  {
    PIO_PullUp( unDefPinDescription[ulPin].pPort, unDefPinDescription[ulPin].ulPin, ulVal ) ;
  }
  else
  {
    PIO_SetOutput( unDefPinDescription[ulPin].pPort, unDefPinDescription[ulPin].ulPin, ulVal, 0, PIO_PULLUP ) ;
  }
}
