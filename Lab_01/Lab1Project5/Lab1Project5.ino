// Define pin numbers for LEDs
#define GREEN_LED 3
#define RED_LED 2

#include <Arduino.h>

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set up LED pins as outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Ensure LEDs are off initially
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void loop() {
  // Define the main array and the pattern to detect
  char mainArray[] = {'A', 'B', 'C', 'D', 'E', 'B', 'C', 'D', 'F'};
  char pattern[] = {'B', 'C', 'D'};

  int mainSize = sizeof(mainArray) / sizeof(mainArray[0]);
  int patternSize = sizeof(pattern) / sizeof(pattern[0]);
  int count = 0; // Count occurrences of the pattern

  // Loop through the main array to search for the pattern
  for (int i = 0; i <= mainSize - patternSize; i++) {
    bool match = true;
    for (int j = 0; j < patternSize; j++) {
      if (mainArray[i + j] != pattern[j]) {
        match = false;
        break;
      }
    }

    if (match) {
      count++;
      Serial.println("Pattern detected!");
      digitalWrite(GREEN_LED, HIGH); // Turn on green LED for each detection
      delay(200);
      digitalWrite(GREEN_LED, LOW); // Turn off after brief delay
    }
  }

  // Check if the pattern was detected
  if (count > 0) {
    Serial.print("Total occurrences of the pattern: ");
    Serial.println(count);
  } else {
    Serial.println("Pattern not detected.");
    digitalWrite(RED_LED, HIGH); // Turn on red LED if no pattern is found
    delay(2000);
    digitalWrite(RED_LED, LOW); // Turn off after brief delay
  }

  // Wait before restarting the loop
  delay(10000);
}
