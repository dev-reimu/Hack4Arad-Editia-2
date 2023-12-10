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
  server.enableCORS(true);
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
  server.send(200, "text/html", "<!DOCTYPE html>\n<html lang=\"en\">\n\n<head>\n    <meta charset=\"UTF-8\">\n    <title>UrbanDev Crew</title>\n    <style>\n        body {\n            font-family: Arial, sans-serif;\n            background-color: #f2f2f2;\n            display: flex;\n            flex-direction: column;\n            align-items: center;\n            justify-content: center;\n            /* Center the content vertically */\n            height: 100vh;\n            margin: 0;\n        }\n\n        #message-container {\n            background-color: #fff;\n            border-radius: 5px;\n            padding: 10px;\n            margin-bottom: 10px;\n            width: 400px;\n            /* Increase the width of the container */\n            height: 300px;\n            /* Increase the height of the container */\n            overflow-y: auto;\n            /* Add scrollbars if content exceeds container height */\n        }\n\n        #message-input {\n            width: 300px;\n            padding: 5px;\n            border: 1px solid #ccc;\n            border-radius: 3px;\n            font-size: 14px;\n        }\n\n        #send-button {\n            padding: 5px 10px;\n            background-color: #4CAF50;\n            color: #fff;\n            border: none;\n            border-radius: 3px;\n            cursor: pointer;\n            font-size: 14px;\n        }\n\n        #send-button:hover {\n            background-color: #45a049;\n        }\n    </style>\n</head>\n\n<body>\n    <div id=\"message-container\"></div>\n    <div id=\"form-container\">\n        <form id=\"send-container\">\n            <input type=\"text\" id=\"message-input\">\n            <button type=\"submit\" id=\"send-button\">Send</button>\n\n        </form>\n    </div>\n \n\n    <script>\n        const name = prompt('What is your name?');\n\n        const messageForm = document.getElementById('send-container');\n        const messageInput = document.getElementById('message-input');\n        const messageContainer = document.getElementById('message-container');\n        const maxMessages = 10;\n\n        messageForm.addEventListener('submit', e => {\n            \n            e.preventDefault();\n            const message = messageInput.value;\n            fetch('http://192.168.4.1/message', {\n                method: 'POST',\n                headers: {\n                    'Content-Type': 'text/plain'\n                },\n                body: name + \": \" + message,\n            });\n\n            messageInput.value = '';\n            return false;\n        });\n\n        function appendMessage(message) {\n            const messageElement = document.createElement('div');\n            messageElement.innerText = message;\n            messageContainer.append(messageElement);\n\n            // Remove the first message if the number of messages exceeds the maximum\n            if (messageContainer.children.length > maxMessages) {\n                messageContainer.removeChild(messageContainer.firstChild);\n            }\n        }\n        appendMessage('You joined');\n\n        async function refresher() {\n\n\n\n            let response = await fetch(\"http://192.168.4.1/refresh\", {\n                method: \"GET\",\n            });\n\n            let data = await response.text();\n            if(data!=\"No changes.\")\n                if(data.includes(\"@#@#@#\")){\n                    let arr = data.split(\"@#@#@#\");\n                    for(let i=0;i<arr.length;i++){\n                        appendMessage(arr[i]);\n                    }\n                }\n                else{\n                    appendMessage(data);\n                }\n\n\n        }\n\n        \n       \n\n        setInterval(refresher, 5000);\n    </script>\n</body>\n\n</html>");
}

void timeoutChangesRequest() {
  delay(10000);
  clients = 0;
  allMessages = "";
  Serial.println("No change requests. Clearing buffers.");
}

void checkForChanges() {
  //timeoutChangesRequest();

  if (clients <= 0) {
    server.send(200, "text/plain", "No changes.");
    Serial.println("No changes.");
    return;
  }

  server.send(200, "text/plain", allMessages);

  if (totalClients == WiFi.softAPgetStationNum()) {
    clients -= 1;
  }
  else {
    totalClients = WiFi.softAPgetStationNum();
  }

  if (clients <= 0) {
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

    server.send(200, "text/plain", "Message received.");
  }
  else {
    server.send(404, "text/plain", "Failed to send message.");
  }
}