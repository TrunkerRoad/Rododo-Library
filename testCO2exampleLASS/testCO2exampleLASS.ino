/* SenseAirS8 Module Example for Arduino 

MIT license
written by Rododo ,Taiwan. http://www.facebook.com/rododo.farm/

Designed for LASS(location aware sensing system)
*/
#include <Wire.h>
#include <SHT2x.h>
//NOTICE:DO NOT CHANGE ORDER
#include <WiFi.h>
#include "Adafruit_MCP9808.h"
#include <PowerManagement.h>

char ssid[] = "Microwind_TWN";     // your network SSID (name)
char pass[] = "0919734011";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status


Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();

#include <SoftwareSerial.h>
SoftwareSerial Serial1(0, 1); // RX, TX

#define USE_SOFTWARESERIAL
#include <senseairs8.h>
senseAirCO2 co2sensor(&Serial1);



WiFiClient client;
char server[] = "192.168.1.20";    // name address for Google (using DNS)



void readpm25(int *PM1 = 0, int *PM25 = 0, int *PM10 = 0){
  
     // based on the LASS codes: https://github.com/LinkItONEDevGroup/LASS/blob/master/Device_LinkItOne/LASS/LASS.ino
      unsigned long timeout = millis();
      int count=0;
      byte incomeByte[24];
      boolean startcount=false;
      byte data;
      while (1){
        if((millis() - timeout) > 1200) {    
          Serial.println("[G3-ERROR-TIMEOUT]");
          break;
        }
        if(Serial1.available()){
          data=Serial1.read();
          //Serial.print(data,HEX);
          if(data==0x42 && !startcount){
            startcount = true;
            count++;
            incomeByte[0]=data;
          } else if (startcount){
            count++;
            incomeByte[count-1]=data;
            if(count>=24) {break;}
          }
        }
      }
      unsigned int calcsum = 0; // BM
      unsigned int exptsum;
      for(int i = 0; i < 22; i++) {
        calcsum += (unsigned int)incomeByte[i];
      }
    
      exptsum = ((unsigned int)incomeByte[22] << 8) + (unsigned int)incomeByte[23];
      if(calcsum == exptsum) {
        *PM1 = ((unsigned int)incomeByte[10] << 8) + (unsigned int)incomeByte[11];
        *PM25 = ((unsigned int)incomeByte[12] << 8) + (unsigned int)incomeByte[13];
        *PM10 = ((unsigned int)incomeByte[14] << 8) + (unsigned int)incomeByte[15];
      } else {
        Serial.println("#[exception] PM2.5 Sensor CHECKSUM ERROR!");
        *PM1 = -1;
        *PM25 = -1;
        *PM10 = -1;
      }  
}

void setup() {
  // put your setup code here, to run once:
 Wire.begin();
 /*
  if (!tempsensor.begin()) {
    Serial.println("Couldn't find MCP9808!");
  }*/
  
 Serial.begin(115200);
 Serial1.begin(9600);
   // attempt to connect to Wifi network:
  if (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
    
    // wait 10 seconds for connection:
    delay(7000);
  }
 
 pinMode(5,OUTPUT);
 pinMode(9,OUTPUT);
 pinMode(3,OUTPUT);
 digitalWrite(5,LOW);
 digitalWrite(3,HIGH);
 digitalWrite(9,HIGH);
 PowerManagement.sleep();
 delay(3000);
}


void clearbuffer(){
  while(Serial1.available())
  Serial1.read();
}


void loop() {
  // put your main code here, to run repeatedly:
 
 digitalWrite(5,LOW);
 digitalWrite(3,HIGH);
 delay(10);
 Serial.print("CO2 : ");
 uint16_t ab=co2sensor.getValue();
 Serial.print(ab);
 Serial.println(" ppm");
 digitalWrite(5,LOW);
 digitalWrite(3,LOW);
 delay(10);
 //Serial1.print("dim=70");
 //endcmd();
 //delay(3000);
 //Serial1.print("dim=10");
 //endcmd();
 //digitalWrite(9,HIGH);
 //delay(10000);
 clearbuffer();
 digitalWrite(5,HIGH);
 digitalWrite(3,LOW);
 delay(10);
 int pm25,pm1,pm10;
 readpm25(&pm1,&pm25,&pm10);
 Serial.print("PM2.5:");
 Serial.println(pm25);
 //digitalWrite(9,LOW);
 digitalWrite(5,LOW);
 digitalWrite(3,LOW);
 //tempsensor.shutdown_wake(0);   // Don't remove this line! required before reading temp
  // Read and print out the temperature, then convert to *F
 //float c = tempsensor.readTempC();
 //Serial1.print("CH5.txt=\"");
 //Serial1.print(c,1);
 //Serial1.print(" C\"");
 //endcmd();
 //Serial1.flush();
 delay(250);
 //tempsensor.shutdown_wake(1);   
 Serial1.print("CH3.txt=\"");
 Serial1.print((int)ab);
 Serial1.print("\"");
 endcmd();
 Serial1.flush();
 
 Serial1.print("CH1.txt=\"");
 Serial1.print(SHT2x.GetTemperature(),1);
 Serial1.print(" C\"");
 endcmd();
 Serial1.flush();
 
 Serial1.print("CH2.txt=\"");
 Serial1.print((int)SHT2x.GetHumidity());
 Serial1.print("%\"");
 endcmd();
 Serial1.flush();
 
 Serial1.print("CH4.txt=\"");
 Serial1.print(pm25);
 Serial1.print("\"");
 endcmd();
 Serial1.flush();
 delay(10);

 Serial.print("You're connected to the network");
 printCurrentNet();
 printWifiData();
 delay(3000);

/*
   if (client.connect(server, 8080)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET / HTTP/1.1");
    client.println("Host: http://192.168.1.20");
    client.println("Connection: close");
    client.println();
  }

   while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop();
    delay(30000);
  }
  */
}


void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

}


void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}


void endcmd(){
 Serial1.write(0xFF);
 Serial1.write(0xFF);
 Serial1.write(0XFF);
}

