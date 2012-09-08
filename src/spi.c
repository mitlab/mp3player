/*****************************************************************************/
/*	Copyright Sebastian Meuren 2003, 2008 
	
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */
/*****************************************************************************/
#include	<avr/io.h>
#include	<stdint.h>
#include	"spi.h"



inline uint8_t	spi_rw(uint8_t u8Value){
		/*	Lets start a data-transmission */
		SPDR0 =	u8Value;
		while((SPSR0 & _BV(SPIF0)) == 0);
		return	SPDR0;
}

void spi_lowspeed()
{
	SPCR0 |=  (_BV(SPR00) | _BV(SPR01));
	SPSR0 = 0;
}

void spi_highspeed()
{
	SPCR0 = _BV(SPE0) | _BV(MSTR0) ;
	SPSR0 = 0;
}

void spi_init(void)
{
//-----------------------------------------------------------------------------
/*	Setting up the SPI */
//-----------------------------------------------------------------------------
/*	ATTENTION: We have to configure the SS pin as an output before!!!
	we configure the SPI!! Otherwise the SS pin will work as a Slave
	Select!!! */
	PORTB 	|= _BV(4);
	DDRB	|= _BV(4);
/*	Configure the SCLK-Pin as (LOW) Output */
	DDRB	|= _BV(7);
/*	Configure the MOSI-Pin as (LOW) Output */
	DDRB	|= _BV(5);
/*	The vs1001 needs the following configuration:
	We use the clock-doubler of the chip. This means we have
	to start with a SPI-Frequency below 2 Mhz (see Appnote)
	We set	SPR0 = SPR1 = 1, SPI2X=0 which
	equals 1/128 of the system frequency 
	SPIE = 0: No Interrupts used so far
	SPE	= 1: SPI enabled
	DORD = 0: MSB first 
	MSTR = 1: Master Operation
	CPOL = 0: SCK low when idle
	CPHA = 0: Sample on the leading edge
	(SPI2X = 0): Choose fosc/4 as the SCK-frequency
	SPR1 = 1
	SPR0 = 1 */
	SPCR0	= 	_BV(SPE0) | _BV(MSTR0) | _BV(SPR01);
/*	Set SPI2X = 0!! (diffrent register!)  */
	SPSR0	= 0;
}
