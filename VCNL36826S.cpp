/*

@file VCNL36826S.cpp

@Author 
Ernesto Ortiz-Perdomo

*/

#include "VCNL36826S.h"
/*  Instantiate a VCNL36862S class */
VCNL36862S::VCNL36826S(){
	_i2caddr = VCNL36826S_I2C_ADDR;
}

/*  Check tha sensor exists and start I2C communication */
boolean VCNL36826S::exists(){
	Wire.begin();
	Wire.beginTransmission(_i2caddr);
	Wire.write(VCNL36826S_ID);
	Wire.endTransmission(false);
	Wire.requestFrom(_i2caddr,uint8_t(2)) // Read from Sensor 2 bytes
	byte lowByte = Wire.read();
	byte highByte = Wire.read();
	
	Serial.println(lowByte);
	Serial.println(highByte);
	
	return true;
}

/*
	@brief SetUp and basic functions 
*/
boolean VCNL36826S::init(){
	
	write16b(PS_CONF1, b00000011, b00000000); // Power ON
	write16b(PS_CONF1, b10000011, b00000010); // Enable bias circuit
	
	write16b(PS_CONF2, b11110110, b11110000);  // Start, Enable interruption logic high/low mode 
	
	// Set the PS interrupt levels
	write16b(PS_THDL, b10010000, b00000001);  // Low threshold dec 400 
	write16b(PS_THDH, b11110100, b00000001);  // High threshold dec 500  Ref. Application Notes
	return true;	
}
boolean VCNL36826S::SetVCSELCurrent(c3,c4){
	/*
		VCSEL_I_6mA    // Current selection 
		VCSEL_I_8mA
		VCSEL_I_10mA
		VCSEL_I_12mA
		VCSEL_I_14mA
		VCSEL_I_16mA
		VCSEL_I_18mA
		VCSEL_I_20mA
	*/
	write8b(PS_CONF4,VCSEL_I_10mA);  //Default 10 mA
	return true;
}
boolean VCNL36826S::setInter(){
	write8b(PS_CONF3,VCSEL_I_10mA);
	return true;
}

/*	Settings for Low Power Mode*/

boolean VCNL36826S::lowPower(){
	write16b(PS_CONF3, b0001100, b00010010); 
	write16b(PS_LP, b10101101, b00000111);  
	return true;
}

/*	Return 16-bit proximity measurement */	

uint16_t VCNL36826S::readProximity(){
	return readData(PS_DATA);
}

/****************************************************************************
@brief I2C Interface

*****************************************************************/

/*	Read 2 bytes from sensor */

uint16_t VCNL36826S::readData(uint8_t command){
	uint16_t reading;
	Wire.beginTransmission(_i2caddr);
	Wire.write(command);
	Wire.endTransmission(false);
	Wire.requestFrom(_i2caddr, uint8_t(2));
	while(!Wire.available());
		uint8_t byteLow = Wire.read();
	while(!Wire.available());
		uint16_t byteHigh = Wire.read();
	reading = (byteHigh <<= 8) + byteLow;
}

/*	Write 2 bytes to VCNL36862 data byte low data byte high*/

void VCNL36826S::write16b(uint8_address, uint8_t low, uint8_t high){
	Wire.beginTransmission(_i2caddr);
	Wire.write(address);
	Wire.write(low);
	Wire.write(high);
	Wire.endTransmission();
}

/*  Function 8-bit write I2C */

void VCNL36826S::write8b(uint8_t address, uint8_t data){
	Wire.beginTransmission(_i2cadrr);
	Wire.write(address);
	Wire.write(data);
	Wire.endTransmission();
}