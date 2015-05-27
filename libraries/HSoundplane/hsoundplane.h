////////////////////////////////////////////////////////////////////////////
//
//  This file is part of HSoundplane library
//
//	Works with the following hardware (150415):
//		- Soundplane piezo-driver v0.95 - R003
//		- Soundplane piezo-layer v.095 - R006
//
//  Copyright (c) 2015, www.icst.net
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of 
//  this software and associated documentation files (the "Software"), to deal in 
//  the Software without restriction, including without limitation the rights to use, 
//  copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the 
//  Software, and to permit persons to whom the Software is furnished to do so, 
//  subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in all 
//  copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION 
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef _HSOUNDPLANE_H
#define _HSOUNDPLANE_H

#include "drv2667.h"

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | MACROS																	| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
// HSoundplane characteristics
#define HS_SLAVE_NUMBER		4			// number of slaves to drive (1 - 4)
#define HS_COORD_MAX		16			// maximal amount of simultaneous coordinate pairs
#define HS_CPS				8			// number of columns per slave (usually 8)
#define HS_DPS				8			// number of drv2667 per slave (usually 8)
#define HS_PIEZO_MAX		72			// absolute maximum available piezos on a slave
#define HS_9RAW_MODE		0			//
#define HS_COL_OFFSET		1
#define HS_COL_NUMBER		30

// I2C addresses
#define I2C_MASTER_ADDRESS	0x40		// i2c master address
#define I2C_SLAVE_ADDR1		0x50		// i2c slave1 address
#define I2C_SLAVE_ADDR2		0x51		// i2c slave2 address
#define I2C_SLAVE_ADDR3		0x52		// i2c slave3 address
#define I2C_SLAVE_ADDR4		0x53 		// i2c slave4 address
#define I2C_SWITCH_ADDR1	0x70		// i2c switch1 address (range: 0x70 - 0x77)
#define I2C_SWITCH_ADDR2	0x71		// i2c switch2 address
#define I2C_SWITCH_ADDR3	0x72		// i2c switch3 address
#define I2C_SWITCH_ADDR4	0x73		// i2c switch4 address

// Pinout of the arduino nano on the driver board
#define LED1_PIN			3			// LED1 -> device started up
#define LED2_PIN			5			// LED2 -> drv2667 enabled
#define LED3_PIN			6			// LED3 -> SPI activity
#define XCK_PIN				4			// UART clock
#define SCL_PIN				A5			// i2c SCL (not needed since automatically assigned by setup)
#define SDA_PIN				A4			// i2c SDA (not needed since automatically assigned by setup)
#define SCK_PIN				13			// SPI clock (i.e. shift registers shifting clock)
#define SS_PIN				10			// slave select (for SPI slave mode)
#define MISO_PIN			12			// MISO (for SPI master receiver/slave transmitter mode)
#define MOSI_PIN			11			// MOSI (i.e. shift register serial in)
#define OE_PIN				7			// shift registers output enable (active low)
#define LOAD_PIN			8			// shift registers load clock (active rising)
#define CLR_PIN				9			// shift registers clear (active low)
#define SW_ADDR_0			2			// i2c switch hardware address bit0
#define SW_ADDR_1			A2			// i2c switch hardware address bit1
#define SW_ADDR_2			A3			// i2c switch hardware address bit2

#define LED_ON				LOW			// macro to set if LEDs are switched on HIGH or LOW
#define LED_OFF				HIGH		// and never forget it after that


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | ENUM																	| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
// I2C command headers
// -------------------
// Commands from master to slave with first byte (below) defining command type
enum i2cCommand {
	i2cCmd_regSet,
	i2cCmd_notify
};

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | VARIABLES																| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
// Array of eight piezo columns (8 x 9) single addressed. To combine: bitwise AND (&)
// NOTE: works also with 5 piezos connected per raw (master jumps over empty contacts)
const uint32_t piezoArray1[32] = {
	0xFFFFFFFE, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF7, 0xFFFFFFEF, 0xFFFFFFDF, 0xFFFFFFBF, 0xFFFFFF7F, 0xFFFFFEFF,	// row 1
	0XFFFFFDFF, 0XFFFFFBFF, 0XFFFFF7FF, 0XFFFFEFFF, 0XFFFFDFFF, 0XFFFFBFFF, 0XFFFF7FFF, 0XFFFEFFFF, 0XFFFDFFFF,	// row 2
	0XFFFBFFFF, 0XFFF7FFFF, 0XFFEFFFFF, 0XFFDFFFFF, 0XFFBFFFFF, 0XFF7FFFFF, 0XFEFFFFFF, 0XFDFFFFFF, 0XFBFFFFFF,	// row 3
	0XF7FFFFFF, 0xEFFFFFFF, 0xDFFFFFFF, 0xBFFFFFFF, 0x7FFFFFFF};												// row 4 (5/9)
