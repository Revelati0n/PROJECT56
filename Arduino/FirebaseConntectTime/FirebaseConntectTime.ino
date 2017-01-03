#include <Arduino.h>
//#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>
#include <LiquidCrystal_I2C.h>
#include <TaskScheduler.h>
#include <Wire.h>

#define FIREBASE_HOST "database-001-43e9a.firebaseio.com"
#define FIREBASE_AUTH "GgUHdhrDLstPgYTweOXGMr8ICpNuXMiNsoGGcEkA"
#define WIFI_SSID "lol"
#define WIFI_PASSWORD "021732273"
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void CallbackTime();
void CallbackSwitch();

Task TaskTime(5000, TASK_FOREVER, &CallbackTime);
Task TaskSwitch(500, TASK_FOREVER, &CallbackSwitch);

Scheduler runner;

void CallbackTime() {
    DynamicJsonBuffer jsonBuffer;
    HTTPClient http;
    http.begin("http://192.168.1.37/time.php");
    int httpCode = http.GET();
        if(httpCode > 0) {
            if(httpCode == HTTP_CODE_OK) {
                JsonObject& root = jsonBuffer.parseObject(http.getString());
                String DayOfWeek = root[String("DayOfWeek")];
                String Hour = root[String("Hour")];
                String Minute = root[String("Minute")];
                lcd.setCursor(0, 0);
                lcd.print(DayOfWeek + ":" + Hour + ":" + Minute);
            }
        }
    http.end();
}

void CallbackSwitch() {
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
                lcd.setCursor(0, 0);
                lcd.print(DayOfWeek + ":" + Hour + ":" + Minute);
                Serial.println(DayOfWeek + ":" + Hour + ":" + Minute);
                bool isON = Firebase.getBool("Timers/"+ Hour + ":" + Minute +"/Active");
                if (!Firebase.failed()) {
                    Serial.println(isON);
                    if(isON){
                        lcd.setCursor(0, 1);
                        lcd.print("123456");
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
}

void setup() {
    Serial.begin(9600);
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
    runner.init();
    runner.addTask(TaskSwitch);
    TaskSwitch.enable();
}
void loop() {
    runner.execute();
}
