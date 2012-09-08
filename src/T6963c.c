/*
	T6963c.c - Function-implementations

	Simple LowLevel T6963c Library without Drawing functions
	for Rect's, Circles, etc...
	The files should be self-explanatory.

	(c) Simon Kueppers. If you find a bug or if you have a 
	question, feel free to Email me:
	simon.kueppers@stlcd.de
*/

#include "T6963c.h"
#include <util/delay.h>

//Set to 1 if you want to speedup the library
//Set to 0 if you want to get small code-size
#define T6963C_USE_INLINE 				1

#define nop() asm volatile ("nop")

//Macros for Bitbanging
#define T6963C_RESET_ENABLE 			nop() //T6963C_PORT_CTRL &= ~(1<<T6963C_RST)
#define T6963C_RESET_DISABLE 			nop() //T6963C_PORT_CTRL |= (1<<T6963C_RST)
#define T6963C_WR_ENABLE				T6963C_PORT_CTRL &= ~(1<<T6963C_WR)
#define T6963C_WR_DISABLE				T6963C_PORT_CTRL |= (1<<T6963C_WR)
#define T6963C_RD_ENABLE				T6963C_PORT_CTRL &= ~(1<<T6963C_RD)
#define T6963C_RD_DISABLE				T6963C_PORT_CTRL |= (1<<T6963C_RD)
#define T6963C_CE_ENABLE				T6963C_PORT_CTRL &= ~(1<<T6963C_CE)
#define T6963C_CE_DISABLE				T6963C_PORT_CTRL |= (1<<T6963C_CE)
#define T6963C_DATA_ENABLE				T6963C_PORT_CTRL &= ~(1<<T6963C_CD)
#define T6963C_CMD_ENABLE				T6963C_PORT_CTRL |= (1<<T6963C_CD)

//T6963c Commands
#define T6963C_CMD_SETTEXTHOME			0x40
#define T6963C_CMD_SETTEXTAREA			0x41
#define T6963C_CMD_SETGRAPHICHOME		0x42
#define T6963C_CMD_SETGRAPHICAREA		0x43
#define T6963C_CMD_SETADDRESS			0x24
#define T6963C_CMD_SETOFFSET			0x22
#define T6963C_CMD_SETCURSOR			0x21
#define T6963C_CMD_WRITE_INC			0xC0
#define T6963C_CMD_READ_INC				0xC1
#define T6963C_CMD_WRITE_DEC			0xC2
#define T6963C_CMD_READ_DEC				0xC3
#define T6963C_CMD_WRITE				0xC4
#define T6963C_CMD_READ					0xC5
#define T6963C_CMD_AUTOWRITE			0xB0
#define T6963C_CMD_AUTOREAD				0xB1
#define T6963C_CMD_AUTORESET			0xB2
#define T6963C_CMD_MODESET				0x80
#define T6963C_CMD_DISPMODE				0x90
#define T6963C_CMD_SETPIXEL				0xF8
#define T6963C_CMD_RESETPIXEL			0xF0
#define T6963C_CMD_CURSORPATTERN		0xA0

//Status Bits
#define T6963C_STA0						0x01
#define T6963C_STA1						0x02
#define T6963C_STA2						0x04
#define T6963C_STA3						0x08
#define T6963C_STA4						0x10
#define T6963C_STA5						0x20
#define T6963C_STA6						0x40
#define T6963C_STA7						0x80

//Mode Set Bits
//Already defined in T6963c.h for using with T6963cInit()

//Display Mode Bits
//Also already defined in T6963c.h for using with T6963cInit()


//Read data from T6963c
#if T6963C_USE_INLINE 
static inline  __attribute__ ((always_inline))
#endif
uint8_t T6963cGetData()
{
	//To be implemented. No use to implement this function, yet.
	return 0;
}

//Status Check
#if T6963C_USE_INLINE 
static inline  __attribute__ ((always_inline))
#endif
uint8_t T6963cGetStatus()
{
	uint8_t Status;

	//Databus as input. RD LOW, CD HIGH
	T6963C_DDR_DATA = 0x00;
	T6963C_PORT_DATA = 0x00;
	T6963C_CMD_ENABLE;
	T6963C_RD_ENABLE;
	
	nop();
	
	T6963C_CE_ENABLE;
	//tACC = 150ns
	nop();
	nop();
	nop();
	Status = T6963C_PIN_DATA;
	
	T6963C_CE_DISABLE;
	T6963C_RD_DISABLE;

	return Status;
}
	
//Write data to T6963c
#if T6963C_USE_INLINE 
static inline  __attribute__ ((always_inline))
#endif
void T6963cPutData(uint8_t Data)
{
	while ((T6963cGetStatus() & (T6963C_STA0 | T6963C_STA1)) != (T6963C_STA0 | T6963C_STA1))
		nop();

	//Databus as output
	T6963C_DDR_DATA = 0xFF;
	T6963C_PORT_DATA = Data;
	T6963C_DATA_ENABLE;
	T6963C_WR_ENABLE;

	nop();
		
	T6963C_CE_ENABLE;
	//tCE = 80ns
	nop();
	nop();
	T6963C_CE_DISABLE;
	//tDH = 40ns
	nop();

	T6963C_WR_DISABLE;
}

