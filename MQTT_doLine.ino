#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include "DHT.h"
// #include <TimerOne.h>

#define DHTPIN 2     // what pin we're connected to
#define wifi_ssid "Thanhcoder"
#define wifi_password "88888888"

#define mqtt_server "103.139.155.86"  // MQTT Cloud address
#define humidity_topic "thanh_humidity"
#define temperature_topic "thanh_temperature"

#define DHTTYPE DHT11   // DHT 11

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);
//------------------------------------------------------
int mot1=16;
int mot2=5;
int mot3=4;
int mot4=0;
 
int left=13;
int right=12;

int Left=0;
int Right=0;

void LEFT (void);
void RIGHT (void);
void STOP (void);
void control_car(void);
void setup_pin()
{
  pinMode(mot1,OUTPUT);
  pinMode(mot2,OUTPUT);
  pinMode(mot3,OUTPUT);
  pinMode(mot4,OUTPUT); 

  pinMode(left,INPUT);
  pinMode(right,INPUT);

  digitalWrite(left,HIGH);
  digitalWrite(right,HIGH);

  // analogWrite(mot1,100);
  // analogWrite(mot2,0);
  // analogWrite(mot3,100);
  // analogWrite(mot4,0);
}
//-----------------------------------------

void setup() {
    Serial.begin(9600);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    dht.begin();
}

void setup_wifi() {
    delay(10);
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println("connected wifi");
        delay(500);
        Serial.print(".");
    }
}

void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        if (client.connect("nodeMcuDHT11")) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(5000);
        }
    }
}

// bool checkBound(float newValue, float prevValue, float maxDiff) {
//     return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
// }

long lastMsg = 0;
float temp = 0.0;
float hum = 0.0;
float diff = 1.0;
void loop() {
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    long now = millis();
    if (now - lastMsg > 30000) {
        // Wait a few seconds between measurements
        lastMsg = now;

        float newTemp = dht.readTemperature();
        float newHum = dht.readHumidity();
        // if (checkBound(newTemp, temp, diff)) {
            temp = newTemp;
            Serial.print("New temperature:");
            Serial.println(String(temp).c_str());
            client.publish(temperature_topic, String(temp).c_str());
        // }

        // if (checkBound(newHum, hum, diff)) {
            hum = newHum;
            Serial.print("New humidity:");
            Serial.println(String(hum).c_str());
            client.publish(humidity_topic, String(hum).c_str());
        // }
    }
    Left=digitalRead(left);
    Right=digitalRead(right);
    
  if(Right==0 && Left==0){
    analogWrite(mot1,100);
    analogWrite(mot2,0);
    analogWrite(mot3,100);
    analogWrite(mot4,0);
  }    
  else if(Left==1 && Right==0)
    LEFT();
  else if(Right==1 && Left==0)
    RIGHT();
    
}
//----------------------------------------------------------------------------
void LEFT (void)
{
  analogWrite(mot3,0);
  analogWrite(mot4,0);
  while(Left==1)
  {
    Left=digitalRead(left);
    Right=digitalRead(right);
    if(Right==1)
    {
      int lprev=Left;
      int rprev=Right;
      STOP();
      while((lprev==Left)&&(rprev==Right))
      {
        Left=digitalRead(left);
        Right=digitalRead(right);
      }
    }
    analogWrite(mot1,255);
    analogWrite(mot2,0);
  }
  analogWrite(mot1,100);
  analogWrite(mot2,0);
  analogWrite(mot3,100);
  analogWrite(mot4,0);
}

void RIGHT (void)
{
  analogWrite(mot1,0);
  analogWrite(mot2,0);
  while(Right==1)
  {
    Left=digitalRead(left);
    Right=digitalRead(right);
    if(Left==1)
    {
      int lprev=Left;
      int rprev=Right;
      STOP();
      while(((lprev==Left)&&(rprev==Right))==1)
      {
        Left=digitalRead(left);
        Right=digitalRead(right);
      }
    }
    analogWrite(mot3,255);
    analogWrite(mot4,0);
  }
  analogWrite(mot3,100);
  analogWrite(mot4,0);
  analogWrite(mot1,100);
  analogWrite(mot2,0);
}
void STOP (void)
{
  analogWrite(mot1,0);
  analogWrite(mot2,0);
  analogWrite(mot3,0);
  analogWrite(mot4,0);
}
void control_car(void)
{
  analogWrite(mot1,100);
  analogWrite(mot2,0);
  analogWrite(mot3,100);
  analogWrite(mot4,0);
  while(1)
  {
    Left=digitalRead(left);
    Right=digitalRead(right);
  if((Left==0 && Right==1)==1)
    LEFT();
  else if((Right==0 && Left==1)==1)
    RIGHT();
  }
}