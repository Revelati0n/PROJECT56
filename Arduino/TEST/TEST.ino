#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "database-001-43e9a.firebaseio.com"
#define FIREBASE_AUTH "TwP7laX2ZV8xBa3bbAoPNb1JHgTl1ppywjuK29NX"
#define WIFI_SSID "lol"
#define WIFI_PASSWORD "021732273"

int SW_PIN1 = 5; //D1
int SW_PIN2 = 4; //D2
int SW_PIN3 = 0; //D3
int SW_PIN4 = 2; //D4

int IN_PIN1 = 3; //D9
int IN_PIN2 = 1; //D10

void setup() {
    Serial.begin(9600);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(SW_PIN1, OUTPUT);
    pinMode(SW_PIN2, OUTPUT);
    pinMode(SW_PIN3, OUTPUT);
    pinMode(SW_PIN4, OUTPUT);
    pinMode(IN_PIN1, INPUT);
    pinMode(IN_PIN2, INPUT);
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
    http.begin("http://192.168.1.36/time.php");
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

    delay(1000);

    if(digitalRead(IN_PIN1) == HIGH){
        Firebase.setBool("Alert/PIN-1", true);
    }else{
        Firebase.setBool("Alert/PIN-1", false);
    }

    if(digitalRead(IN_PIN2) == HIGH){
        Firebase.setBool("Alert/PIN-2", true);
    }else{
        Firebase.setBool("Alert/PIN-2", false);
    }

    if(Firebase.getBool("Controllers/SW-1") == true){
      digitalWrite(SW_PIN1, LOW);
    }else{
      digitalWrite(SW_PIN1, HIGH);
    }
    if(Firebase.getBool("Controllers/SW-2") == true){
      digitalWrite(SW_PIN2, LOW);
    }else{
      digitalWrite(SW_PIN2, HIGH);
    }
    if(Firebase.getBool("Controllers/SW-3") == true){
      digitalWrite(SW_PIN3, LOW);
    }else{
      digitalWrite(SW_PIN3, HIGH);
    }
    if(Firebase.getBool("Controllers/SW-4") == true){
      digitalWrite(SW_PIN4, LOW);
    }else{
      digitalWrite(SW_PIN4, HIGH);
    }

    delay(1000);

}
