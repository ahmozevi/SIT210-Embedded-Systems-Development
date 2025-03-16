#include "WiFiNINA.h"
#include "secrets.h"
#include "ThingSpeak.h" 
#include "DHT.h"

#define DHTPIN A2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// Sensor Data
int Humidity, TemperatureC, TemperatureF, HeatIndexC, HeatIndexF;
String myStatus = "";

void setup() 
{
  Serial.begin(115200);      // Initialize serial 
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak 
  Serial.println(F("Starting DHT22"));
  dht.begin();
}

void loop() 
{
  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED)
    {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print("Connecting");
      delay(5000);     
    } 
    Serial.println("\nConnected");
    delay(2000);
  }
  else if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wifi Connected");
  }
  Humidity = dht.readHumidity();
  TemperatureC = dht.readTemperature();
  TemperatureF = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(Humidity) || isnan(TemperatureC) || isnan(TemperatureF)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  
  HeatIndexC = dht.computeHeatIndex(TemperatureC, Humidity, false);
  HeatIndexF = dht.computeHeatIndex(TemperatureF, Humidity);

  // Ensure Connection to ThingSpeak
  if (!client.connect("api.thingspeak.com", 80)) 
  {
    Serial.println(F("Connection to ThingSpeak failed."));
    return;
  }
    Serial.print(F("Humidity: "));
    Serial.print(Humidity);
    Serial.println(F("%"));
    Serial.print(F("Temperature: "));
    Serial.print(TemperatureC);
    Serial.print(F("°C / "));
    Serial.print(TemperatureF);
    Serial.println(F("°F"));
    Serial.print(F("Heat index: "));
    Serial.print(HeatIndexC);
    Serial.print(F("°C / "));
    Serial.print(HeatIndexF);
    Serial.println(F("°F"));
  // set the fields with the values
  ThingSpeak.setField(1, Humidity);
  ThingSpeak.setField(2, TemperatureC);
  ThingSpeak.setField(3, TemperatureF);

  // write to the ThingSpeak channel 
  int response = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if(response == 200)
  {

    Serial.println("ThingSpeak update successful");

  }

  else
  {
    Serial.println("ThingSpeak Update Failed. HTTP error code " + String(response));
  }
  
  delay(30000); // Wait 30 seconds to update the channel again
}
