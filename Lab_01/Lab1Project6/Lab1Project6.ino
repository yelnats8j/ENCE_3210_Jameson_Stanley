// Pin definitions
#define RED_LED 3      // Red LED on Pin 3
#define GREEN_LED 4    // Green LED on Pin 4
#define BLUE_LED 5     // Blue LED on Pin 5
#define BUTTON_PIN 2   // Button on Pin 2

// LED state array: Each row represents the state of the LEDs [Red, Green, Blue]
const int ledStates[6][3] = 
{
    {1, 0, 0}, // State 0: Red LED on
    {1, 1, 0}, // State 1: Red and Green LEDs on
    {1, 1, 1}, // State 2: Red, Green, and Blue LEDs on
    {0, 1, 1}, // State 3: Green and Blue LEDs on
    {0, 0, 1}, // State 4: Blue LED on
    {1, 0, 1}  // State 5: Red and Blue LEDs on
};

int state = 0;            // Current state in the sequence
volatile int isRunning = 0; // Flag to track whether the sequence is running
bool lastButtonState = HIGH; // Stores the last button state

void setup() 
{
    // Initialize LED pins as output
    pinMode(RED_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BLUE_LED, OUTPUT);

    // Initialize button pin as input with pull-up resistor
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    // Initialize LEDs as off
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);

    // Initialize Serial Monitor
    Serial.begin(9600);
    Serial.println("Press the button to start/stop the LED sequence.");
}

void loop() {
    // Read the button state
    bool currentButtonState = digitalRead(BUTTON_PIN);

    // Check if the button state has changed and is now pressed
    if (lastButtonState == HIGH && currentButtonState == LOW) {
        isRunning = !isRunning; // Toggle the running state

        // Print to Serial Monitor whether the sequence is starting or stopping
        if (isRunning) {
            Serial.println("Sequence started.");
        } else {
            Serial.println("Sequence stopped.");
        }
    }

    // Update the last button state
    lastButtonState = currentButtonState;

    // Execute the LED sequence if running
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
    } else {
        // If not running, ensure all LEDs are off
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
    }
}
