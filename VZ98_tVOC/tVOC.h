/* AECL_VZ89 Module Library for Arduino 

MIT license
written by Rododo ,Taiwan. http://www.facebook.com/rododo.farm/

Designed for LASS(location aware sensing system)
*/


#ifndef AECL_TVOC_H
#define AECL_TVOC_H
#include <inttypes.h>
#include <Wire.h>
#define MAX_RETRYCOUNT 30

struct field
{uint16_t CO2e;
 uint16_t rVOC;
 uint16_t tVOC;
 uint32_t rawRes;
};


class tVOC
{
public:
	tVOC(){};
  boolean update(void);
  boolean calibrate(uint16_t CO2e);
  field getValue;
};
#endif

boolean tVOC::calibrate(uint16_t CO2e){
  //did not carry out due to professional issues.
}


boolean tVOC::update(){
    Wire.beginTransmission(0x70);
    Wire.write(0x09);
    if (Wire.endTransmission(false) != 0) {
      return 0;
    }
    Wire.requestFrom(0x70, 6);
    int testtry;
    while(Wire.available() < 6) {
      delay(1);
      if(testtry++ >=MAX_RETRYCOUNT){
       return 0; 
      }
    }
    getValue.CO2e = Wire.read();
    getValue.rVOC = Wire.read();  
    getValue.tVOC = Wire.read();
    getValue.rawRes = Wire.read();
    getValue.rawRes =getValue.rawRes + (long)(256*Wire.read());
    getValue.rawRes =getValue.rawRes + (long)(65536*Wire.read());
    getValue.rawRes =getValue.rawRes *10;
    getValue.CO2e = (uint16_t)((getValue.CO2e-13)*(1600/229)+400);
    getValue.tVOC = (uint16_t)(getValue.tVOC-13) * (1000/229);
    return 1;
}