/*
@file VCNL36826S.cpp

@Author 
Ernesto Ortiz-Perdomo
V 2.1.0
*/

#include "VCNL36826S.h"

VCNL36826S::VCNL36826S()
{
    _i2caddr = VCNL36826S_I2C_ADDR;

    _num = 0x02;

}

uint16_t VCNL36826S::readProximity(void)
{
    return readData(PS_DATA);

}

boolean VCNL36826S::exists()
{
    Wire.begin();
    Wire.beginTransmission(_i2caddr);
    Wire.write(VCNL36826S_ID);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2caddr,_num);
    byte lowByte = Wire.read();
    //byte highByte = Wire.read();
    if(lowByte == 0x26)
    {
        Serial.println("Found!");
        return true;
    }
    else
    {
        return false;
        Serial.println("Device not found.");
    }
}

boolean VCNL36826S::initparam()
{
    uint16_t current_value;
    reset();
    // Configuration register 1
    current_value = readData(CONF_1);  
    current_value = current_value | PS_ON;
    writeToCommand(CONF_1, current_value);  // Power on 
    
    current_value = readData(CONF_1);
    current_value = current_value | (PS_INIT|PS_bit1);
    writeToCommand(CONF_1, current_value);

    // Conf reg 2
    current_value = readData(CONF_2);
    current_value = current_value | PS_ST;
    writeToCommand(CONF_2, current_value);

    // read proximity data

    readData(PS_DATA);

    // VCSEL 6 mA, Sun cancel enabled & Output 16 bits

    current_value = readData(CONF_3);
    current_value = current_value | PS_AF | ((VCSEL_I_6mA|PS_HD_16)<< 8); //|SC_EN

    // PS period = 80 ms, PS Integration time 8T,  
    current_value = readData(CONF_2);
    current_value = current_value | PS_PERIOD_80ms | (PS_IT_8T<<8) |(PS_ITB_50us<<8)|(MPS8<<8);
    current_value = current_value | PERS_4 | FIRST_H| SMART_PERS;

    writeToCommand(CONF_2,current_value);

    current_value = readData(AC_LPPERI);
    current_value = current_value | AC_T24ms | AC_NUM4 |AC_EN|AC_TRIGGER_EN|AC_INT_EN;
    writeToCommand(AC_LPPERI, current_value);

    return true;
}
boolean VCNL36826S::setthreshold(uint16_t upper_t,uint16_t lower_t)
{
    writeToCommand(THDL,lower_t);
    writeToCommand(THDH,upper_t);
    return true;
}

boolean VCNL36826S::reset()
{
    writeToCommand(CONF_1,0x01);
    writeToCommand(CONF_2,0x00);
    writeToCommand(CONF_3,0x00);
    writeToCommand(THDH,0x00);
    writeToCommand(THDL,0x00);
    writeToCommand(AC_LPPERI,0x00);
    return true;
}
boolean VCNL36826S::lowPower()
{
    uint16_t current_value;
    current_value = readData(AC_LPPERI);
    current_value = current_value | ((LPEN_EN | LPPER_320 ) << 8);
    writeToCommand(AC_LPPERI, current_value);
    //Serial.println("Entering low power mode!");
    return true;
}
boolean VCNL36826S::power_off()
{
    reset();
    Serial.println("Device off");
    return true;
}

uint8_t VCNL36826S::readInt()
{
    uint8_t inter = readData2(INT_Flag);
    return inter;
}
/* _______________________________________________________
    @brief I2C Interface
*/ 
uint16_t VCNL36826S::readData(uint8_t command)
{
    Wire.begin();
    Wire.beginTransmission(_i2caddr);
    Wire.write(command);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2caddr,_num);
    /*if(Wire.available() != 0x02);
    {
        Serial.println("No hay respuesta  ");
        return;
    }*/
    byte Lbyte = Wire.read();
    byte Hbyte = Wire.read();
    uint16_t value = (Hbyte << 8)| Lbyte;
    return value;
}
uint8_t VCNL36826S::readData2(uint8_t command)
{
    Wire.begin();
    Wire.beginTransmission(_i2caddr);
    Wire.write(command);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2caddr,_num);
    
    byte Lbyte = Wire.read();  // Not used
    byte Hbyte = Wire.read();  // Int flag
    return Hbyte;
}

void VCNL36826S::write16b(uint8_t reg, uint8_t Lbyte, uint8_t Hbyte)
{
    Wire.beginTransmission(_i2caddr);
    Wire.write(reg);
    Wire.write(Lbyte);
    Wire.write(Hbyte);
    Wire.endTransmission();
}

