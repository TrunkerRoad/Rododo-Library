/* SenseAirS8 Module Library for Arduino 

MIT license
written by Rododo ,Taiwan. http://www.facebook.com/rododo.farm/

Designed for LASS(location aware sensing system)
*/


#ifndef SENSEAIRS8_H
#define SENSEAIRS8_H
#define TimeOut 100

#include "CRC16.h"


class senseAirCO2
{
public:
  senseAirCO2(void);
  uint16_t getValue(void);
  void ABCcalibrate(void);

private:
  CRC _mycrc;
  boolean _readComplete = false;  // whether the string is complete
  byte _message[8] = {0xFE, 0x04, 0x00, 0x00, 0x00, 0x04, 0xE5, 0xC6};
	 byte _incomeByte[13];
	 byte _readcount;
	 void resetBuffer(void);
  void serialRead(void);
};
#endif



senseAirCO2::senseAirCO2(void){
	CO2S.begin(9600);
}

uint16_t senseAirCO2::getValue(void){
    resetBuffer();
    delay(1);
    CO2S.write(_message, sizeof(_message));
    CO2S.flush();
    serialRead();
    unsigned short crc;
    crc =_mycrc.CRC16(_incomeByte,13);
    uint16_t co2;
    if(crc != 0){  
    	co2=0;
    } else {
    	co2 =((unsigned int)_incomeByte[9]<<8) +(unsigned int)_incomeByte[10];
    }
    return co2;
}

void senseAirCO2::ABCcalibrate(void){
  byte _message2[] = {0xFE, 0x06, 0x00, 0x00, 0x00, 0x00, 0x9D, 0xC5};
  CO2S.write(_message2, sizeof(_message2));
  CO2S.flush();
  byte _message3[]= {0xFE, 0x06, 0x00, 0x01, 0x7C, 0x06, 0x6C, 0xC7};
  CO2S.write(_message3, sizeof(_message3));
  CO2S.flush();
  delay(2500);
}


void senseAirCO2::resetBuffer(void){
  memset(_incomeByte,0,sizeof(_incomeByte));
  _readcount=0;
  _readComplete = false;
}

void senseAirCO2::serialRead(void) {
	long startTime = millis();
  while(true){
    if(CO2S.available()) {
      byte inChar = (byte)CO2S.read();
      _incomeByte[_readcount] = inChar;
      _readcount++;
      if (_readcount==13) {
        _readComplete = true;
        break;
      }
      if ((millis()-startTime)>TimeOut){
      	break;
      }
    }
  }
}