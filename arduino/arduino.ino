#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

String httpRequest;

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(2, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("UrbanDevCrew Distress Hotspot");

  // Set up web server
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // HTML with IP in DOM
  //String htmlDocument = "";
  //String ipAddress = WiFi.localIP().toString();
  //htmlDocument.replace("IP_ADDRESS", ipAddress);
  Serial.println(WiFi.softAPIP());
  
  digitalWrite(2, HIGH);
  delay(3000);
  digitalWrite(2, LOW);
  
  server.send(200, "text/html", "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><style>body{padding:0;margin:0;display:flex;justify-content:center}#message-container{width:80%;max-width:1200px}#message-container div{background-color:#ccc;padding:5px}#message-container div:nth-child(2n){background-color:#fff}#send-container{position:fixed;padding-bottom:30px;bottom:0;background-color:#fff;max-width:1200px;width:80%;display:flex}#message-input{flex-grow:1}</style></head><body><div id=\"message-container\"></div><form id=\"send-container\"><input type=\"text\" id=\"message-input\"><button type=\"submit\" id=\"send-button\">Send</button></form></body></html>");
}
