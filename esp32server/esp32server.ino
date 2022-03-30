#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>

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

String Domain = "loadoutput.dns.army";
String Token = "nDZTHRftjo29LTZKtUq7jGk1bTdAxM";

// Web Server
WebServer server(80);

String IpAddress2String(const IPAddress& ipAddress) {
    return String(ipAddress[0]) + String(".") +
           String(ipAddress[1]) + String(".") +
           String(ipAddress[2]) + String(".") +
           String(ipAddress[3]);
}

void setup(void) {
  // Init WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  // Init DDNS
  HTTPClient http;
  String ip = IpAddress2String(WiFi.localIP());;
  String url = String("http://dynv6.com/api/update?hostname=") + Domain + "&token=" + Token + "&ipv4=" + ip;
  http.begin(url);
  int httpCode = http.GET(); //Make the request
  String payload = http.getString();
  http.end(); //Free the resources

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

  server.on(UriBraces("/test"), []() {
    String ip = IpAddress2String(WiFi.localIP());;
    String url = String("http://dynv6.com/api/update?hostname=") + Domain + "&token=" + Token + "&ipv4=" + ip;
    server.send(200, "text/plain", url);
  });

  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
