#include "AM2320.h"
#include <Wire.h>
#define MAXRETRYCOUNT 100
// 
// AM2321 / AM2320 Temperature & Humidity Sensor library for Arduino
//      
// The MIT License (MIT)
//
// Original by: THAKSHAK GUDIMETLA
//
// Modified and Bug corrected by RODODO.FARM
// http://www.facebook.com/rododo.farm

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

AM2320::AM2320(){

}

void AM2320::read_sensor_readings()
{
	long stricttime;
	memset(data_buffer,0,sizeof(data_buffer));    
	int len = 2+ AM2320_read_count +2;// COMMAND + DATA + REGCOUNT + CRCLSB + CRCMSB
	Wire.beginTransmission(AM2320_address);
	stricttime=millis();
	while((millis()-stricttime)<2);
	Wire.endTransmission(false);
	//delay(2);
	stricttime=millis();
	while((millis()-stricttime)<2);
	//
	// Read Command
	//
	Wire.beginTransmission(AM2320_address);
	Wire.write(AM2320_read_sensor_data);
	Wire.write(AM2320_RH_hb);
	Wire.write(AM2320_read_count);
	Wire.endTransmission();
	//
	// Waiting
	//
	//delay(2); //>1.5ms
	stricttime=millis();
	while((millis()-stricttime)<2);
	//
	// Read
	//
	Wire.requestFrom(AM2320_address, len);

	int testtry=0;
	while(Wire.available() < len) {
      delay(1);
      if(testtry++ >=MAXRETRYCOUNT){
       return;
      }
    }

	for (int i = 0; i < len; i++)
	{
	  data_buffer[i] = Wire.read();
	  //	Serial.println(data_buffer[i]);
	  //delay(2); //>1.5ms
	stricttime=millis();
	while((millis()-stricttime)<2);
	}
}

void AM2320::update(){
	read_sensor_readings();
}

float AM2320::getTemperature(void)
{
	temperature  = data_buffer[4] << 8;
	temperature += data_buffer[5];
	return((float)temperature/10);
}

float AM2320::getHumidity(void)
{
	humidity     = data_buffer[2] << 8;
	humidity    += data_buffer[3];
	return ((float)humidity/10);
}
    
unsigned long AM2320::CRC16(uint8_t *ptr, uint8_t length) 
{ 
      unsigned long crc = 0xFFFF; 
      uint8_t s = 0x00; 

      while(length--) 
      { 
        crc ^= *ptr++; 
        for(s = 0; s < 8; s++) 
        { 
          if((crc & 0x01) != 0) 
          { 
            crc >>= 1; 
            crc ^= 0xA001; 
          } 
          else 
          { 
            crc >>= 1; 
          } 
        } 
      } 

      return crc; 
}

bool AM2320::CRCCheck(void)
{
	if (0 == CRC16(data_buffer, 8))
            return true;
        return false;
}
