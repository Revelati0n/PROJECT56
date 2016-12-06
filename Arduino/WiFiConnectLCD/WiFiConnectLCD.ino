#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

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
}
int i = 0;
void loop() {
    i++;
    lcd.setCursor(0, 0);
    lcd.print(i);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(250);
}