//Write Command to T6963c
#if T6963C_USE_INLINE 
static inline  __attribute__ ((always_inline))
#endif
void T6963cPutCmd(uint8_t Cmd)
{
	while ((T6963cGetStatus() & (T6963C_STA0 | T6963C_STA1)) != (T6963C_STA0 | T6963C_STA1))
		nop();

	//Databus as output
	T6963C_DDR_DATA = 0xFF;
	T6963C_PORT_DATA = Cmd;
	T6963C_CMD_ENABLE;
	T6963C_WR_ENABLE;

	nop();

	T6963C_CE_ENABLE;
	//tCE = 80ns
	nop();
	nop();
	T6963C_CE_DISABLE;
	//tDH = 40ns
	nop();

	T6963C_WR_DISABLE;
}

//Write Data in AutoWrite-Mode to T6963c
#if T6963C_USE_INLINE 
static inline  __attribute__ ((always_inline))
#endif
void T6963cPutDataAW(uint8_t Data)
{


	//Databus as output
	T6963C_DDR_DATA = 0xFF;
	T6963C_PORT_DATA = Data;
	T6963C_DATA_ENABLE;
	T6963C_WR_ENABLE;

	nop();
		
	T6963C_CE_ENABLE;
	//tCE = 80ns
	nop();
	nop();

	T6963C_CE_DISABLE;
	//tDH = 40ns
	nop();

	T6963C_WR_DISABLE;

	while ((T6963cGetStatus() & T6963C_STA3) != T6963C_STA3)
		nop();
	
}

void T6963cInit(uint8_t Options, uint8_t Mode)
{
	//Setup Pins
	T6963C_PORT_DATA = 0x00;
	T6963C_DDR_DATA = 0x00;

	T6963C_PORT_CTRL |= (1<<T6963C_WR) | (1<<T6963C_RD) | (1<<T6963C_CE);
	//T6963C_PORT_CTRL &= ~((1<<T6963C_RST) | (1<<T6963C_CD));
	T6963C_PORT_CTRL &= ~((1<<T6963C_CD));
	//T6963C_DDR_CTRL	|= (1<<T6963C_WR) | (1<<T6963C_RD) | (1<<T6963C_CE) | (1<<T6963C_RST) | (1<<T6963C_CD);
	T6963C_DDR_CTRL	|= (1<<T6963C_WR) | (1<<T6963C_RD) | (1<<T6963C_CE) | (1<<T6963C_CD);

	//Reset
	T6963C_RESET_ENABLE;
	_delay_ms(10);
	T6963C_RESET_DISABLE;
	_delay_ms(10);

	//Set Graphics Home Address 0x0000
	T6963cPutData((uint8_t) (T6963C_ADDR_GRAPHIC >> 0));
	T6963cPutData((uint8_t) (T6963C_ADDR_GRAPHIC >> 8));
	T6963cPutCmd(T6963C_CMD_SETGRAPHICHOME);

	//Set Graphics Area to T6963C_GRPH_COLS byte per line
	T6963cPutData(T6963C_GRPH_COLS);
	T6963cPutData(0);
	T6963cPutCmd(T6963C_CMD_SETGRAPHICAREA);

	//Set Text Home Address to 0x0800
	T6963cPutData((uint8_t) (T6963C_ADDR_TEXT >> 0));
	T6963cPutData((uint8_t) (T6963C_ADDR_TEXT >> 8));
	T6963cPutCmd(T6963C_CMD_SETTEXTHOME);

	//Set Text Area to T6963C_TEXT_COLS columns
	T6963cPutData(T6963C_TEXT_COLS);
	T6963cPutData(0);
	T6963cPutCmd(T6963C_CMD_SETTEXTAREA);
		
	//Set Internal CGROM and XOR mode
	T6963cPutCmd(T6963C_CMD_MODESET | Options);

	//Set Display Mode
	T6963cPutCmd(T6963C_CMD_DISPMODE | Mode);

	//Set CGRAM Address to 1800-1FFF
	T6963cPutData(T6963C_ADDR_CGRAML >> 11);
	T6963cPutData(0);
	T6963cPutCmd(T6963C_CMD_SETOFFSET);

	//Clear Display
	T6963cClear();

	//Reset some Addresses
	T6963cSetAddress(0);
	T6963cSetCursorXY(0, 0);

}

void T6963cCursorPattern(uint8_t Height)
{
	T6963cPutCmd(T6963C_CMD_CURSORPATTERN | (Height - 1));
}

void T6963cSetCursorXY(uint8_t X, uint8_t Y)
{
	T6963cPutData(X);
	T6963cPutData(Y);
	T6963cPutCmd(T6963C_CMD_SETCURSOR);
}

