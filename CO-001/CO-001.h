/* AECL_CO_001M Module Library for Arduino 

MIT license
written by Rododo ,Taiwan. http://www.facebook.com/rododo.farm/

Designed for LASS(location aware sensing system)
*/

#ifndef AECL_CO_001_H
#define AECL_CO_001_H
#include <inttypes.h>
#include <Wire.h>
#define MAX_RETRYCOUNT 30


class CO001
{
public:
	CO001(){};
	int16_t getValue(void);
};
#endif

int16_t CO001::getValue(void){
	Wire.beginTransmission(0x48);
    Wire.write(0xA0);
    if (Wire.endTransmission(false) != 0) {
      return -1;
    }
    Wire.requestFrom(0x48, 2);
    int testtry;
    while(Wire.available() < 2) {
      delay(1);
      if(testtry++ >=100){
       return -1;
      }
    }
    uint16_t co = Wire.read();
    co = (co << 8) + Wire.read();
    return co;

}