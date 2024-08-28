#include <WiFi.h>
#include <HTTPClient.h>

#define WIFI_SSID "tyrant"
#define WIFI_PASSWORD "speed123"

const int ledPin = 5;   // GPIO pin for LED
const int relayPin1 = 4; // GPIO pin for Relay 1
const int relayPin2 = 2; // GPIO pin for Relay 2
const char *uplinkUrl = "https://console.thingzmate.com/api/v1/device-types/esp3211/devices/esp3210/uplink";
const char *downlinkUrl = "https://console.thingzmate.com/api/v1/device-types/esp3211/devices/esp3210/downlink";
String AuthorizationToken = "Bearer 70302768998061dab0bae1accbeabab0";

void setup() {
  Serial.begin(115200);  // Start the serial communication
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);

  delay(100);  // Short delay before Wi-Fi connection attempt
  
  // Connect to Wi-Fi
  Serial.println("Connecting to WiFi...");
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 50) {  // Retry up to 50 times
    delay(500);  // Wait for 500ms
    Serial.print(".");
    retries++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConnected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi. Restarting...");
    ESP.restart();  // Restart the ESP32 if Wi-Fi connection fails
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Sending Uplink Data
    http.begin(uplinkUrl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", AuthorizationToken);
    // Check if LED, Relay1, and Relay2 are ON or OFF
    String ledStatus = digitalRead(ledPin) == HIGH ? "ON" : "OFF";
    String relay1Status = digitalRead(relayPin1) == HIGH ? "ON" : "OFF";
    String relay2Status = digitalRead(relayPin2) == HIGH ? "ON" : "OFF";

    // Create JSON payload with LED, Relay1, and Relay2 statuses
    String payload = "{\"led_status\":\"" + ledStatus + "\",\"relay1_status\":\"" + relay1Status + "\",\"relay2_status\":\"" + relay2Status + "\"}";

    // Send POST request
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Uplink HTTP Response code: " + String(httpResponseCode));
      Serial.println(response);
    } else {
      Serial.print("Uplink Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Free resources

    // Receiving Downlink Data
    http.begin(downlinkUrl);
    http.addHeader("Authorization", AuthorizationToken);

    // Send GET request
    httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println("Downlink HTTP Response code: " + String(httpResponseCode));
      Serial.println(response);

      // Parse the response and control LED/Relay based on separate "ON" and "OFF" commands
      if (response.indexOf("LON") >= 0) {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED turned ON");
      } else if (response.indexOf("LOFF") >= 0) {
        digitalWrite(ledPin, LOW);
        Serial.println("LED turned OFF");
      }

      if (response.indexOf("R1ON") >= 0) {
        digitalWrite(relayPin1, HIGH);
        Serial.println("Relay 1 turned ON");
      } else if (response.indexOf("R1OFF") >= 0) {
        digitalWrite(relayPin1, LOW);
        Serial.println("Relay 1 turned OFF");
      }

      if (response.indexOf("R2ON") >= 0) {
        digitalWrite(relayPin2, HIGH);
        Serial.println("Relay 2 turned ON");
      } else if (response.indexOf("R2OFF") >= 0) {
        digitalWrite(relayPin2, LOW);
        Serial.println("Relay 2 turned OFF");
      }

    } else {
      Serial.print("Downlink Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end(); // Free resources

  } else {
    Serial.println("WiFi not connected. Attempting to reconnect...");
    
    // Attempt to reconnect to Wi-Fi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 50) {  // Retry up to 50 times
      delay(500);  // Wait for 500ms
      Serial.print(".");
      retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nReconnected to WiFi");
    } else {
      Serial.println("\nFailed to reconnect to WiFi. Restarting...");
      ESP.restart();  // Restart the ESP32 if Wi-Fi connection fails
    }
  }

  delay(5000); // Wait for 5 seconds before sending the next request
}
