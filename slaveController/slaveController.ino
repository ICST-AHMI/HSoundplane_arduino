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

#include <Wire.h>
#include <SPI.h>
#include "drv2667.h"
#include "HSoundplane.h"
#include "slaveSettings.h"

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | setup																	| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void setup()
{
	// Starting communication
	Serial.begin(SERIAL_SPEED); // Serial...
  
	Wire.begin(I2C_SLAVE_ADDRESS); // i2c...
	if(i2cFastMode) Wire.setClock(400000);
	Wire.onRequest(requestEvent); // attach request event (slave transmitter) handler
	Wire.onReceive(receiveEvent); // attach receive event (slave receiver) handler

	SPI.begin(); // SPI...

	// Setting up pin directions & values
	pinMode(LED1_PIN, OUTPUT);			// LEDs...
	pinMode(LED2_PIN, OUTPUT);
	pinMode(LED3_PIN, OUTPUT);
	
	pinMode(MOSI_PIN, OUTPUT);			// SPI...
	pinMode(MISO_PIN, INPUT_PULLUP);
	pinMode(SCK_PIN, OUTPUT);
	pinMode(SS_PIN, INPUT_PULLUP);
  
	pinMode(OE_PIN, OUTPUT);			// shift registers...
	pinMode(LOAD_PIN, OUTPUT);
	pinMode(CLR_PIN, OUTPUT);
	digitalWrite(OE_PIN, HIGH); 		// disable latch outputs
	digitalWrite(CLR_PIN, LOW);			// reset registers
	digitalWrite(CLR_PIN, HIGH);
	digitalWrite(LOAD_PIN, LOW);		// storage clock active on rising edge
	digitalWrite(OE_PIN, LOW);			// enable latch outputs
  
	switchAddress = (I2C_SWITCH_ADDRESS - I2C_SWITCH_ADDR1);
	pinMode(SW_ADDR_0, OUTPUT);			// i2c switch address
	digitalWrite(SW_ADDR_0, (switchAddress & 0x01));
	pinMode(SW_ADDR_1, OUTPUT);
	digitalWrite(SW_ADDR_1, (switchAddress & 0x02));
	pinMode(SW_ADDR_2, OUTPUT);  
	digitalWrite(SW_ADDR_2, (switchAddress & 0x04));
	pinMode(A6, OUTPUT);				// fixing patch for wrong routing on board
	pinMode(A7, OUTPUT);				// "Soundplane piezo-driver v0.95 - R002"
	
	
	syncPinState = false;				// sync pin...
	pinMode(SYNC_PIN_1, OUTPUT);
	digitalWrite(SYNC_PIN_1, syncPinState);
	
	slaveInitFlag = false;				// slave initialization flag...
	slaveNotifyFlag = false;			// slave notification flag...
	slaveWriteFlag = false;				// slave writing command flag...
  
  	if(debug) {
		Serial.print("\nStarting up slave controller... #"); Serial.println(SLAVE_ID, DEC);
		Serial.println("*************************************");
		Serial.print("serial:\n\t- port @ "); Serial.println(SERIAL_SPEED, DEC);
		Serial.print("i2c:\n\t- port @ "); Serial.println((i2cFastMode) ? "400 kHz" : "100 kHz");
		Serial.print("\t- own address: 0x"); Serial.println(I2C_SLAVE_ADDRESS, HEX);
		Serial.print("\t- switch address: 0x"); Serial.println(I2C_SWITCH_ADDRESS, HEX);
		Serial.print("\t- switch address pins: "); Serial.print(switchAddress & 0x04);
		Serial.print(" / "); Serial.print(switchAddress & 0x02); Serial.print(" / "); Serial.println(switchAddress & 0x01);
		Serial.println("piezos:\n\t- raws: 9\n\t- columns: 8\n\t -> available piezos: 72");
		Serial.println("*************************************\n");

		for(uint8_t i = 0; i < 3; i++) {
			digitalWrite(LED1_PIN, LED_ON);
			digitalWrite(LED2_PIN, LED_OFF);
			digitalWrite(LED3_PIN, LED_OFF);
			delay(50);
			digitalWrite(LED1_PIN, LED_OFF);
			digitalWrite(LED2_PIN, LED_ON);
			delay(50);
			digitalWrite(LED2_PIN, LED_OFF);
			digitalWrite(LED3_PIN, LED_ON);
			delay(50);
		}
	}
	
	// announcing startup done...
	digitalWrite(LED1_PIN, LED_ON);
	digitalWrite(LED2_PIN, LED_OFF);
	digitalWrite(LED3_PIN, LED_OFF);
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | loop																	| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void loop()
{
	// delay(1);
	if(slaveInitFlag) {

	}
	
	if(slaveNotifyFlag) {
		syncPinState = !syncPinState;
		digitalWrite(SYNC_PIN_1, syncPinState);

		slaveNotifyFlag = false;

		syncPinState = !syncPinState;
		digitalWrite(SYNC_PIN_1, syncPinState);
	} 
	
	if(slaveWriteFlag) {
		// syncPinState = !syncPinState;
		// digitalWrite(SYNC_PIN_1, syncPinState);

		// send the piezo bitmasks to the shift registers
		piezoSend(piezoVal1, piezoVal2, piezoVal3);
		slaveWriteFlag = false;

		// syncPinState = !syncPinState;
		// digitalWrite(SYNC_PIN_1, syncPinState);
	}
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | requestEvent															| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void requestEvent()
{
	// syncPinState = !syncPinState;
	// digitalWrite(SYNC_PIN_1, syncPinState);
	
	if(debug) {
		Serial.print("i2c registration request received... sending own address 0x");
		Serial.println(I2C_SLAVE_ADDRESS, HEX);
	}
	Wire.write(I2C_SLAVE_ADDRESS);

	// syncPinState = !syncPinState;
	// digitalWrite(SYNC_PIN_1, syncPinState);
}


/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | receiveEvent															| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void receiveEvent(int howmany)
{
	uint8_t decount = howmany;

	// receive the first byte and check if it's an initialization request
	uint8_t received = Wire.read();
	decount--;
	if(debug) {
		Serial.print("\nI2C message received... length: "); Serial.print(howmany, DEC);
		Serial.print(" first byte: 0x"); Serial.println(received, HEX);
	}
	
	switch(received) {
		case i2cCmd_regSet:
			if(debug) {
			Serial.println("Setting shift registers...");
		}
			// receive all sent bytes and set (bitwise AND) the piezo bismasks
			piezoVal1 = 0xFFFFFFFF;
			piezoVal2 = 0xFFFFFFFF;
			piezoVal3 = 0xFFFFFFFF;
		
			syncPinState = !syncPinState;
			digitalWrite(SYNC_PIN_1, syncPinState);

			while(decount > 0) {
				received = Wire.read();
				decount--;
				if(debug) {
					Serial.print("Received "); Serial.print(received, DEC);
				}
				if(received < 32) {
					piezoVal1 &= piezoArray1[received];
					if(debug) {
						Serial.print("\t-> piezoVal1: 0x"); Serial.println(piezoVal1, HEX);
					}
				} else if(received < 64) {
					piezoVal2 &= piezoArray2[received-32];
					if(debug) {
						Serial.print("\t-> piezoVal2: 0x"); Serial.println(piezoVal2, HEX);
					}
				} else if(received < 72) {
					piezoVal3 &= piezoArray3[received-64];
					if(debug) {
						Serial.print("\t-> piezoVal3: 0x"); Serial.println(piezoVal3, HEX);
					}
				} else {
					if(debug) {
						Serial.println("\t!!piezo value out of range!!");
					}
				}
			}
			if(debug) {
				Serial.println("");
			}

			slaveWriteFlag = true;

			syncPinState = !syncPinState;
			digitalWrite(SYNC_PIN_1, syncPinState);
			break;
		
		case i2cCmd_notify:
			syncPinState = !syncPinState;
			digitalWrite(SYNC_PIN_1, syncPinState);
			
			received = Wire.read();
			decount--;
			if(received == 1) {
				if(debug) {
					Serial.println("OK notification");
				}
				digitalWrite(LED2_PIN, LED_ON);
			} else {
				if(debug) {
					Serial.println("NOT OK notification");
				}
				digitalWrite(LED2_PIN, LED_OFF);
			}
			while(decount > 0) {
				received = Wire.read();
				decount--;
			}
			
			syncPinState = !syncPinState;
			digitalWrite(SYNC_PIN_1, syncPinState);
			break;
		
		default:
			break;
	}
}




/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
/* | piezoSend																| */
/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void piezoSend(uint32_t val1, uint32_t val2, uint32_t val3)
{
	digitalWrite(LED3_PIN, LOW);		// notify SPI activity
	digitalWrite(LOAD_PIN, LOW);		// prepare LOAD pin
	digitalWrite(CLR_PIN, LOW);			// reset all shift registers
	digitalWrite(CLR_PIN, HIGH);		// ...
	
	SPI.beginTransaction(settingsA);	// start the SPI transaction with saved settings
	SPI.transfer((uint8_t)(val3));
	SPI.transfer((uint8_t)(val2 >> 24));
	SPI.transfer((uint8_t)(val2 >> 16));
	SPI.transfer((uint8_t)(val2 >> 8));
	SPI.transfer((uint8_t)(val2));
	SPI.transfer((uint8_t)(val1 >> 24));
	SPI.transfer((uint8_t)(val1 >> 16));
	SPI.transfer((uint8_t)(val1 >> 8));
	SPI.transfer((uint8_t)(val1));
	SPI.endTransaction();
	
	if(debug) {
		Serial.print("Sending to shift registers:\nb'");
		Serial.print((uint8_t)(val3), BIN); Serial.print(" ");
		Serial.print((uint8_t)(val2 >> 24), BIN); Serial.print(" ");
		Serial.print((uint8_t)(val2 >> 16), BIN); Serial.print(" ");
		Serial.print((uint8_t)(val2 >> 8), BIN); Serial.print(" ");
		Serial.print((uint8_t)(val2), BIN); Serial.print(" ");
		Serial.print((uint8_t)(val1 >> 24), BIN); Serial.print(" ");
		Serial.print((uint8_t)(val1 >> 16), BIN); Serial.print(" ");
		Serial.print((uint8_t)(val1 >> 8), BIN); Serial.print(" ");
		Serial.print((uint8_t)(val1), BIN); Serial.println("'");
		
		Serial.print("0x   "); Serial.print((uint8_t)val3, HEX);
		Serial.print("       "); Serial.print((uint8_t)(val2 >> 24), HEX);
		Serial.print("       "); Serial.print((uint8_t)(val2 >> 16), HEX);
		Serial.print("       "); Serial.print((uint8_t)(val2 >> 8), HEX);
		Serial.print("       "); Serial.print((uint8_t)(val2), HEX);
		Serial.print("       "); Serial.print((uint8_t)(val1 >> 24), HEX);
		Serial.print("       "); Serial.print((uint8_t)(val1 >> 16), HEX);
		Serial.print("       "); Serial.print((uint8_t)(val1 >> 8), HEX);
		Serial.print("       "); Serial.println((uint8_t)(val1), HEX);
	}
	
	digitalWrite(LOAD_PIN, HIGH);		// generate rising edge on LOAD pin
	digitalWrite(LED3_PIN, HIGH);		// stop SPI activity notification
}
