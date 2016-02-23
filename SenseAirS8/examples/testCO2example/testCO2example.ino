/* SenseAirS8 Module Example for Arduino 

MIT license
written by Rododo ,Taiwan. http://www.facebook.com/rododo.farm/

Designed for LASS(location aware sensing system)
*/

//NOTICE:DO NOT CHANGE ORDER
#include <SoftwareSerial.h>
SoftwareSerial Serial1(0, 1); // RX, TX
#define CO2S Serial1  //Select Serial'N here!
#include <senseairs8.h>
//END OF NOTICE

senseAirCO2 co2sensor;

void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
 Serial.print("CO2 : ");
 Serial.print(co2sensor.getValue());
 Serial.println(" ppm");
 delay(1000);
}
