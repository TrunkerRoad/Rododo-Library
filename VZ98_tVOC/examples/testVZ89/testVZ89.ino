/* AECL_VZ89 Module Testing Code for Arduino 

MIT license
written by Rododo ,Taiwan. http://www.facebook.com/rododo.farm/

Designed for LASS(location aware sensing system)
*/


#include <tVOC.h>

tVOC tVOCsensor;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(tVOCsensor.update()){
    Serial.print("CO2e: ");
    Serial.print(tVOCsensor.getValue.CO2e);
    Serial.print(" voc(short): ");
    Serial.print(tVOCsensor.getValue.rVOC);
    Serial.print(" tVOC(long): ");  //use this value for tVOC;
    Serial.print(tVOCsensor.getValue.tVOC);
    Serial.print(" rawRes: ");
    Serial.println(tVOCsensor.getValue.rawRes);
    delay(2000); //this is delay between polls
  } else {
    Serial.print("Cant read sensor!");
  }
}
