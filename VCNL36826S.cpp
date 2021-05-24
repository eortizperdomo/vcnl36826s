/*

@file VCNL36826S.cpp

@Author 
Ernesto Ortiz-Perdomo

*/

#include "VCNL36826S.h"

/*  @brief Instantiate a VCNL36862S class */

VCNL36826S::VCNL36826S(){
	_i2caddr = VCNL36826S_I2C_ADDR;
}

/*  @brief Check tha sensor exists and start I2C communication */
boolean VCNL36826S::exists(){
	
	Wire.begin();
	Wire.beginTransmission(_i2caddr);
	Wire.write(VCNL36826S_ID);
	Wire.endTransmission(false);
	Wire.requestFrom(_i2caddr,uint8_t(2)); // Read from Sensor 2 bytes
	byte lowByte = Wire.read();
	byte highByte = Wire.read();
	if(lowByte == 0x26){
		return true;
	}
	else{
		return false;
	}
}

/*
	@brief SetUp and basic functions 
*/

boolean VCNL36826S::initial(){
	// Reset  
	write16b(PS_CONF1, 0x01, 0x00);
	write16b(PS_CONF2, 0x00, 0x00);
	write16b(PS_CONF3, 0x00, 0x00);
	write16b(PS_THDL, 0x00, 0x00);
	write16b(PS_THDH, 0x00, 0x00);
	write16b(PS_CANC, 0x00, 0x00);
	write16b(PS_AC, 0x00, 0x00);
	
	write16b(PS_CONF4, 0x0C, 0xF7);
	write16b(PS_CONF2, 0x66, 0xFC);
	// Turn On Chip
	write16b(PS_CONF1, 0x03, 0x00); // Power ON
	//write16b(PS_CONF1, B10000011, B00000010); // Enable bias circuit
	write16b(PS_CONF1, 0x83, 0x02); // Enable bias circuit
	
	readCom(_i2caddr, PS_DATA);
	
	// Set the PS interrupt levels
	write16b(PS_THDL, 0x74, 0x0E);		// Low threshold dec 3700  ~= 180 mm
	write16b(PS_THDH, 0xFF, 0x0F);  	// High threshold dec 4095 Max ~= 200 mm 
	return true;
}

boolean VCNL36826S::interMod(uint8_t selection){
	uint8_t interrup = 0x12;
	uint8_t set = interrup xor selection;
	write16b(PS_CONF2, set, 0xD8);
	
}

/*  Power off Proximity sensor  
	@brief Set the proximity sensor off if needed. To turn the chip on again "initial()" must be called.
*/

boolean VCNL36826S::poweOffPS(){
	write16b(PS_CONF1, 0x00, 0x00);
	write16b(PS_CONF2, 0x00, 0x00);
}

/*	Settings for Low Power Mode

	@brief  According to requirements, Datasheet and Application notes.
*/

boolean VCNL36826S::lowPower(){
	write16b(PS_CONF2, 0xE4, 0xE8);				// Period 80 ms, persistence 3, interrupt h/l mode, Smart PS persistence disabled PS Started
	write16b(PS_CONF3, 0x0C, 0x12);	// Include PS_CONF4 (PS_CONF3_H) VCSEL_I = 10mA
	write16b(PS_LP, 0xAC, 0x07);  	// Period 320 ms, enable Low Power
}

/*  Settings for Normal Power mode */
boolean VCNL36826S::normalPower(){
	write16b(PS_LP, 0x3F, 0x00);  
	write16b(PS_CONF2, 0x16, 0x18);
	write16b(PS_CONF3, 0x5C, 0xF7);  //  VCSEL 20mA
}


/*	Return 16-bit proximity measurement */	

uint16_t VCNL36826S::readProximity(){
	//return readData(PS_DATA);
	return readCom(_i2caddr,PS_DATA);
}


/*  Reading the Interruption Flag 8 bits

	High/Low interruption mode no Interruption Flag attached.
	Only if Normal or First high interruption mode 
	
*/	
uint8_t VCNL36826S::readIntFlag(){
	Wire.beginTransmission(_i2caddr);
	Wire.write(INT_FLAG);
	Wire.endTransmission(false);
	Wire.requestFrom(_i2caddr,uint8_t(2));
	if (Wire.available() != 0x02)
	{
		return 0;
	}
	byte Lbyte = Wire.read();
	byte Hbyte = Wire.read();
	
	return Hbyte;
}
	/*
	uint16_t reading;
	Wire.beginTransmission(_i2caddr);
	Wire.write(INT_FLAG);
	Wire.endTransmission(false);
	Wire.requestFrom(_i2caddr, uint8_t(2));
	while(!Wire.available());
		uint8_t byteLow = Wire.read();
	while(!Wire.available());
		uint16_t byteHigh = Wire.read();
	return byteHigh;
	}
	*/


/****************************************************************************
@brief I2C Interface

*****************************************************************/

/*	

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
*/

/*  Read 2 bytes from sensor */
uint16_t VCNL36826S::readCom(int address, int cmdCod){
	Wire.beginTransmission(address);
	Wire.write(cmdCod);
	Wire.endTransmission(false);
	Wire.requestFrom(address,0x02);
	if (Wire.available() != 0x02)
	{
		return 0;
	}
	byte Lbyte = Wire.read();
	byte Hbyte = Wire.read();
	
	uint16_t value = (Hbyte << 8) | Lbyte;
	return value;
}
/*	Write 2 bytes to VCNL36862 data byte low data byte high*/

void VCNL36826S::write16b(uint8_t address, uint8_t low, uint8_t high){
	Wire.beginTransmission(_i2caddr);
	Wire.write(address);
	Wire.write(low);
	Wire.write(high);
	Wire.endTransmission();
}
