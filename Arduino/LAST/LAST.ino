#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <DS1302.h>
/*
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;
*/
DS1302 rtc(13, 12, 14);
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
    String Times = rtc.getTimeStr(FORMAT_SHORT);
    bool isON = Firebase.getBool("Timers/"+ Times +"/Active");
    if (Firebase.success()) {
        if(isON){
            Serial.println("IsOn");
            bool isDay = Firebase.getBool("Timers/"+ Times +"/Days/" + String((rtc.getTime().dow  == 7 ? 0 : rtc.getTime().dow)));
            if(isDay){
                Serial.println("IsDay");
                bool isMode = Firebase.getBool("Timers/"+ Times +"/Mode");
                if(isMode){
                    Serial.println("MODE ON");
                    if(Firebase.getBool("Timers/"+ Times +"/SW/0")){
                        Firebase.setBool("Controllers/SW-1", true);
                        delay (100);
                    }
                    if(Firebase.getBool("Timers/"+ Times +"/SW/1")){
                        Firebase.setBool("Controllers/SW-2", true);
                        delay (100);
                    }
                    if(Firebase.getBool("Timers/"+ Times +"/SW/2")){
                        Firebase.setBool("Controllers/SW-3", true);
                        delay (100);
                    }
                    if(Firebase.getBool("Timers/"+ Times +"/SW/3")){
                        Firebase.setBool("Controllers/SW-4", true);
                        delay (100);
                    }
                }else{
                    Serial.println("MODE OFF");
                    if(Firebase.getBool("Timers/"+ Times +"/SW/0")){
                        Firebase.setBool("Controllers/SW-1", false);
                        delay (100);
                    }
                    if(Firebase.getBool("Timers/"+ Times +"/SW/1")){
                        Firebase.setBool("Controllers/SW-2", false);
                        delay (100);
                    }
                    if(Firebase.getBool("Timers/"+ Times +"/SW/2")){
                        Firebase.setBool("Controllers/SW-3", false);
                        delay (100);
                    }
                    if(Firebase.getBool("Timers/"+ Times +"/SW/3")){
                        Firebase.setBool("Controllers/SW-4", false);
                        delay (100);
                    }
                }
            }
        }
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
    delay (500);
}
