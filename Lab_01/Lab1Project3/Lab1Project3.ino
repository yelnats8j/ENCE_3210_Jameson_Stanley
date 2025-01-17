#define INT_MIN 0

// Global integer array
int gArr[50];

void setup() 
{
  Serial.begin(9600); // Start serial communication

  // Initialize the random seed
  randomSeed(analogRead(0)); // Use analog pin A0 for randomness

  // Initialize the array with random values
  for (int i = 0; i < 50; i++) {
    gArr[i] = random(100); // Assign random values (0-99) to each element
  }

  // Print the initialized array for verification
  Serial.println("Initialized array:");
  for (int i = 0; i < 50; i++) {
    Serial.print("gArr[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(gArr[i]);
  }

  // Find the second maximum
  findSecondMaximum();
}

void loop() 
{
  // The loop function is not used for this task
}

void findSecondMaximum() 
{
  int max1 = INT_MIN; // Initialize maximum as the smallest integer
  int max2 = INT_MIN; // Initialize second maximum as the smallest integer

  // Iterate through the array to find the first and second maximum
  for (int i = 0; i < 50; i++) 
  {
    if (gArr[i] > max1) 
    {
      max2 = max1; // Update second maximum
      max1 = gArr[i]; // Update first maximum
    } 
    else if (gArr[i] > max2 && gArr[i] < max1) {
      max2 = gArr[i]; // Update second maximum if it is less than max1
    }
  }

  // Calculate the memory location just after the array
  int* resultLocation = gArr + 50;

 
 

  // Print the result and memory location
  Serial.print("Second maximum value: ");
  Serial.println(max2);
  Serial.print("Stored at memory address: ");
  Serial.println((unsigned long)resultLocation, HEX);
}
