#include <ESP8266WebServer.h>
#include <DNSServer.h> 

const char *ssid = "UrbanDevCrew Distress Hotspot";

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  // Set up web server
  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  Serial.println(WiFi.softAPIP());
  server.send(200, "text/html", "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>UrbanDev Crew</title><style>body {padding: 0;margin: 0;display: flex;justify-content: center;}#message-container {width: 80%;max-width: 1200px;}#message-container div {background-color: #CCC;padding: 5px;}#message-container div:nth-child(2n) {background-color: #FFF;}#send-container {position: fixed;padding-bottom: 30px;bottom: 0;background-color: white;max-width: 1200px;width: 80%;display: flex;}#message-input {flex-grow: 1;}</style></head><body><div id=\"message-container\"></div><form id=\"send-container\"><input type=\"text\" id=\"message-input\"><button type=\"submit\" id=\"send-button\">Send</button></form><script>const name = prompt('What is your name?')const messageContainer = document.getElementById('message-container')const messageForm = document.getElementById('send-container')const messageInput = document.getElementById('message-input')messageForm.addEventListener('submit', e => {e.preventDefault()const message = messageInput.valueappendMessage(You: ${message})messageInput.value = ''return false; // Prevent form submission})function appendMessage(message) {const messageElement = document.createElement('div')messageElement.innerText = messagemessageContainer.append(messageElement)}appendMessage('You joined')</script></body></html>");
}
