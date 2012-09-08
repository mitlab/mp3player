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
/* VS10xx */
/*****************************************************************************/
#ifndef	VS10xx__
#define	VS10xx__
#include <stdint.h>

/*****************************************************************************/
/*	VS10xx: Macros */
/*****************************************************************************/
//#define VS10xx_CONFIGURE_XRESET	DDRB |= _BV(4);    PORTB &= ~_BV(4);
#define VS10xx_CONFIGURE_XDCS		DDRB |=  (1<<PB2);  PORTB |=  (1<<PB2);
#define VS10xx_CONFIGURE_XCS		DDRB |=  (1<<PB3);  PORTB |=  (1<<PB3);
#define	VS10xx_CONFIGURE_DREQ		DDRB &= ~(1<<PB1);  PORTB &= ~(1<<PB1);
/* Changing Pins */
#define	VS10xx_DREQ_STATUS		(PINB & (1<<PB1))
//#define	VS10xx_SELECT_XRESET	PORTB |= _BV(4);
//#define	VS10xx_DESELECT_XRESET	PORTB &= ~_BV(4);
#define	VS10xx_SELECT_XDCS		PORTB |= (1<<PB2);
#define	VS10xx_DESELECT_XDCS	PORTB &= ~(1<<PB2);
#define	VS10xx_SELECT_XCS		PORTB |= (1<<PB3);
#define	VS10xx_DESELECT_XCS		PORTB &= ~(1<<PB3);

/*****************************************************************************/
/* API */
/*****************************************************************************/
void		vs10xx_sinetest(void);
uint8_t		vs10xx_init(void);
void		vs10xx_reset(void);
void		vs10xx_cntr_wr(uint8_t u8Address, uint16_t u16Value);
uint16_t	vs10xx_cntr_rd(uint8_t u8Address);

/*****************************************************************************/
/*	This is the inital volume of the vs10xx */
#define	VS10xx_INITVOLUME	0x30

#endif
