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
  server.send(200, "text/html", "<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n  <meta charset=\"UTF-8\">\n\n  <title>UrbanDev Crew</title>\n\n\n</head>\n\n<body>\n  \n  <div id=\"message-container\"></div>\n  <form id=\"send-container\">\n    <input type=\"text\" id=\"message-input\">\n    <button type=\"submit\" id=\"send-button\">Send</button>\n  </form>\n\n  <script>\n\n    const name = prompt('What is your name?');\n\n    const messageForm = document.getElementById('send-container');\n    const messageInput = document.getElementById('message-input');\n\n    messageForm.addEventListener('submit', e => {\n      e.preventDefault();\n      const message = messageInput.value;\n      fetch('http://192.168.4.1/message', {\n        method: 'post',\n        headers: {\n          'Content-Type': 'text/plain'\n        },\n        body: name + \": \" + message,\n        mode: \"no-cors\"\n        \n      }).then(res => res.text())\n        .then(res => console.log(res))\n      \n      messageInput.value = '';\n      return false; \n    });\n\n\n\n\n    function myFunction() {\n      console.log('This function is called every 5 seconds');\n    }\n\n    //setInterval(myFunction, 5000);\n\n  </script>\n</body>\n\n</html>");
}

void checkForChanges() {
  if (clients <= 0) {
    server.send(200, "text/plain", "");
    return;
  }

  server.send(200, "text/plain", allMessages);

  if (totalClients == WiFi.softAPgetStationNum()) {
    clients -= 1;
  }
  else {
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
      allMessages += "@#@#@#";
    }
    allMessages += message;

    Serial.println("-------------------------");
    Serial.print("All messages: ");
    Serial.println(allMessages);
    Serial.println("-------------------------");
    
    totalClients = WiFi.softAPgetStationNum();
    clients = totalClients;
  }
  else {
    server.send(404, "text/plain", "Failed to send message.");
  }
}