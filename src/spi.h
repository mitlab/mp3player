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
#ifndef __SPI
#define __SPI
#include <avr/io.h>
#include <stdint.h>

inline uint8_t		spi_rw(uint8_t u8Value);
void	spi_init(void);
void spi_lowspeed(void);
void spi_highspeed(void);

#define	SPCR0	SPCR
#define	SPE0	SPE
#define	MSTR0	MSTR
#define	SPDR0	SPDR
#define	SPIF0	SPIF
#define	SPR00	SPR0
#define	SPR01	SPR1
#define	SPCR0	SPCR
#define	SPSR0	SPSR
#define	TCCR0	TCCR

#define	SPI_SELECT_MOSI 	PORTB	|= _BV(5);
#define	SPI_DESELECT_MOSI 	PORTB	&= ~_BV(5);
#define SPI_DESELECT_CLK	PORTB 	&= ~_BV(7);
#define SPI_SELECT_CLK		PORTB 	|= _BV(7);

#endif
