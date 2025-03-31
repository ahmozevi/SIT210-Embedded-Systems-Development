#include <Wire.h>
#include <BH1750.h>

// Define pins for LED and button
const int buttonPin = 2;    // Pin for push button
const int ledPin1 = 5;      // Pin for LED1
const int ledPin2 = 6;      // Pin for LED2

// Create BH1750 object
BH1750 lightMeter;

// Variables to store sensor and button states
volatile bool led1State = false;
volatile bool led2State = false;

// Interrupt handler for the push button
void toggleLED1() {
  led1State = !led1State;  // Toggle LED1 state
  digitalWrite(ledPin1, led1State ? HIGH : LOW);
  Serial.println(led1State ? "LED1 ON" : "LED1 OFF");
}

void setup() {
  // Start serial communication for debugging
  Serial.begin(9600);

  // Set up the LED pins
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  
  // Set up the button pin with internal pull-up resistor
  pinMode(buttonPin, INPUT_PULLUP);
  
  // Attach interrupt to button pin (FALLING edge)
  attachInterrupt(digitalPinToInterrupt(buttonPin), toggleLED1, FALLING);

  // Initialize the BH1750 light sensor
  Wire.begin();
  lightMeter.begin();
  lightMeter.configure(BH1750::CONTINUOUS_HIGH_RES_MODE);

  if (lightMeter.begin()) {
    Serial.println("BH1750 sensor initialized.");
  } else {
    Serial.println("Failed to initialize BH1750 sensor.");
    while (1);  // Halt if sensor fails to initialize
  }

  // Start measuring light in high-resolution mode
  lightMeter.configure(BH1750::CONTINUOUS_HIGH_RES_MODE);
}

void loop() {
  // Read light level from the BH1750 sensor
  float lux = lightMeter.readLightLevel(); // Get light intensity
  
  Serial.print("Current light intensity: ");
  Serial.println(lux);

  // If the light level is below a certain threshold, toggle LED2
  if (lux < 30) {  // Threshold of 100 lux
    led2State = !led2State;  // Toggle the state of LED2 (on/off)
    digitalWrite(ledPin2, led2State ? HIGH : LOW);  // Turn LED2 ON or OFF
    Serial.println(led2State ? "LED2 ON" : "LED2 OFF");
  }

  delay(1000);  // Delay for 1 second before the next reading
}

