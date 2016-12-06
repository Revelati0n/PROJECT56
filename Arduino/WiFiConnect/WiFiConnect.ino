#include <ESP8266WiFi.h>

#define WIFI_SSID "lol"
#define WIFI_PASSWORD "021732273"

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        digitalWrite(LED_BUILTIN, LOW);
        delay(500);
        digitalWrite(LED_BUILTIN, HIGH);
        delay(500);
    }
    digitalWrite(LED_BUILTIN, LOW);
}
void loop() {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
}
