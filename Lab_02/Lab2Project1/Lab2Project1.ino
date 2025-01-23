#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> // Include for _delay_ms()

// Pin definitions
#define RED_LED 4      // Red LED on Pin 3
#define GREEN_LED 5    // Green LED on Pin 4
#define BLUE_LED 6     // Blue LED on Pin 5
#define BUTTON1_PIN 2  // Button 1 on Pin 2 (INT0)
#define BUTTON2_PIN 3  // Button 2 on Pin 3 (INT1)

// Variables to track button presses
volatile int button1_count = 0;
volatile int button2_count = 0;

void setup() {
    Serial.begin(9600); // Start serial communication

    // Configure LED pins as output
    DDRD |= (1 << RED_LED) | (1 << GREEN_LED) | (1 << BLUE_LED);

    // Configure button pins as input
    DDRD &= ~(1 << BUTTON1_PIN); // Set BUTTON1_PIN as input
    DDRD &= ~(1 << BUTTON2_PIN); // Set BUTTON2_PIN as input
    PORTD |= (1 << BUTTON1_PIN); // Enable pull-up resistor for Button 1
    PORTD |= (1 << BUTTON2_PIN); // Enable pull-up resistor for Button 2

    // Configure external interrupts for buttons
    EICRA |= (1 << ISC01) | (1 << ISC00); // INT0: Rising edge triggers ISR (Button 1)
    EICRA |= (1 << ISC11) | (1 << ISC10); // INT1: Rising edge triggers ISR (Button 2)
    EIMSK |= (1 << INT0) | (1 << INT1);   // Enable INT0 and INT1

    // Enable global interrupts
    sei();

    // Turn off all LEDs initially
    PORTD &= ~((1 << RED_LED) | (1 << GREEN_LED) | (1 << BLUE_LED));
}

void loop() {
    // Compare button press counts and update LEDs
    if (button1_count > button2_count) {
        // Turn on Red LED
        PORTD |= (1 << RED_LED);
        PORTD &= ~((1 << GREEN_LED) | (1 << BLUE_LED));
    } else if (button1_count == button2_count) {
        // Turn on Blue LED
        PORTD |= (1 << BLUE_LED);
        PORTD &= ~((1 << RED_LED) | (1 << GREEN_LED));
    } else {
        // Turn on Green LED
        PORTD |= (1 << GREEN_LED);
        PORTD &= ~((1 << RED_LED) | (1 << BLUE_LED));
    }

    // Print button counts to Serial Monitor
    Serial.print("Button 1: ");
    Serial.print(button1_count);
    Serial.print(" | Button 2: ");
    Serial.println(button2_count);

    // Short delay to avoid LED flickering
    _delay_ms(100);
}

// ISR for Button 1 (INT0)
ISR(INT0_vect) {
    button1_count++;
}

// ISR for Button 2 (INT1)
ISR(INT1_vect) {
    button2_count++;
}