const uint32_t piezoArray2[32] = {
	0xFFFFFFFE, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF7,																// raw 4 (4/9)
	0xFFFFFFEF, 0xFFFFFFDF, 0xFFFFFFBF, 0xFFFFFF7F, 0xFFFFFEFF, 0XFFFFFDFF, 0XFFFFFBFF, 0XFFFFF7FF, 0XFFFFEFFF,	// raw 5
	0XFFFFDFFF, 0XFFFFBFFF, 0XFFFF7FFF, 0XFFFEFFFF, 0XFFFDFFFF, 0XFFFBFFFF, 0XFFF7FFFF, 0XFFEFFFFF, 0XFFDFFFFF, // raw 6
	0XFFBFFFFF, 0XFF7FFFFF, 0XFEFFFFFF, 0XFDFFFFFF, 0XFBFFFFFF, 0XF7FFFFFF, 0xEFFFFFFF, 0xDFFFFFFF, 0xBFFFFFFF, // raw 7
	0x7FFFFFFF};																								// raw 8 (1/9)
const uint32_t piezoArray3[8] = {
	0xFFFFFFFE, 0xFFFFFFFD, 0xFFFFFFFB, 0xFFFFFFF7, 0xFFFFFFEF, 0xFFFFFFDF, 0xFFFFFFBF, 0xFFFFFF7F};			// row 8 (8/9)


typedef struct HSdata {
	// raw9 flag...
#if(HS_9RAW_MODE > 0)
	bool raw9 = true;
#else
	bool raw9 = false;
#endif
	
	// command mode flags...
	bool piezoOffAll[HS_SLAVE_NUMBER];				// piezos OFF (all) command for each slave
	bool drvOnAll[HS_SLAVE_NUMBER];					// driver ON (all) command for each slave
	bool drvOffAll[HS_SLAVE_NUMBER];				// drivers OFF (all) command for each slave
	uint8_t drvOn[HS_SLAVE_NUMBER];					// driver ON command for each slave
	uint8_t drvOff[HS_SLAVE_NUMBER];				// driver OFF command for each slave
	
	// data arrays...
	uint8_t inputCoord[HS_COORD_MAX][2];				// input HS coordinates send from computer to master
	uint8_t outputIndex[HS_SLAVE_NUMBER][HS_COORD_MAX];	// output HS piezo indexes for each slave
	uint8_t indexCnt[HS_SLAVE_NUMBER];					// piezo index counter for each slave
	uint8_t drvBm[HS_SLAVE_NUMBER];						// driver bit mask for each slave
	uint8_t drvOldBm[HS_SLAVE_NUMBER];					// previous driver bit mask

	// i2c variables...
	int8_t i2cSlaveAddress[HS_SLAVE_NUMBER] = {		// slave i2c addresses array
		I2C_SLAVE_ADDR1,
		I2C_SLAVE_ADDR2,
		I2C_SLAVE_ADDR3,
		I2C_SLAVE_ADDR4
	};
	int8_t i2cSwitchAddress[HS_SLAVE_NUMBER] = {	// i2c switch addresses array
		I2C_SWITCH_ADDR1,
		I2C_SWITCH_ADDR2,
		I2C_SWITCH_ADDR3,
		I2C_SWITCH_ADDR4
	};
	bool i2cSlaveAvailable[HS_SLAVE_NUMBER];		// slave availability flags
	uint8_t i2cSlaveSetup[HS_SLAVE_NUMBER];			// slave correctly set up (bit mask for each driver)
};


// extern...
extern bool debug;
extern SPISettings settingsA;

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | FUNCTIONS																| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void HSInit(void);

#endif
