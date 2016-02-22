#include <CO-001.h>

CO001 myCo;
void setup() {
  // put your setup code here, to run once:
Wire.begin();  // Control this on your own
}

void loop() {
  // put your main code here, to run repeatedly:
int sense =myCo.getValue();
Serial.print("CO: ");
Serial.print(sense);
Serial.println(" ppm");
delay(1500);           //delay between pull is 1500ms
}
