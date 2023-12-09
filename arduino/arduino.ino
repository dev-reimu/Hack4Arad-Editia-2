#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "UrbanDev-Ard";
const char *password = "";

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "Hello from Arduino!");
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Set up web server
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
