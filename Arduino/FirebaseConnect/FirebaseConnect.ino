#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define FIREBASE_HOST "database-001-43e9a.firebaseio.com"
#define FIREBASE_AUTH "GgUHdhrDLstPgYTweOXGMr8ICpNuXMiNsoGGcEkA"
#define WIFI_SSID "lol"
#define WIFI_PASSWORD "021732273"
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
    lcd.begin();
    lcd.backlight();
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    lcd.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        lcd.print(".");
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
    }
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("IP Address");
    lcd.setCursor(0, 1);
    lcd.print(WiFi.localIP());
    delay(5000);
    lcd.clear();
    digitalWrite(LED_BUILTIN, LOW);
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
void loop() {
    if(Firebase.getBool("Controllers/SW-1") == true){
        lcd.setCursor(0, 0);
        lcd.print("SW-1 ON");
    }else{
        lcd.setCursor(0, 0);
        lcd.print("SW-1 OFF");
    }
    if(Firebase.getBool("Controllers/SW-2") == true){
        lcd.setCursor(9, 0);
        lcd.print("SW-2 ON");
    }else{
        lcd.setCursor(8, 0);
        lcd.print("SW-2 OFF");
    }
    if(Firebase.getBool("Controllers/SW-3") == true){
        lcd.setCursor(0, 1);
        lcd.print("SW-3 ON");
    }else{
        lcd.setCursor(0, 1);
        lcd.print("SW-3 OFF");
    }
    if(Firebase.getBool("Controllers/SW-4") == true){
        lcd.setCursor(9, 1);
        lcd.print("SW-4 ON");
    }else{
        lcd.setCursor(8, 1);
        lcd.print("SW-4 OFF");
    }
    delay(500);
    lcd.clear();
}

