// Define pin numbers for RGB LEDs (optional if not using real LEDs)
#define RED_LED 2
#define GREEN_LED 3
#define BLUE_LED 4

// Include the necessary libraries
#include <Arduino.h>

void setup() 
{
  // Initialize serial communication
  Serial.begin(9600);

  // (Optional) Set up LED pins as outputs
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  // Ensure LEDs are off initially
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
}

void loop() 
{
  // Define the array to search for letters
  char letters[] = {'A', 'R', 'G', 'B', 'C', 'R', 'G', 'B', 'R'};
  int size = sizeof(letters) / sizeof(letters[0]);

  // Counters for occurrences of R, G, and B
  int countR = 0;
  int countG = 0;
  int countB = 0;

  // Iterate through the array
  for (int i = 0; i < size; i++) 
  {
    if (letters[i] == 'R') 
    {
      countR++;
      // Simulate LED turning on with serial print
      Serial.println("Red detected!");
      digitalWrite(RED_LED, HIGH);
    }
    else if (letters[i] == 'G') 
    {
      countG++;
      Serial.println("Green detected!");
      digitalWrite(GREEN_LED, HIGH);
    }
    else if (letters[i] == 'B') 
    {
      countB++;
      Serial.println("Blue detected!");
      digitalWrite(BLUE_LED, HIGH);
    }

    // Turn off LEDs after printing (optional for simulation)
    delay(200); // Simulate visibility duration
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  }

  // Print the final counts
  Serial.print("Total R detected: ");
  Serial.println(countR);
  Serial.print("Total G detected: ");
  Serial.println(countG);
  Serial.print("Total B detected: ");
  Serial.println(countB);

  // Wait for a while before running the loop again
  delay(10000);
}
