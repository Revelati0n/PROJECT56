#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "database-001-43e9a.firebaseio.com"
#define FIREBASE_AUTH "TwP7laX2ZV8xBa3bbAoPNb1JHgTl1ppywjuK29NX"
#define WIFI_SSID "lol"
#define WIFI_PASSWORD "021732273"

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(14, OUTPUT);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
    }
    digitalWrite(LED_BUILTIN, LOW);
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
void loop() {
    DynamicJsonBuffer jsonBuffer;
    HTTPClient http;
    http.begin("http://192.168.1.35/time.php");
    int httpCode = http.GET();
        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                JsonObject& root = jsonBuffer.parseObject(http.getString());
                String DayOfWeek = root[String("DayOfWeek")];
                String Hour = root[String("Hour")];
                String Minute = root[String("Minute")];
                Serial.println(DayOfWeek + ":" + Hour + ":" + Minute);
                bool isON = Firebase.getBool("Timers/"+ Hour + ":" + Minute +"/Active");
                if (Firebase.success()) {
                    if(isON){
                        Serial.println("ON");
                        Firebase.setBool("Controllers/SW-1", Firebase.getBool("Timers/"+ Hour + ":" + Minute +"/SW/0"));
                        delay(200);
                        Firebase.setBool("Controllers/SW-2", Firebase.getBool("Timers/"+ Hour + ":" + Minute +"/SW/1"));
                        delay(200);
                        Firebase.setBool("Controllers/SW-3", Firebase.getBool("Timers/"+ Hour + ":" + Minute +"/SW/2"));
                        delay(200);
                        Firebase.setBool("Controllers/SW-4", Firebase.getBool("Timers/"+ Hour + ":" + Minute +"/SW/3"));
                        delay(200);
                    }
                }
            }
        }
    http.end();
    delay(2000);
    if(Firebase.getBool("Controllers/SW-1") == true){
      digitalWrite(12, LOW);
    }else{
      digitalWrite(12, HIGH);
    }
    if(Firebase.getBool("Controllers/SW-2") == true){
      digitalWrite(14, LOW);
    }else{
      digitalWrite(14, HIGH);
    }
    delay(2000);
}

