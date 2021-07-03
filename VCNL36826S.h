/*
    @file VCNL36826S.h
    
    @Author 
    Ernesto Ortiz - Perdomo
    V 1.1.0

*/

#ifndef VCNL36826S_h
#define VCNL36826S_h

#include "Arduino.h"
#include <Wire.h>

#define VCNL36826S_I2C_ADDR 0x60
#define VCNL36826S_ID  0xFA
#define PS_DATA 0xF8

#define PS_ON  0x02
#define PS_INIT 0x80
#define PS_bit1 0x200

#define PS_ST 0x00 


#define CONF_1 0x00
#define CONF_2 0x03
#define CONF_3 0x04   //L 3 H 4
#define THDL 0X05
#define THDH 0x06
#define CANC 0x07
#define AC_LPPERI 0x08
#define INT_Flag 0xF9  // H FLag

/*  Init registers */

// VCSEL current 
#define VCSEL_I_6mA     0x00
#define VCSEL_I_8mA     0x01
#define VCSEL_I_10mA    0x02
#define VCSEL_I_12mA    0x03
#define VCSEL_I_14mA    0x04
#define VCSEL_I_16mA    0x05
#define VCSEL_I_18mA    0x06
#define VCSEL_I_20mA    0x07

// PS HD 
#define PS_HD_12   0x00
#define PS_HD_16   0x10    // This one

// Sun cancelation
#define SC_EN 0xE0

// PS Period
#define PS_PERIOD_10ms    0x00   // More current
#define PS_PERIOD_20ms 	  0x40
#define PS_PERIOD_40ms    0x80
#define PS_PERIOD_80ms    0xC0

// PS Smart persistence

#define SMART_PERS  0X10
// PS Integration time 
#define PS_IT_1T    0x00
#define PS_IT_2T    0x40
#define PS_IT_4T    0x80	
#define PS_IT_8T    0xC0

// PS ITB 
#define PS_ITB_25us   0x00
#define PS_ITB_50us   0X08    // 50 us better sensitvity

//   PS Persistence
#define PERS_1    0x00
#define PERS_2    0x10
#define PERS_3    0x20
#define PERS_4    0x30

// Interruption
#define INT_DIS   0x00
#define LOGIC_LH  0x04
#define FIRST_H   0x08
#define INT_EN    0x0C

// Low power
#define LPEN_DIS 0x00
#define LPEN_EN  0x01

// Low power period (ms)
#define LPPER_40   0x00
#define LPPER_80   0x02
#define LPPER_160  0x04
#define LPPER_320  0x06

class VCNL36826S
   {
   private:
       void write16b(uint8_t address, uint8_t low, uint8_t high);
       uint16_t readData(uint8_t command_code);
       uint8_t readData2(uint8_t command_code);
       void writeToCommand(uint8_t _cmdCode, uint16_t value);
       uint8_t _i2caddr;
       TwoWire *_wire;

   public:
       VCNL36826S();
       //~VCNL36826S();
       boolean begin(bool resetToDefault = false);
       boolean isConnected();
       boolean exists();
       boolean initparam();
       boolean power_off();
       boolean lowPower(int);
       boolean reset();
       uint16_t readProximity(void);
       uint8_t readInt(void);
       
   };
#endif