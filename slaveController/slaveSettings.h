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

#ifndef _SLAVESETTINGS_H
#define _SLAVESETTINGS_H

#include "HSoundplane.h"

#define SLAVE_ID			4
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | MACROS																	| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
// TO CHANGE FOR EACH SLAVE
// #define I2C_SLAVE_ADDRESS	I2C_SLAVE_ADDR1		// own i2c slave address to respond to

// GENERAL SLAVE SETTINGS
#define I2C_FAST_MODE		1			// 0 -> standad mode (100 kHz) i2c,
										// 1 -> fast mode (400 kHz)
#define SERIAL_SPEED		230400		// serial communication speed for debugging
#define SPI_SPEED			2000000		// SPI communication speed for shift registers

#define INIT_WAIT_MS		100

#define SYNC_PIN_1			A0			// pin used to measure time between events

#if(SLAVE_ID == 1)
#define I2C_SLAVE_ADDRESS	I2C_SLAVE_ADDR1
#define I2C_SWITCH_ADDRESS	I2C_SWITCH_ADDR1
#elif(SLAVE_ID == 2)
#define I2C_SLAVE_ADDRESS	I2C_SLAVE_ADDR2
#define I2C_SWITCH_ADDRESS	I2C_SWITCH_ADDR2
#elif(SLAVE_ID == 3)
#define I2C_SLAVE_ADDRESS	I2C_SLAVE_ADDR3
#define I2C_SWITCH_ADDRESS	I2C_SWITCH_ADDR3
#elif(SLAVE_ID == 4)
#define I2C_SLAVE_ADDRESS	I2C_SLAVE_ADDR4
#define I2C_SWITCH_ADDRESS	I2C_SWITCH_ADDR4
#else
#define I2C_SLAVE_ADDRESS	0xFF
#define I2C_SWITCH_ADDRESS	0xFF
#endif


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | VARIABLES																| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
bool debug = false;				// DEBUG FLAG!!

#if(I2C_FAST_MODE > 0)			// i2c speed flag
	bool i2cFastMode = true;
#else
	bool i2cFastMode = false;
#endif

SPISettings settingsA(SPI_SPEED, MSBFIRST, SPI_MODE0);	// SPI settings

bool syncPinState;
bool slaveInitFlag;
bool slaveNotifyFlag;
bool slaveWriteFlag;
uint8_t switchAddress;

uint32_t piezoVal1;
uint32_t piezoVal2;
uint32_t piezoVal3;


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | FUNCTIONS																| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void requestEvent(void);
void receiveEvent(int);
#endif