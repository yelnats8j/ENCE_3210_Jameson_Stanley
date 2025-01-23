// Pin definitions
#define RED_LED 6      // Red LED for addition
#define GREEN_LED 4    // Green LED for subtraction
#define BLUE_LED 5     // Blue LED for multiplication
#define BUTTON1_PIN 2  // First button for selecting operation (INT0)
#define BUTTON2_PIN 3  // Second button for applying operation (INT1)

// Arrays
int array1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // First array
int array2[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}; // Second array
int array3[10];                                   // Result array (initially empty)

// Variables
volatile int operationCounter = 0; // Tracks the selected operation (0 = addition, 1 = subtraction, 2 = multiplication)
volatile bool applyOperationFlag = false; // Flag to indicate operation should be applied

// LED states for each operation
int ledPins[3] = {RED_LED, GREEN_LED, BLUE_LED}; // Array of LED pins corresponding to operations

// Array of function pointers for operations
typedef int (*operationFunc)(int, int);
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
operationFunc operations[3] = {add, subtract, multiply};

// Debounce timing
volatile unsigned long lastInterruptTime1 = 0;
volatile unsigned long lastInterruptTime2 = 0;
const unsigned long debounceDelay = 50; // 50 ms debounce delay

void setup() {
    // Initialize LED pins as output
    for (int i = 0; i < 3; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW); // Turn off LEDs
    }

    // Initialize button pins as input with pull-up resistors
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);

    // Attach interrupts to buttons
    attachInterrupt(digitalPinToInterrupt(BUTTON1_PIN), selectOperationISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(BUTTON2_PIN), applyOperationISR, FALLING);

    // Initialize Serial Monitor
    Serial.begin(9600);
    Serial.println("Press BUTTON1 to select operation and BUTTON2 to apply operation.");
}

void loop() {
    // If the applyOperationFlag is set, execute the selected operation
    if (applyOperationFlag) {
        applyOperationFlag = false; // Reset the flag
        applyOperation(operationCounter);
    }
}

// ISR for BUTTON1 (Select Operation)
void selectOperationISR() {
    unsigned long currentTime = millis();
    if (currentTime - lastInterruptTime1 > debounceDelay) { // Debounce check
        operationCounter = (operationCounter + 1) % 3; // Cycle through 0, 1, 2

        // Update LEDs based on selected operation
        updateLEDs(operationCounter);

        // Print selected operation to Serial Monitor
        const char* operationsNames[3] = {"Addition", "Subtraction", "Multiplication"};
        Serial.print(operationsNames[operationCounter]);
        Serial.println(" selected.");

        lastInterruptTime1 = currentTime; // Update last interrupt time
    }
}

// ISR for BUTTON2 (Apply Operation)
void applyOperationISR() {
    unsigned long currentTime = millis();
    if (currentTime - lastInterruptTime2 > debounceDelay) { // Debounce check
        applyOperationFlag = true; // Set the flag to apply operation
        lastInterruptTime2 = currentTime; // Update last interrupt time
    }
}

// Function to update LEDs based on the selected operation
void updateLEDs(int operation) {
    for (int i = 0; i < 3; i++) {
        digitalWrite(ledPins[i], i == operation ? HIGH : LOW); // Turn on the selected LED
    }
}

// Function to apply the selected operation to the arrays
void applyOperation(int operation) {
    Serial.println("Applying operation to arrays...");
    for (int i = 0; i < 10; i++) {
        array3[i] = operations[operation](array1[i], array2[i]); // Perform the selected operation

        // Print result to Serial Monitor
        Serial.print("array1[");
        Serial.print(i);
        Serial.print("] ");
        Serial.print(operation == 0 ? "+" : operation == 1 ? "-" : "*");
        Serial.print(" array2[");
        Serial.print(i);
        Serial.print("] = ");
        Serial.println(array3[i]);
    }

    // Turn off all LEDs once operation is complete
    for (int i = 0; i < 3; i++) {
        digitalWrite(ledPins[i], LOW);
    }

    // Turn on all LEDs to indicate operation is complete
    delay(500);
    for (int i = 0; i < 3; i++) {
        digitalWrite(ledPins[i], HIGH);
    }
    Serial.println("Operation complete. All LEDs are ON.");
}
