/* AECL_CO001_M Module Testing Code for Arduino 

MIT license
written by Rododo ,Taiwan. http://www.facebook.com/rododo.farm/

Designed for LASS(location aware sensing system)
*/


#include <CO-001.h>

CO001 myCo;
void setup() {
  // put your setup code here, to run once:
Wire.begin();  // Control this on your own
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
int sense =myCo.getValue();
Serial.print("CO: ");
Serial.print(sense);
Serial.println(" ppm");
delay(1500);           //delay between pull is 1500ms
}
