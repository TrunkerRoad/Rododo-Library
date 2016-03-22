#include <Wire.h>
#include <AM2320.h>
AM2320 th;
void setup() {
  Wire.begin();
  Serial.begin(115200);
}

void loop() {
  th.update();
  //th.update() //for some board like linkitone ....should update twice....maybe timing issue
  Serial.println("---------------");
  Serial.print("temperature: ");
  Serial.print(th.getTemperature(),1);
  Serial.print("  || humidity: ");
  Serial.print(th.getHumidity(),1);
  Serial.print("%");
  Serial.print("  CRC:");
  if(th.CRCCheck()){
    Serial.println("OK");
  } else {
    Serial.println("Failed");
  }
  delay(2000);
}