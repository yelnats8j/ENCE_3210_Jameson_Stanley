// Pin definitions
#define RED_LED 4      // Red LED on Pin 3
#define GREEN_LED 5    // Green LED on Pin 4
#define BLUE_LED 6     // Blue LED on Pin 5
#define BUTTON_PIN 2   // Button on Pin 2 (INT0)

// LED state array: Each row represents the state of the LEDs [Red, Green, Blue]
const int ledStates[6][3] = {
    {1, 0, 0}, // State 0: Red LED on
    {1, 1, 0}, // State 1: Red and Green LEDs on
    {1, 1, 1}, // State 2: Red, Green, and Blue LEDs on
    {0, 1, 1}, // State 3: Green and Blue LEDs on
    {0, 0, 1}, // State 4: Blue LED on
    {1, 0, 1}  // State 5: Red and Blue LEDs on
};

volatile int state = 0;      // Current state in the sequence
volatile bool isRunning = false; // Flag to track whether the sequence is running
volatile unsigned long lastDebounceTime = 0; // Time of the last button press
const unsigned long debounceDelay = 50; // Debounce delay in milliseconds

void setup() 
{
    // Initialize LED pins as output
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    // Initialize button pin as input with pull-up resistor
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Attach interrupt to the button pin
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), debounceToggle, FALLING);

    // Initialize Serial Monitor
    Serial.begin(9600);
    Serial.println("Press the button to start/stop the LED sequence.");

    // Ensure all LEDs are initially off
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
}

void loop() 
{
    // If the sequence is running, update the LEDs
    if (isRunning) 
    {
        // Set the LEDs based on the current state
        digitalWrite(RED_LED, ledStates[state][0]);
        digitalWrite(GREEN_LED, ledStates[state][1]);
        digitalWrite(BLUE_LED, ledStates[state][2]);

        // Print the current LED state to the Serial Monitor
        Serial.print("State ");
        Serial.print(state);
        Serial.print(": RED=");
        Serial.print(ledStates[state][0] ? "ON" : "OFF");
        Serial.print(", GREEN=");
        Serial.print(ledStates[state][1] ? "ON" : "OFF");
        Serial.print(", BLUE=");
        Serial.println(ledStates[state][2] ? "ON" : "OFF");

        // Move to the next state
        state = (state + 1) % 6;

        // Delay between state changes
        delay(500); // Adjust delay for timing (500ms per state)
    } else 
    {
        // If not running, ensure all LEDs are off
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
    }
}

// Debounced ISR to toggle the sequence state
void debounceToggle() 
{
    unsigned long currentTime = millis(); // Get the current time
    if ((currentTime - lastDebounceTime) > debounceDelay) 
    { // Check if debounce delay has passed
        isRunning = !isRunning; // Toggle the running state
        lastDebounceTime = currentTime; // Update the last debounce time

        // Print the current state to the Serial Monitor
        if (isRunning) 
        {
            Serial.println("Sequence started.");
        } else 
        {
            Serial.println("Sequence stopped.");
        }
    }
}
