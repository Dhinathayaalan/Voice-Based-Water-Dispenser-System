#include <WiFi.h>
#include <WebServer.h>

// Wi-Fi credentials
const char* ssid = "tryant";
const char* password = "speed123";

// Relay pin assignments
const int HOT_WATER_RELAY = 4;
const int COLD_WATER_RELAY = 2;

// Create a web server on port 80
WebServer server(80);

void handleHotOn() {
  digitalWrite(HOT_WATER_RELAY, HIGH);
  server.send(200, "text/plain", "Hot water ON");
}

void handleHotOff() {
  digitalWrite(HOT_WATER_RELAY, LOW);
  server.send(200, "text/plain", "Hot water OFF");
}

void handleColdOn() {
  digitalWrite(COLD_WATER_RELAY, HIGH);
  server.send(200, "text/plain", "Cold water ON");
}

void handleColdOff() {
  digitalWrite(COLD_WATER_RELAY, LOW);
  server.send(200, "text/plain", "Cold water OFF");
}

void setup() {
  Serial.begin(115200);

  pinMode(HOT_WATER_RELAY, OUTPUT);
  pinMode(COLD_WATER_RELAY, OUTPUT);
  digitalWrite(HOT_WATER_RELAY, LOW);
  digitalWrite(COLD_WATER_RELAY, LOW);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP address: " + WiFi.localIP().toString());

  // Define routes
  server.on("/hot_on", handleHotOn);
  server.on("/hot_off", handleHotOff);
  server.on("/cold_on", handleColdOn);
  server.on("/cold_off", handleColdOff);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
