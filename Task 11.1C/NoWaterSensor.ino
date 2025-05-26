// Pin assignments
const int pumpPin = 7;            // Controls pump via relay or transistor
const int dustSensorPin = 9;      // IR line sensor (digital output)

// Set offset to simulate current time (e.g. 11:10:00 PM)
const unsigned long offsetSeconds = (11 * 3600) + (10 * 60);  // 11:10 PM Change according to current time before uploading

void setup() {
  pinMode(pumpPin, OUTPUT);
  pinMode(dustSensorPin, INPUT);
  Serial.begin(9600);
}

// Function to print simulated time in 12-hour format with AM/PM
void printSimulatedClock() {
  unsigned long seconds = (millis() / 1000) + offsetSeconds;

  int hour24 = (seconds / 3600) % 24;
  int hour12 = hour24 % 12;
  if (hour12 == 0) hour12 = 12;

  int minute = (seconds / 60) % 60;
  int second = seconds % 60;

  String ampm = (hour24 >= 12) ? "PM" : "AM";

  char timeStr[20];
  snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d %s", hour12, minute, second, ampm.c_str());

  Serial.print("Time: ");
  Serial.print(timeStr);
}

void loop() {
  int isDusty = digitalRead(dustSensorPin);  // LOW = dusty, HIGH = clean

  // Print time and dust status
  printSimulatedClock();
  Serial.print(" | Dust: ");
  Serial.println(isDusty == LOW ? "DUSTY" : "CLEAN");

  if (isDusty == LOW) {
    Serial.println("Cleaning triggered.");
    digitalWrite(pumpPin, HIGH);   // Turn pump ON
    delay(5000);                   // Run for 5 seconds
    digitalWrite(pumpPin, LOW);    // Turn pump OFF
  } else {
    Serial.println("No cleaning required.");
  }

  delay(10000);  // Check every 10 seconds
}