void T6963cClear()
{
	uint16_t i;
	
	//Clear Text RAM using AutoWriting
	T6963cSetAddress(T6963C_ADDR_TEXT);
	T6963cPutCmd(T6963C_CMD_AUTOWRITE);
	while ((T6963cGetStatus() & (T6963C_STA0 | T6963C_STA1)) != (T6963C_STA0 | T6963C_STA1))
		nop();
	for (i=0; i<(T6963C_TEXT_COLS * T6963C_TEXT_ROWS); i++)
		T6963cPutDataAW(0x00);
	T6963cPutCmd(T6963C_CMD_AUTORESET);
	
	//Clear Graphic RAM using AutoWriting
	T6963cSetAddress(T6963C_ADDR_GRAPHIC);
	T6963cPutCmd(T6963C_CMD_AUTOWRITE);
	while ((T6963cGetStatus() & (T6963C_STA0 | T6963C_STA1)) != (T6963C_STA0 | T6963C_STA1))
		nop();
	for (i=0; i<(T6963C_GRPH_COLS * T6963C_GRPH_ROWS); i++)
		T6963cPutDataAW(0x00);
	T6963cPutCmd(T6963C_CMD_AUTORESET);
	
}

void T6963cSetAddress(uint16_t Address)
{
	//Send Address-LowByte, HighByte and Command
	T6963cPutData((uint8_t) (Address >> 0));
	T6963cPutData((uint8_t) (Address >> 8));
	T6963cPutCmd(T6963C_CMD_SETADDRESS);
}


void T6963cWrite(uint8_t Byte)
{
	//Send Databyte and Command
	T6963cPutData(Byte);
	T6963cPutCmd(T6963C_CMD_WRITE_INC);
}

void T6963cWriteChunk(uint8_t* pBuffer, uint16_t nBytes)
{
	//Initiate AutoWrite, write Data from RAM and escape AutoWrite-Mode
	T6963cPutCmd(T6963C_CMD_AUTOWRITE);
	while ((T6963cGetStatus() & (T6963C_STA0 | T6963C_STA1)) != (T6963C_STA0 | T6963C_STA1))
		nop();	
	while (nBytes--)
		T6963cPutDataAW(*pBuffer++);
	T6963cPutCmd(T6963C_CMD_AUTORESET);
}

void T6963cWriteChunk_P(const prog_uint8_t* pBuffer, uint16_t nBytes)
{
	//Initiate AutoWrite, write Data from Flash and escape AutoWrite-Mode
	T6963cPutCmd(T6963C_CMD_AUTOWRITE);
	while ((T6963cGetStatus() & (T6963C_STA0 | T6963C_STA1)) != (T6963C_STA0 | T6963C_STA1))
		nop();
	while (nBytes--)
		T6963cPutDataAW(pgm_read_byte(pBuffer++));
	T6963cPutCmd(T6963C_CMD_AUTORESET);
}

void T6963cSetPixel(uint8_t Bit)
{
	//OR Command with Pixelnumber and send Command
	T6963cPutCmd(T6963C_CMD_SETPIXEL | (Bit & 0x07));
}

void T6963cResetPixel(uint8_t Bit)
{
	//OR Command with Pixelnumber and send Command
	T6963cPutCmd(T6963C_CMD_RESETPIXEL | (Bit & 0x07));
}

void T6963cPutChar(char Char)
{
	//Subtract 0x20 (T6963c has a slightly different ASCII Charset), send as Data and send Command
	uint8_t c = Char;

	if (c < 0x80)
		c -= 0x20;

	T6963cPutData(c);
	T6963cPutCmd(T6963C_CMD_WRITE_INC);
}

void T6963cPutString(char* pString)
{
	//Initiate AutoWrite. Then fetch each char from RAM and send, while the String has not ended.
	//escape AutoWrite-Mode when finished.
	uint8_t c;

	T6963cPutCmd(T6963C_CMD_AUTOWRITE);
	while ((T6963cGetStatus() & (T6963C_STA0 | T6963C_STA1)) != (T6963C_STA0 | T6963C_STA1))
		nop();
	while ((c = *pString++) != '\0')
	{
		if (c < 0x80)
			c -= 0x20;
		T6963cPutDataAW(c);
	}
	T6963cPutCmd(T6963C_CMD_AUTORESET);
}

void T6963cPutString_P(const prog_char* pString)
{
	//Initiate AutoWrite. Then fetch each char from Flash and send, while the String has not ended.
	//escape AutoWrite-Mode when finished.
	uint8_t c;

	T6963cPutCmd(T6963C_CMD_AUTOWRITE);
	while ((T6963cGetStatus() & (T6963C_STA0 | T6963C_STA1)) != (T6963C_STA0 | T6963C_STA1))
		nop();
	while ((c = pgm_read_byte(pString++)) != '\0')
	{
		if (c < 0x80)
			c -= 0x20;
		T6963cPutDataAW(c);
	}
	T6963cPutCmd(T6963C_CMD_AUTORESET);
}
