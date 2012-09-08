/*
	T6963c.h - Defines and Function prototypes

	Simple LowLevel T6963c Library without Drawing functions
	for Rect's, Circles, etc...
	The files should be self-explanatory.

	(c) Simon Kueppers. If you find a bug or if you have a 
	question, feel free to Email me:
	simon.kueppers@stlcd.de
*/

#ifndef T6963C_H
#define T6963C_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdint.h>

//Pinout
#define T6963C_PORT_DATA 				PORTC	//Dataport
#define	T6963C_DDR_DATA					DDRC
#define T6963C_PIN_DATA					PINC

#define T6963C_PORT_CTRL				PORTD	//Controlport (RW, RD, CE, RST, CD)
#define T6963C_DDR_CTRL					DDRD
#define T6963C_PIN_CTRL					PINDA

#define T6963C_WR						PA2		//Controlbits
#define T6963C_RD						PA4
#define T6963C_CE						PA3
//#define T6963C_RST						PA3
#define T6963C_CD						PA5

//Physical Description
#define T6963C_ADDR_CGRAML				0x1800	//Lower half of CG (Chars 00-7F)
#define T6963C_ADDR_CGRAMH				(T6963C_ADDR_CGRAML + 0x80*8)	//Higher half of CG (Chars 80-FF)
#define T6963C_ADDR_GRAPHIC				0x0400
#define T6963C_ADDR_TEXT				0x0000
#define T6963C_TEXT_COLS				20		//40 bytes for 6x8 Font, 30 for 8x8 Font
#define T6963C_TEXT_ROWS				16
#define T6963C_GRPH_COLS				T6963C_TEXT_COLS
#define T6963C_GRPH_ROWS				128
#define T6963C_FONTWIDTH				8

//Options Defines for Init()
#define T6963C_MODE_OR					0x00
#define T6963C_MODE_XOR					0x01
#define T6963C_MODE_AND					0x03
#define T6963C_MODE_TEXTATTR			0x04
#define T6963C_CG_INTERNALROM			0x00
#define T6963C_CG_EXTERNALRAM			0x04

//Mode Defines for Init()
#define T6963C_DISP_OFF					0x00
#define T6963C_CURSOR					0x02
#define T6963C_CURSOR_BLINK				0x03
#define T6963C_TEXT						0x04
#define T6963C_GRAPHIC					0x08
#define T6963C_TEXT_GRAPHIC				0x0C


//Sets Port & Initializes T6963c
void T6963cInit(uint8_t Options, uint8_t Mode);

//Set Cursor-Pattern
void T6963cCursorPattern(uint8_t Height);

//Locates Cursor at Text-Position
void T6963cSetCursorXY(uint8_t X, uint8_t Y);

//Clears Graphic and Text RAM
void T6963cClear();

//Sets Addresspointer for reading/writing from/to T6963c
void T6963cSetAddress(uint16_t Address);

//Writes a Single Byte and increments Address
void T6963cWrite(uint8_t Byte);

//Writes a Chunk from OnChip-RAM
void T6963cWriteChunk(uint8_t* pBuffer, uint16_t nBytes);

//Writes a Chunk from OnChip-Flash
void T6963cWriteChunk_P(const prog_uint8_t* pBuffer, uint16_t nBytes);

//Sets a Pixel (7=MSBit, 0=LSBit)
void T6963cSetPixel(uint8_t Bit);

//Resets a Pixel
void T6963cResetPixel(uint8_t Bit);

//Puts a char (Set Addresspointer to T6963C_ADDR_TEXT before!)
void T6963cPutChar(char Char);

//Puts a 0-terminated String from OnChip-RAM. (Set Addresspointer to T6963C_ADDR_TEXT before!)
void T6963cPutString(char* String);

//Puts a 0-terminated String from OnChip-Flash. (Set Addresspointer to T6963C_ADDR_TEXT before!)
void T6963cPutString_P(const prog_char* String);

//Defines for more comfort: Use them like a function
#define T6963cWriteAt(Address, Byte) \
	do { \
		T6963cSetAddress(Address); \
		T6963cWrite(Byte); \
	} while(0)

#define T6963cWriteChunkAt(StartAddress, pBuffer, nBytes) \
	do { \
	  	T6963cSetAddress(StartAddress); \
		T6963cWriteChunk(pBuffer, nBytes); \
	} while(0)

#define T6963cWriteChunkAt_P(StartAddress, pBuffer, nBytes) \
	do { \
		T6963cSetAddress(StartAddress); \
		T6963cWriteChunk_P(pBuffer, nBytes); \
	} while(0)

#define T6963cSetPixelAt(Address, Bit) \
	do { \
		T6963cSetAddress(Address); \
		T6963cSetPixel(Bit); \
	} while(0)

#define T6963cSetPixelXY(X, Y) \
	do { \
		T6963cSetAddress(T6963C_ADDR_GRAPHIC + (T6963C_GRPH_COLS * (Y)) + ((X) / T6963C_FONTWIDTH)); \
		T6963cSetPixel((T6963C_FONTWIDTH - 1) - ((X) % T6963C_FONTWIDTH)); \
	} while(0)

#define T6963cResetPixelAt(Address, Bit) \
	do { \
		T6963cSetAddress(Address); \
		T6963cResetPixel(Bit); \
	} while(0)

#define T6963cResetPixelXY(X, Y) \
	do { \
		T6963cSetAddress(T6963C_ADDR_GRAPHIC + (T6963C_GRPH_COLS * (Y)) + ((X) / T6963C_FONTWIDTH)); \
		T6963cResetPixel((T6963C_FONTWIDTH - 1) - ((X) % T6963C_FONTWIDTH)); \
	} while(0)

#define T6963cPutCharAt(Address, Char) \
	do { \
		T6963cSetAddress(Address); \
		T6963cPutChar(Char); \
	} while(0)

#define T6963cPutCharXY(X, Y, Char) \
	do { \
		T6963cSetAddress(T6963C_ADDR_TEXT + (T6963C_TEXT_COLS * (Y)) + (X)); \
		T6963cPutChar(Char); \
	} while(0)

#define T6963cPutStringAt(Address, pString) \
	do { \
		T6963cSetAddress(Address); \
		T6963cPutString(pString); \
	} while(0)

#define T6963cPutStringXY(X, Y, pString) \
	do { \
		T6963cSetAddress(T6963C_ADDR_TEXT + (T6963C_TEXT_COLS * (Y)) + (X)); \
		T6963cPutString(pString); \
	} while(0)

#define T6963cPutStringAt_P(Address, pString) \
	do { \
		T6963cSetAddress(Address); \
		T6963cPutString_P(pString); \
	} while(0)

#define T6963cPutStringXY_P(X, Y, pString) \
	do { \
		T6963cSetAddress(T6963C_ADDR_TEXT + (T6963C_TEXT_COLS * (Y)) + (X)); \
		T6963cPutString_P(pString); \
	} while(0)

#endif //T6963C_H
