#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include <DHT.h>
#include <map>
map< char, int > mymap;
#define DHTPIN 2     // what pin we're connected to
#define wifi_ssid "Thanh"
#define wifi_password "11111111"

#define mqtt_server "192.168.43.13"  // MQTT Cloud address
#define humidity_topic "Home/BedRoom/DHT22/Humidity"
#define temperature_topic "Home/BedRoom/DHT22/Temperature"

#define DHTTYPE DHT11   // DHT 11

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    dht.begin();
}

void setup_wifi() {
    delay(10);
    Serial.println("connecting wifi");
    WiFi.begin(wifi_ssid, wifi_password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("connected");
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
    if (now - lastMsg > 3000) {
        // Wait a few seconds between measurements
        lastMsg = now;
         client.publish(temperature_topic, "Thanhpro");
        // float newTemp = dht.readTemperature();
        // float newHum = dht.readHumidity();
        // // if (checkBound(newTemp, temp, diff)) {
        //     temp = newTemp;
        //     Serial.print("New temperature:");
        //     Serial.println(String(temp).c_str());
        //     client.publish(temperature_topic, String(temp).c_str());
        // // }

        // // if (checkBound(newHum, hum, diff)) {
        //     hum = newHum;
        //     Serial.print("New humidity:");
        //     Serial.println(String(hum).c_str());
        //     client.publish(humidity_topic, String(hum).c_str());
        // // }
    }
    
}