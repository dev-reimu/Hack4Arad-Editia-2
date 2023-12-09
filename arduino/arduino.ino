#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

int totalClients;
int clients;
String allMessages;

void setup() {
  Serial.begin(115200);

  // Access point
  WiFi.mode(WIFI_AP);
  WiFi.softAP("UrbanDevCrew Distress Hotspot");

  // Set up web server
  server.on("/", HTTP_GET, handleRoot);
  server.on("/message", HTTP_POST, handleMessage);
  server.on("/refresh", HTTP_GET, checkForChanges);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  Serial.println(WiFi.softAPIP());
  server.send(200, "text/html", "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>UrbanDev Crew</title></head><body><div id=\"message-container\"></div><form id=\"send-container\"><input type=\"text\" id=\"message-input\"><button type=\"submit\" id=\"send-button\">Send</button></form><script>const name = prompt('What is your name?'); const messageForm = document.getElementById('send-container'); const messageInput = document.getElementById('message-input'); messageForm.addEventListener('submit', e => { e.preventDefault(); const message = messageInput.value; fetch('/message', { method: 'post', headers: { 'Content-Type': 'text/plain' }, body: message }).then(res => res.text()) .then(res => console.log(res)) messageInput.value = ''; return false; });</script></body></html>");
}

void checkForChanges() {
  server.send(200, "text/plain", allMessages);

  if (totalClients == WiFi.softAPgetStationNum()) {
    clients -= 1;
  }
  else{
    totalClients = WiFi.softAPgetStationNum();
  }

  if (clients == 0) {
    allMessages = "";
  }

  Serial.print("Total clients: ");
  Serial.println(totalClients);
  Serial.print("Clients left: ");
  Serial.println(clients);
  Serial.println("");
}

void handleMessage() {
  if (server.hasArg("plain")) {
    String message = server.arg("plain");
    
    Serial.print("Received text: ");
    Serial.println(message);

    if (allMessages != "") {
      strcat(allMessages, "@#@#@#");
    }
    strcat(allMessages, message);

    Serial.println("-------------------------");
    Serial.print("All messages: ");
    Serial.println(allMessages);
    Serial.println("-------------------------");
    
    totalClients = WiFi.softAPgetStationNum();
    clients = totalClients;
    
    Serial.println(clients);
  }
  else {
    server.send(404, "text/plain", "Failed to send message.");
  }
}
