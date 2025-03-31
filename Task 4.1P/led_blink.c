#include <wiringPi.h>  // Correct include format
#include <stdio.h>      // For printf
#include <unistd.h>     // For sleep function

#define LED_PIN 0  // WiringPi Pin 0 (GPIO 17)

int main() {
    // Initialize WiringPi
    if (wiringPiSetup() == -1) {
        printf("WiringPi setup failed!\n");
        return 1;
    }
    
    printf("WiringPi setup successful!\n");

    pinMode(LED_PIN, OUTPUT); // Set GPIO pin as output

    while (1) {  // Infinite loop
        digitalWrite(LED_PIN, HIGH); // Turn LED on
        sleep(1);  // Wait 1 second
        digitalWrite(LED_PIN, LOW); // Turn LED off
        sleep(1);  // Wait 1 second
    }

    return 0;
}
