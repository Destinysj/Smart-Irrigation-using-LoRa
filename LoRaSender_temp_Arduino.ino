//-----------------------------------------------------------------//
//                      Remote operation of Pumps using LoRa
//-----------------------------------------------------------------//

// 5v
// Gnd
//  LoRa v1.4 chip     Arduno UNO
//  SCK ................D13
//  MISO................D12
//  MOSI................D11
//  NSS.................D10
//  NRESET..............D9
//  D101................ 2 

#include <SPI.h>
#include <LoRa.h> 
#include <DHT.h> // works well with arduino uno but does not with arduino nano
#define DHTPIN 4 // connect DH11/DHT22 to pin 4

#define DHTTYPE DHT22 // change type DHT11 if using DHT11
DHT dht(DHTPIN, DHTTYPE);

int counter_0 = 0;
int counter_1 = 1;
int WATERPUMP_1 = 6;  // relay_1 to pin 6 
int WATERPUMP_2 = 7;  // relay_2 to pin 7
int x = 60;           // threshold value of humidity
void setup() {
  Serial.begin(9600);
  pinMode(7,OUTPUT);  // setting pin 7 as output for water pump
  pinMode(6,OUTPUT);  // setting pin 6 as output for water pump
  digitalWrite(WATERPUMP_1, HIGH); // Initial condition to relay are low
  digitalWrite(WATERPUMP_2, HIGH); // 
  
  while (!Serial);

  Serial.println("LoRa Sender");

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println(F("DHTxx test!"));

  dht.begin();
  
}

void loop() {
  delay(100);
  float t = dht.readTemperature();
  Serial.print(F("%  Temperature: "));  // -------------------------------------------------------------------------
  Serial.println(t);                    //  Reading Temperature values // Read temperature as Celsius (the default)
  Serial.print(F("°C "));               // -------------------------------------------------------------------------
  
  float h = dht.readHumidity();         // ---------------------------
  Serial.print(F("Humidity: "));        // Reading Humidity values
  Serial.print(h);                      //---------------------------
  

  // logic for operating two waterpumps via Relay module
    if ( h>=0 && h<x )
    {
     digitalWrite(WATERPUMP_2,LOW);
     digitalWrite(WATERPUMP_1, HIGH); //turns on the motor at specific range of temp
     Serial.println("PUMP_1 is On");
     LoRa.beginPacket();
     LoRa.print(counter_0);
     LoRa.endPacket();
     delay(1000);
    }
    else if(h>=x && h<=80)
    {
      digitalWrite(WATERPUMP_1,LOW);
      digitalWrite(WATERPUMP_2,HIGH);
      Serial.println("PUMP_2 is On");
      LoRa.beginPacket();
      LoRa.print(counter_1); 
      LoRa.endPacket();
      delay(1000);  
    }
  
  //LoRa.endPacket();

  //counter_1++;

  delay(2000);
}

// problem .1 sensor stopped reading when motor is turned onn???
// siginificance of serial available ??
