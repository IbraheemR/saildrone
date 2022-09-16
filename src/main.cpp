#include <Arduino.h>
#include <WiFi.h>
#include "DNSServer.h"
#include "SPIFFS.h"

#include "ESPAsyncWebServer.h"
#include "ESP32Servo.h"

AsyncWebServer server(80);

Servo sailServo;
Servo rudderServo;

void setup()
{
  pinMode(22, OUTPUT);
  digitalWrite(22, LOW);

  sailServo.attach(32);
  rudderServo.attach(33);

  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS error!");
    return;
  }

  WiFi.softAP("SAILBOT");
  Serial.println(WiFi.softAPIP());

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

  server.on(
    "/sail",
    HTTP_POST,
    [](AsyncWebServerRequest *request) {},
    NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      int angle = String((const char*) data).toInt();

      sailServo.write(angle);

      request->send(200, "text/plain", "OK");
    });

  server.on(
    "/rudder",
    HTTP_POST,
    [](AsyncWebServerRequest *request) {},
    NULL,
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
      int angle = String((const char*) data).toInt();

      rudderServo.write(angle);
  
      request->send(200, "text/plain", "OK");
    });
  server.begin();
}

void loop()
{
  // put your main code here, to run repeatedly:
}