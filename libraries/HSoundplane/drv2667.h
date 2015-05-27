////////////////////////////////////////////////////////////////////////////
//
//  This file is part of the HSoundplane library
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

#ifndef _DRV2667_H
#define _DRV2667_H

// Resigster definitions of the TI DRV2667 piezo driver

#define DRV2667_I2C_ADDRESS		0x59


// Register addresses
#define DRV2667_REG00			0x00
#define DRV2667_REG01			0x01
#define DRV2667_REG02			0x02
#define DRV2667_REG03			0x03
#define DRV2667_REG04			0x04
#define DRV2667_REG05			0x05
#define DRV2667_REG06			0x06
#define DRV2667_REG07			0x07
#define DRV2667_REG08			0x08
#define DRV2667_REG09			0x09
#define DRV2667_REG0A			0x0A
#define DRV2667_REG0B			0x0B		
#define DRV2667_REGFF			0xFF

// Register 0x00 (Status)
#define ILLEGAL_ADDR			(1 << 2)
#define FIFO_EMPTY				(1 << 1)
#define FIFO_FULL				(1 << 0)

// Register 0x01 (Control 1)
#define ID_3					(1 << 6)
#define ID_2					(1 << 5)
#define ID_1					(1 << 4)
#define ID_0					(1 << 3)
#define INPUT_MUX				(1 << 2)
#define GAIN_1					(1 << 1)
#define GAIN_0					(1 << 0)

// Register 0x02 (Control 2)
#define DEV_RST					(1 << 7)
#define STANDBY					(1 << 6)
#define TIMEOUT_1				(1 << 3)
#define TIMEOUT_0				(1 << 2)
#define EN_OVERRIDE				(1 << 1)
#define GO						(1 << 0)

// Register 0x03 to 0x0A (Waveform sequencer)
// ...TBD

// Register 0x0B (FIFO)
#define FIFODATA_7				(1 << 7)
#define FIFODATA_6				(1 << 6)
#define FIFODATA_5				(1 << 5)
#define FIFODATA_4				(1 << 4)
#define FIFODATA_3				(1 << 3)
#define FIFODATA_2				(1 << 2)
#define FIFODATA_1				(1 << 1)
#define FIFODATA_0				(1 << 0)

// Register 0xFF (Page)
#define PAGE_7					(1 << 7)
#define PAGE_6					(1 << 6)
#define PAGE_5					(1 << 5)
#define PAGE_4					(1 << 4)
#define PAGE_3					(1 << 3)
#define PAGE_2					(1 << 2)
#define PAGE_1					(1 << 1)
#define PAGE_0					(1 << 0)

#endif
