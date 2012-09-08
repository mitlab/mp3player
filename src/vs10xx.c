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
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include "spi.h"
#include "vs10xx.h"

/*****************************************************************************/
/*	VS10xx: Read a SCI-Register */
/*****************************************************************************/
uint16_t	vs10xx_cntr_rd(uint8_t u8Address)
{
	uint16_t		u16Ret	= 0;
	
	//spi_lowspeed();
		
	VS10xx_DESELECT_XCS
	
	spi_rw(3); // READ command
	spi_rw(u8Address);
	/*	Read the Register in */
	u16Ret =	((uint16_t)spi_rw(0)) << 8;
	u16Ret |=	((uint16_t)spi_rw(0));

	VS10xx_SELECT_XCS

	/*	Wait for DREQ to go high */
	while(VS10xx_DREQ_STATUS == 0);
	return u16Ret;
}

/*****************************************************************************/
/*	VS10xx: Write a SCI-Register */
/*****************************************************************************/
void	vs10xx_cntr_wr(uint8_t u8Address,uint16_t u16Value)
{
	//spi_lowspeed();
			
		/*	Make XCS low */
		VS10xx_DESELECT_XCS
		
		/*	Write the control-word: READ */
		spi_rw(2);
		/*	Write the Number of the register out */
		spi_rw(u8Address);
		/*	Read the Register in */
		spi_rw(u16Value>>8);
		spi_rw(u16Value);
		/* XCS high again */
		VS10xx_SELECT_XCS

		/*	Wait for DREQ to go high */
		while(VS10xx_DREQ_STATUS == 0);
}
/*****************************************************************************/
/*	VS10xx: Reset a SCI-Register */
/*****************************************************************************/
void	vs10xx_reset(void)
{
	uint16_t	u16reg;
	
	//	Set the Software-Reset Flag in the Mode-register 
	vs10xx_cntr_wr(0,0x0004);
	//	Wait at least 2 us 
	_delay_us(40);
	//	Now wait for DREQ to go high 
	while(VS10xx_DREQ_STATUS == 0);

	
	//enable clock doubler
	// 12 MHz crzstal
	vs10xx_cntr_wr(3,0x9800 | 0x3E8);
	u16reg	= vs10xx_cntr_rd(3);
	_delay_ms(10);
	while(VS10xx_DREQ_STATUS == 0);

	//vs10xx_cntr_wr(13,0x5555);
	//u16reg	= vs10xx_cntr_rd(13);
		
	
//		vs10xx_cntr_wr(2,0xf3fa);
//		u16reg	= vs10xx_cntr_rd(2);
	

	/*	Setupt the Interface:
		We dont share the XCS-Pin and we use the VS1002-Native Mode for
		Datatransfer 
		Bit.2:	SoftwareReset 0
		Bit.5:	Allow SDI-Tests 0
		Bit.10: SDI-Share 0
		Bit.11:	Native SPI-Mode 1
		*/
	vs10xx_cntr_wr(0,0x0800);

	/*	Write s.th. to the AUDATA-Register */
	//vs10xx_cntr_wr(5,0x1f40);		//0x1f40 for 8000 Hz (from Appnote)
	/*	wait for at least 11000 clock cycles (Appnote):
		@12.288 Mhz we have to wait ~ 1ms*/
	_delay_ms(2);  //!!!!!!!!!!!!!!!!!!!!
	while(VS10xx_DREQ_STATUS == 0);		//Wait for DREQ to go high

	/*	Now we can use an SPI-speed up to 4 Mhz */


	/*	Setting Up the initial Audio-Volume */
	vs10xx_cntr_wr(11,VS10xx_INITVOLUME | (VS10xx_INITVOLUME << 8));
	while(VS10xx_DREQ_STATUS == 0);		//Wait for DREQ to go high
}

uint8_t vs10xx_init(void)
{
	VS10xx_CONFIGURE_XDCS
	VS10xx_CONFIGURE_XCS
	VS10xx_CONFIGURE_DREQ

	vs10xx_reset();

	return	0;
}
/*****************************************************************************/

/*****************************************************************************/
/*	VS10xx: Sine-Test */
/*****************************************************************************/
void vs10xx_sinetest()
{
	uint16_t u16reg;

	/*	Read the current value of the SCI_MODE-Register */
	u16reg = vs10xx_cntr_rd(0);

	/*	Set Bit.5: Allow SDI-tests */
	u16reg	|=	_BV(5);
	vs10xx_cntr_wr(0, u16reg);
	while(VS10xx_DREQ_STATUS == 0);		//Wait for DREQ to go high
		
	/*	Start SINE-Test */
	VS10xx_DESELECT_XDCS
	spi_rw(0x53);
	spi_rw(0xEF);
	spi_rw(0x6E);
	spi_rw(252);
	spi_rw(0x00);
	spi_rw(0x00);
	spi_rw(0x00);
	spi_rw(0x00);
	VS10xx_SELECT_XDCS
	
	_delay_ms(1000);
	
	/*	Stop it again */
	VS10xx_DESELECT_XDCS
	spi_rw(0x45);
	spi_rw(0x78);
	spi_rw(0x69);
	spi_rw(0x74);
	spi_rw(0x00);
	spi_rw(0x00);
	spi_rw(0x00);
	spi_rw(0x00);
	VS10xx_SELECT_XDCS

	//	Disable SDI-Tests again 
	u16reg	&=	~_BV(5);
	vs10xx_cntr_wr(0, u16reg);
	while(VS10xx_DREQ_STATUS == 0);		//Wait for DREQ to go high
}
