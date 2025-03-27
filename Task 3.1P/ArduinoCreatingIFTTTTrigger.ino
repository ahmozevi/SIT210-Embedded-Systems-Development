#include <Wire.h>
#include <BH1750.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>

// WiFi credentials
char ssid[] = "ghar_2.4";      // your WiFi name
char pass[] = "gharoghari_2121"; // your WiFi password

// IFTTT settings
char iftttKey[] = "b9C92O10AjOaS8_B9iPxv5";  // your IFTTT Webhooks key
char eventNameOn[] = "Sunlight_Detected_On";  // IFTTT event for sunlight detected
char eventNameOff[] = "Sunlight_Detected_Off";  // IFTTT event for sunlight stopped

WiFiClient client;
BH1750 lightSensor;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // Initialize the light sensor
  lightSensor.begin();
  
  // Connect to WiFi
  connectToWiFi();
}

void loop() {
  float lux = lightSensor.readLightLevel(); // Get light intensity
  
  Serial.print("Current light intensity: ");
  Serial.println(lux);

  // If the light intensity exceeds a threshold (e.g., 100 lux), send an IFTTT notification
  if (lux > 40) {
    sendIFTTTNotification(eventNameOn); // Sunlight detected
  } else {
    sendIFTTTNotification(eventNameOff); // Sunlight stopped
  }
  
  delay(10000); // Wait for 10 seconds before checking again
}

void connectToWiFi() {
  WiFi.begin(ssid, pass);
  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("....");
  }

  Serial.println("Connected to WiFi");
}

void sendIFTTTNotification(const char* eventName) {
  String url = "/trigger/" + String(eventName) + "/with/key/" + String(iftttKey);
  String message = "?value1=" + String("Notification");

  client.connect("maker.ifttt.com", 80);  // Connect to IFTTT server
  client.print("GET " + url + message + " HTTP/1.1\r\n");
  client.print("Host: maker.ifttt.com\r\n");
  client.print("Connection: close\r\n\r\n");
  
  delay(1000); // Give some time for the request to complete
}
