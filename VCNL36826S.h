/*
@file VCNL36826S.h

@Author
Ernesto Ortiz-Perdomo
e_ortizpe1810@hotmail.com

*/

#ifndef VCNL36826S_h
#define VCNL36826S_h

#include "Arduino.h"
#include <Wire.h>       //Std library I2C

/*   Constants   */

#define VCNL36826S_I2C_ADDR 0x60     // I2C Address 

/*   Registers   */ 

#define PS_CONF1      0x00  
#define PS_CONF2      0x03
#define PS_CONF3      0x04
#define PS_CONF4      0x04
#define PS_THDL       0x05
#define PS_THDH       0x06
#define PS_CANC       0x07
#define PS_AC_L       0x08
#define PS_LP     0x08
#define PS_DATA       0xF8
#define INT_FLAG      0xF9
#define VCNL36826S_ID 0xFA
#define PS_AC         0xFB

/*	Register 0x00 Data byte low  Table 2 Datasheet CONF1_L*/

#define PS_INIT 0x01<<7
#define PS_ON 0x01<<1

/*	Register 0x00 Data byte high  Table 2 Datasheet CONF1_H

Reserverd 

*/

/*  Register 0x03 Data byte Low  Table 4 Datasheet CONF2_L */

#define PS_PERIOD_10ms    0x00<<6
#define PS_PERIOD_20ms 	  0x01<<6
#define PS_PERIOD_40ms    0x02<<6
#define PS_PERIOD_80ms    0x03<<6
#define PS_PERS                     //PS interrupt persistence setting
#define PS_INT_DIS        0x00<<2
#define PS_INT_EN         0x03<<2   //Enable interruptions 
#define PS_SMART_PERS_EN  0x01<<1   // Enable smart persistence
#define PS_SMART_PERS_DIS 0x00<<1
#define PS_ST_S           0x00<<0     //start PS
#define PS_ST_P           0x01<<0

/*	Register 0x03 data byte high Table 5 Datasheet CONF2_H */

#define PS_IT_1T    0x00<<6
#define PS_IT_2T    0x01<<6
#define PS_IT_4T    0x02<<6		
#define PS_IT_8T    0x03<<6
#define PS_MPS_1    0x00<<4
#define PS_MPS_2    0x01<<4
#define PS_MPS_4    0x02<<4
#define PS_MPS_8    0x03<<4
#define PS_ITB_25   0x00<<3
#define PS_ITB_50   0x01<<3
#define PS_HG_EN    0x00<<2
#define PS_HG_DIS   0x00<<2

/*	Register 0x04 Data byte low  Table 6 PS_CONF3  */

#define PS_AF_EN    0x01<<6
#define PS_AUTO     0x00<<6
#define PS_FOR_Trig 0x01<<5


/* Register 0x04 Data byte high Table 7 PS_CONF4*/

#define PS_SC_EN      0x03<<5    		// Sun cancelation enable
#define PS_HD_12      0x00<<4			// PS output 12 bits
#define PS_HD_16 	  0x01<<4			// PS output 16 bits
#define VCSEL_I_6mA   0x00<<0       // Current selection 
#define VCSEL_I_8mA   0x01<<0
#define VCSEL_I_10mA    0x02<<0
#define VCSEL_I_12mA    0x03<<0
#define VCSEL_I_14mA    0x04<<0
#define VCSEL_I_16mA    0x05<<0
#define VCSEL_I_18mA    0x06<<0
#define VCSEL_I_20mA    0x07<<0

/*	Register 0x08 Data byte low Table 12  */
#define PS_LPPER_40		0x00<<1 	
#define PS_LPPER_80		0x01<<1
#define PS_LPPER_160	0x02<<1
#define PS_LPPER_320	0x03<<1  
#define PS_LPEN_EN		0x01<<0
#define PS_LPEN_DIS		0x00<<0

/*
	@brief VCNL36826S class
*/

class VCNL36826S{
	public:
		VCNL36826S();
		boolean exists();		//  Check connection
		boolean initial();			//	Initial configuration				
		//boolean setInter();		//	Set interruption
		boolean lowPower();		//	Set low power mode
		
		boolean SetVCSELCurrent(uint8_t c3 = B00000000 , uint8_t c4 = B00010100);

    	uint16_t readProximity(void);
    	
    private:
    	void write8b(uint8_t address,uint8_t data);
    	void write16b(uint8_t address, uint8_t low, uint8_t high);
    	uint16_t readData(uint8_t command_code);
    	
    	uint8_t _i2caddr;
    	TwoWire *_wire;
};
#endif
