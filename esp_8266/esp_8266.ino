#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "li";
const char* password = "abcd1234";

ESP8266WebServer server(80);

int ledPin = D5; // GPIO pin connected to the LED

void setup() {Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.on("/number", HTTP_GET, [](){
    String value = server.arg("value");
    Serial.print("Received number: ");
    Serial.println(value);
    server.send(200, "text/plain", "Number received");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
