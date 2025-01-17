#define ARRAY_VALUE 360

double cosOutput[ARRAY__VALUE]; // creates the array with 360 values

void setup() 
{
  Serial.begin(9600); // starts the serial monitor on the 9600 baud
}

void loop() 
{
  for (int i = 0; i < 360; i++)
  {    //for loop that goes through the array 
    cosOutput[i] = cos(i*DEG_TO_RAD);   //inputs the calculated cosine into the array using the defined DEG_TO_RAD value since the cosine function takes radians
    Serial.println(cosOutput[i]); //prints the output to the serial monitor;
  }
}
