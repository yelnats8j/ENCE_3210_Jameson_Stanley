#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Pin definitions
#define BUTTON1_PIN 2 // Button 1 on Pin 2 (INT0) - Trigger calculation
#define BUTTON2_PIN 3 // Button 2 on Pin 3 (INT1) - Display results

// Global Arrays
volatile int x[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100}; // Example pre-filled array
volatile int y[10];                                             // Result array (to be calculated)

// Variables
volatile unsigned long lastInterruptTime1 = 0; // Debounce timing for Button 1
volatile unsigned long lastInterruptTime2 = 0; // Debounce timing for Button 2
const unsigned long debounceDelay = 50;      // 50 ms debounce delay

void setup() {
    // Initialize button pins as input with pull-up resistors
    DDRD &= ~(1 << BUTTON1_PIN); // Set BUTTON1_PIN as input
    DDRD &= ~(1 << BUTTON2_PIN); // Set BUTTON2_PIN as input
    PORTD |= (1 << BUTTON1_PIN); // Enable pull-up resistor for BUTTON1_PIN
    PORTD |= (1 << BUTTON2_PIN); // Enable pull-up resistor for BUTTON2_PIN

    // Attach interrupts to buttons
    EICRA |= (1 << ISC01) | (1 << ISC00); // INT0: Rising edge for Button 1
    EICRA |= (1 << ISC11) | (1 << ISC10); // INT1: Rising edge for Button 2
    EIMSK |= (1 << INT0) | (1 << INT1);   // Enable INT0 and INT1

    // Enable global interrupts
    sei();

    // Initialize Serial Monitor (if using a simulator or debugging environment)
    Serial.begin(9600);
    Serial.println("Press BUTTON1 to calculate the y array. Press BUTTON2 to display the results.");
}

void loop() {
    // Main loop remains idle as interrupts handle the tasks
}

// ISR for BUTTON1_PIN (INT0): Calculate the y array
ISR(INT0_vect) {
    unsigned long currentTime = millis();
    if ((currentTime - lastInterruptTime1) > debounceDelay) { // Debounce check
        // Calculate y array
        y[0] = 2 * x[0]; // Special case for the first element
        for (int n = 1; n < 10; n++) {
            y[n] = 2 * x[n] - x[n - 1]; // Apply formula y[n] = 2 * x[n] - x[n-1]
        }

        // Print to indicate ISR execution
        Serial.println("Button 1 pressed: y array calculated.");
        lastInterruptTime1 = currentTime; // Update last interrupt time
    }
}

// ISR for BUTTON2_PIN (INT1): Display the y array
ISR(INT1_vect) {
    unsigned long currentTime = millis();
    if ((currentTime - lastInterruptTime2) > debounceDelay) { // Debounce check
        // Print the y array
        Serial.println("Button 2 pressed: Displaying y array...");
        for (int i = 0; i < 10; i++) {
            Serial.print("y[");
            Serial.print(i);
            Serial.print("] = ");
            Serial.println(y[i]);
        }
        lastInterruptTime2 = currentTime; // Update last interrupt time
    }
}
