#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include "dht.h"

#define MQ2pin 2 // A5
#define Atomizer 0 // A3
#define AirPIN 1 // A1

float sensorValue;  //variable to store sensor value
float atomizerValue;
float airValue;
const int dht_apin = A3; // Analog Pin sensor is connected to A3
char ssid[] = "IoTTestNetwork"; // Network name
const char password [] = "CENSOREDPASSWORD"; // PWD
const unsigned long channel_id = 1587209; // CH ID
const char write_api_key[] = "CENSOREDAPIKEY";// API key LOOK INNTO API KEYS TAB (Gonna keep it hidden)

dht DHT; // Initalize dht to DHT
WiFiClient client; // Connect to to a specified internet IP address

void setup() {
  // Setting up connection
  Serial.begin(9600);
  delay(100);
  Serial.print("Connecting to " + String(ssid));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.println("Attempting to Connect to " + String(ssid) + " .");
    delay(100);
  }
  Serial.println();
  Serial.println("Connected!");
  ThingSpeak.begin(client);
  
  }

void loop() {
  
  sensorValue = analogRead(MQ2pin); // read analog input pin 2
  airValue = analogRead(AirPIN); // read analog input pin 1
  DHT.read22(dht_apin);
  
  Serial.println("Posting " + String(DHT.humidity, 2) + "% to ThingSpeak");
  ThingSpeak.setField(1, String(DHT.humidity, 2));
  
  Serial.println("Posting " + String(DHT.temperature, 2) + " to ThingSpeak");
  ThingSpeak.setField(2, String(DHT.temperature, 2));
  
  Serial.println("Posting " + String(sensorValue, 2) + " to ThingSpeak");
  ThingSpeak.setField(3, String(sensorValue, 2));

// GROVE START IF STATEMENT
  if(DHT.humidity>=50){ //if the humidity % is equal or above the setpoint 51% the atomizer turn ON (HIGH)
    digitalWrite(Atomizer,HIGH); // HIGH = ON
    Serial.print("ON \n");
    Serial.println("Posting " + String(atomizerValue, 1) + " to ThingSpeak");
    ThingSpeak.setField(4, String(atomizerValue, 2));
    }

    else if(DHT.humidity<=30){ 
    digitalWrite(Atomizer,HIGH);
    Serial.print("ON \n");
    Serial.println("Posting " + String(atomizerValue = 1) + " to ThingSpeak");
    ThingSpeak.setField(4, String(atomizerValue, 2));
    }

    else if(DHT.humidity){
      digitalWrite(Atomizer,LOW); // LOW = OFF
      Serial.print("OFF \n");
      Serial.println("Posting " + String(atomizerValue = 0) + " to ThingSpeak");
      ThingSpeak.setField(4, String(atomizerValue, 2));
    }
// GROVE END

  Serial.println("Posting " + String(airValue, 2) + " to ThingSpeak");
  ThingSpeak.setField(5, String(airValue, 2));
  
  ThingSpeak.writeFields(channel_id, write_api_key);
  Serial.println();
  //delay(600000); // 10min delay
  delay(300000); // 5min delay
  //delay(15000); // 15s delay Testing purposes
}