void VCNL36826S::writeToCommand(uint8_t _cmdCode, uint16_t value)
{
    Wire.beginTransmission(_i2caddr);
    Wire.write(_cmdCode);
    Wire.write(uint8_t(value & 0xff));  // Low byte
    Wire.write(uint8_t(value >> 8));   // High byte
    Wire.endTransmission();
}

boolean VCNL36826S::exists()
{
    Wire.begin();
    Wire.beginTransmission(_i2caddr);
    Wire.write(VCNL36826S_ID);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2caddr,0x02);
    byte lowByte = Wire.read();
    //byte highByte = Wire.read();
    if(lowByte == 0x26)
    {
        Serial.println("Found!");
        return true;
    }
    else
    {
        return false;
        Serial.println("Device not found.");
    }
}

boolean VCNL36826S::initparam()
{
    uint16_t current_value;
    reset();
    // Configuration register 1
    current_value = readData(CONF_1);  
    current_value = current_value | PS_ON;
    writeToCommand(CONF_1, current_value);  // Power on 
    
    current_value = readData(CONF_1);
    current_value = current_value | (PS_INIT|PS_bit1);
    writeToCommand(CONF_1, current_value);

    // Conf reg 2

    current_value = readData(CONF_2);
    current_value = current_value | PS_ST;
    writeToCommand(CONF_2, current_value);

    // read proximity data

    readData(PS_DATA);

    // VCSEL 6 mA, Sun cancel enabled & Output 16 bits

    current_value = readData(CONF_3);
    current_value = current_value | ((VCSEL_I_6mA|PS_HD_16|SC_EN)<< 8); 

    // PS period = 80 ms, PS Integration time 8T,  
    current_value = readData(CONF_2);
    current_value = current_value | PS_PERIOD_20ms | (PS_IT_8T<<8) |(PS_ITB_50us);

    current_value = current_value | PERS_2 | LOGIC_LH | SMART_PERS;

    writeToCommand(CONF_2,current_value);

    // Treshold 
    writeToCommand(THDL,0x190);
    writeToCommand(THDH,0x1F4);
    return true;
}
boolean VCNL36826S::reset()
{
    writeToCommand(CONF_1,0x01);
    writeToCommand(CONF_2,0x00);
    writeToCommand(CONF_3,0x00);
    writeToCommand(THDH,0x00);
    writeToCommand(THDL,0x00);
    writeToCommand(AC_LPPERI,0x00);
    return true;
}
boolean VCNL36826S::lowPower(int val)
{
    uint16_t current_value;
    if(val == 1)
    {
    current_value = readData(AC_LPPERI);
    current_value = current_value | ((LPEN_EN | LPPER_320 ) << 8);
    writeToCommand(AC_LPPERI,current_value);
    Serial.println("Entering low power mode!");
    
    }
    else{
        current_value = readData(AC_LPPERI);
        current_value = current_value | (LPEN_DIS <<8 );
        Serial.println("Normal Power");
    }
}
boolean VCNL36826S::power_off()
{
    reset();
    Serial.println("Device off");
}

uint8_t VCNL36826S::readInt()
{
    uint8_t inter = readData2(INT_Flag);
    return inter;
}
/* _______________________________________________________
    @brief I2C Interface
*/ 
uint16_t VCNL36826S::readData(uint8_t command)
{
    Wire.begin();
    Wire.beginTransmission(_i2caddr);
    Wire.write(command);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2caddr,0x02);
    /*if(Wire.available() != 0x02);
    {
        Serial.println("No hay respuesta  ");
        return;
    }*/
    byte Lbyte = Wire.read();
    byte Hbyte = Wire.read();
    uint16_t value = (Hbyte << 8)| Lbyte;
    return value;
}
uint8_t VCNL36826S::readData2(uint8_t command)
{
    Wire.begin();
    Wire.beginTransmission(_i2caddr);
    Wire.write(command);
    Wire.endTransmission(false);
    Wire.requestFrom(_i2caddr,0x02);
    byte Lbyte = Wire.read();  // Not used
    byte Hbyte = Wire.read();  // Int flag
    return Hbyte;

}

void VCNL36826S::write16b(uint8_t reg, uint8_t Lbyte, uint8_t Hbyte)
{
    Wire.beginTransmission(_i2caddr);
    Wire.write(reg);
    Wire.write(Lbyte);
    Wire.write(Hbyte);
    Wire.endTransmission();
}

void VCNL36826S::writeToCommand(uint8_t _cmdCode, uint16_t value)
{
    Wire.beginTransmission(_i2caddr);
    Wire.write(_cmdCode);
    Wire.write(uint8_t(value & 0xff));  // Low byte
    Wire.write(uint8_t(value >> 80));   // High byte
    Wire.endTransmission();
}