// Pin definitions
#define RED_LED 6      // Red LED for addition
#define GREEN_LED 4    // Green LED for subtraction
#define BLUE_LED 5     // Blue LED for multiplication
#define BUTTON1_PIN 2  // First button for selecting operation
#define BUTTON2_PIN 3  // Second button for applying operation

// Arrays
int array1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; // First array
int array2[10] = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}; // Second array
int array3[10];                                   // Result array (initially empty)

// Variables
int operationCounter = 0; // Tracks the selected operation (1 = addition, 2 = subtraction, 3 = multiplication)
volatile bool operationSelected = false; // Tracks if an operation is selected
bool lastButton1State = HIGH; // Tracks last state of BUTTON1
bool lastButton2State = HIGH; // Tracks last state of BUTTON2

// LED states for each operation
int ledPins[3] = {RED_LED, GREEN_LED, BLUE_LED}; // Array of LED pins corresponding to operations

// Array of function pointers for operations
typedef int (*operationFunc)(int, int);
int add(int a, int b) { return a + b; }
int subtract(int a, int b) { return a - b; }
int multiply(int a, int b) { return a * b; }
operationFunc operations[3] = {add, subtract, multiply};

void setup() {
    // Initialize LED pins as output
    for (int i = 0; i < 3; i++) {
        pinMode(ledPins[i], OUTPUT);
        digitalWrite(ledPins[i], LOW); // Turn off LEDs
    }

    // Initialize button pins as input with pull-up resistors
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);

    // Initialize Serial Monitor
    Serial.begin(9600);
    Serial.println("Press BUTTON1 to select operation and BUTTON2 to apply operation.");
}

void loop() {
    // Read the button states
    bool currentButton1State = digitalRead(BUTTON1_PIN);
    bool currentButton2State = digitalRead(BUTTON2_PIN);

    // Handle BUTTON1 for selecting operation
    if (lastButton1State == HIGH && currentButton1State == LOW) {
        operationCounter = (operationCounter + 1) % 3; // Cycle through 0, 1, 2
        operationSelected = true;

        // Update LEDs based on selected operation
        updateLEDs(operationCounter);

        // Print selected operation
        const char* operationsNames[3] = {"Addition", "Subtraction", "Multiplication"};
        Serial.print(operationsNames[operationCounter]);
        Serial.println(" selected.");
    }
    delay(50);

    // Handle BUTTON2 for applying operation
    if (lastButton2State == HIGH && currentButton2State == LOW && operationSelected) {
        applyOperation(operationCounter);
        operationSelected = false; // Reset operation selection
    }
    delay(50);

    // Update last button states
    lastButton1State = currentButton1State;
    lastButton2State = currentButton2State;
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
