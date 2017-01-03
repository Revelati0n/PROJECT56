#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
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
void loop() {
    DynamicJsonBuffer jsonBuffer;
    HTTPClient http;
    // http.begin("http://www.venusz.tk/");
    http.begin("http://192.168.1.37/time.php");
    int httpCode = http.GET();
        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                JsonObject& root = jsonBuffer.parseObject(http.getString());
                String H = root[String("DayOfWeek")];
                String m = root[String("Hour")];
                String s = root[String("Minute")];
                lcd.setCursor(0, 0);
                lcd.print(H + ":" + m + ":" + s);
            }
        }
    http.end();
    delay(500);
}
