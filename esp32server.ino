#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <uri/UriBraces.h>
#include <uri/UriRegex.h>

// WiFi
const char* ssid     = "CSTH-AX";
const char* password = "CSTH636797";

// PWM
const int Freq = 5000;
const int Resolution = 8;

const int LChannel = 0;
const int RChannel = 1;
const int LOut = 13;
const int ROut = 15;
String LKey = "left";
String RKey = "right";
String LValue = "255";
String RValue = "100";

// Web Server
WebServer server(80);

void setup(void) {
  // Init WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Init PWM PIN
  ledcSetup(LChannel, Freq, Resolution);
  ledcSetup(RChannel, Freq, Resolution);

  ledcAttachPin(LOut, LChannel);
  ledcAttachPin(ROut, RChannel);

  ledcWrite(LChannel, LValue.toInt());
  ledcWrite(RChannel, RValue.toInt());

  // Init Web Server
  server.on(UriBraces("/left/{}"), []() {
    LValue = server.pathArg(0);
    ledcWrite(LChannel, LValue.toInt());
    server.send(200, "text/plain", "Left: " + LValue + "/255");
  });

  server.on(UriBraces("/right/{}"), []() {
    RValue = server.pathArg(0);
    ledcWrite(RChannel, RValue.toInt());
    server.send(200, "text/plain", "Right: " + RValue + "/255");
  });

  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
