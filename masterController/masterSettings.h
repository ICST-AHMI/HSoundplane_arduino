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

#ifndef _MASTERSETTINGS_H
#define _MASTERSETTINGS_H

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | MACROS																	| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#define I2C_FAST_MODE		1			// 0 -> standad mode (100 kHz) i2c,
										// 1 -> fast mode (400 kHz)
#define SERIAL_SPEED		230400		// serial communication speed for
										// 1. host command reception (CDC or rawHID)
										// 2. debugging

#define SCMD_START			253			// serial command start byte
#define SCMD_STOP			255			// serial command stop byte
//-- SERIAL COMMANDS --
#define SCMD_SETTINGS		100			// threshold value above wich
										// setting commands are sent
#define SCMD_POFF_ALL		110			// switch off all relays (byte 2: slave#)
#define SCMD_DOFF_S0		120			// switch off slave0 drivers (byte 2: drivers bitmask)
#define SCMD_DOFF_S1		121			// switch off slave1 drivers (byte 2: drivers bitmask)
#define SCMD_DOFF_S2		122			// switch off slave2 drivers (byte 2: drivers bitmask)
#define SCMD_DOFF_S3		123			// switch off slave3 drivers (byte 2: drivers bitmask)
#define SCMD_DOFF_ALL		129			// switch off all drivers (byte 2: slave#)
#define SCMD_DON_S0			130			// switch on slave0 drivers (byte 2: drivers bitmask)
#define SCMD_DON_S1			131			// switch on slave1 drivers (byte 2: drivers bitmask)
#define SCMD_DON_S2			132			// switch on slave2 drivers (byte 2: drivers bitmask)
#define SCMD_DON_S3			133			// switch on slave3 drivers (byte 2: drivers bitmask)
#define SCMD_DON_ALL		139			// switch on all drivers (byte 2: slave#)
#define SCMD_DEBUG			200			// toggle debug mode (byte 2: 0 -> off, >0 -> on)
#define SCMD_RESET			250			// master software reset (byte 2: unused)
//-- ERROR MESSAGES --
#define SERR_NOERROR		0			// no error
#define SERR_MISMATCH		1			// mismatch between entered and calculated length
#define SERR_COORD			2
#define SERR_SETTINGS		3
#define SERR_CRLF			255

#define STARTUP_WAIT_MS		500			// startup waiting time to let the slaves be ready
#define INIT_WAIT_MS		50			// initialization waiting time to SEE slave getting ready

#define SLAVE_REG_RETRIES	5

#define SYNC_PIN_1			2			// pin used to measure time between events


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | VARIABLES																| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
bool debug = false;						// DEBUG FLAG!!

#if(I2C_FAST_MODE > 0)					// i2c speed flag
  bool i2cFastMode = true;
#else
  bool i2cFastMode = false;
#endif
  
bool syncPinState;

// String slicedCmd[2 * HS_COORD_MAX];		// command line sliced into integers

// extern...
extern struct HSdata HSd;


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | FUNCTIONS																| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void parseCommand(void);
void registerSlave(void);
uint8_t setupSlaveDrv(int8_t addr, uint8_t dbm, bool reset, bool on, uint8_t gain);
void notifySlave(int8_t addr, bool notification);
void distributeCoordinates(uint8_t len, uint8_t orig[HS_COORD_MAX][2], uint8_t dest[HS_SLAVE_NUMBER][HS_COORD_MAX]);
void sendToSlave(uint8_t sAddr, uint8_t *mes, uint8_t len);
#endif