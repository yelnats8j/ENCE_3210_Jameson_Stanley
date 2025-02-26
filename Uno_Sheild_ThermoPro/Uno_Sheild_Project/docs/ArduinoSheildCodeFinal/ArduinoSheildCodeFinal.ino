// Include Libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "images.h"  // Include bitmaps

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// Pin Definitions
#define LOCK_BUTTON 3
#define ONOFF_BUTTON 2
#define UNITS_BUTTON 6
#define CAL_BUTTON 5
#define LIGHT_BUTTON 4
#define LED 9
#define TEMP_SENSOR 12

// OLED Display Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// MPU6050 Gyroscope
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Temperature Sensor Configuration
OneWire oneWire(TEMP_SENSOR);
DallasTemperature sensors(&oneWire);

// State Flags
volatile bool displayOn = false;  // Screen starts OFF
bool isLocked = false;  // Default: Unlocked
volatile float tempC = 0;
bool displayInFahrenheit = false;  
bool ledState = false;  // Track LED toggle state

// Timer Variables
unsigned long unitsPressStart = 0;
bool unitsButtonPressed = false;
unsigned long calFlashStart = 0;
bool calFlashing = false;

// Bitmap Array
const uint8_t* bitmaps[] = { images[0], images[1], images[2], images[3] };

void setup() {
  Serial.begin(9600);

  // Initialize MPU6050
  Wire.begin();
  accelgyro.initialize();
  if (accelgyro.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
  }

  // Initialize Buttons
  pinMode(LOCK_BUTTON, INPUT);
  pinMode(ONOFF_BUTTON, INPUT);
  pinMode(UNITS_BUTTON, INPUT);
  pinMode(CAL_BUTTON, INPUT);
  pinMode(LIGHT_BUTTON, INPUT);

  // Initialize LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Initialize Temperature Sensor
  sensors.begin();

  // Initialize Display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true); // Halt execution
  }

  // Start with Display OFF
  display.clearDisplay();
  display.display();
  
  attachInterrupt(digitalPinToInterrupt(ONOFF_BUTTON), isr_onoff_button, FALLING);
  attachInterrupt(digitalPinToInterrupt(LOCK_BUTTON), isr_lock_button, FALLING);
}

void loop() {
  checkButtons();
  updateScreenRotation(); // Update rotation based on MPU6050 data

  if (displayOn) {
    updateTemperature();
  } else {
    // Keep the screen OFF if displayOn is false
    display.clearDisplay();
    display.display();
  }

  delay(50);
}

// ISR for On/Off Button
void isr_onoff_button() {
  displayOn = !displayOn;
}

// ISR for Lock Button
void isr_lock_button() {
  isLocked = !isLocked;  // Toggle Lock State
  Serial.println(isLocked ? "Locked" : "Unlocked");
}

// Button Handling
void checkButtons() {
  // Light Button (Toggle LED On/Off)
  if (digitalRead(LIGHT_BUTTON) == HIGH) {
    ledState = !ledState;  // Toggle state
    digitalWrite(LED, ledState ? HIGH : LOW);
    delay(250);  // Small debounce delay
  }

  // Units Button (Must be Held for 3 Seconds)
  if (digitalRead(UNITS_BUTTON) == HIGH) {
    if (!unitsButtonPressed) {
      unitsPressStart = millis();
      unitsButtonPressed = true;
    } else if (millis() - unitsPressStart >= 3000) {
      displayInFahrenheit = !displayInFahrenheit;
      Serial.println(displayInFahrenheit ? "Switched to Fahrenheit" : "Switched to Celsius");
      unitsButtonPressed = false;  // Reset after switching
    }
  } else {
    unitsButtonPressed = false;
  }

  // Cal Button - Flash "CAL" on screen for 1 second
  if (digitalRead(CAL_BUTTON) == HIGH) {
    calFlashStart = millis();
    calFlashing = true;
  }

  if (calFlashing && millis() - calFlashStart < 1000) {
    flashCal();
  } else {
    calFlashing = false;
  }
}

// Adjust Screen Rotation Based on Gyroscope Data
void updateScreenRotation() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  if (ax > 10000) {
    display.setRotation(2);  // Normal
  } else if (ax < -10000) {
    display.setRotation(0);  // Upside down
  }
}

// Read and Display Temperature + Lock/Unlock Icon
void updateTemperature() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  if (temp != DEVICE_DISCONNECTED_C) {
    tempC = displayInFahrenheit ? (temp * 9.0 / 5.0) + 32.0 : temp;
    displayTemperature();
  } else {
    Serial.println("Error: Could not read temperature data");
  }
}

// OLED Display Update
void displayTemperature() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  // Show Temperature
  display.println("Temp:");
  display.print(tempC);
  display.print(displayInFahrenheit ? " F" : " C");

  // Draw Lock or Unlock Icon
  display.drawBitmap(0, 10, isLocked ? bitmaps[0] : bitmaps[1], 128, 32, WHITE);

  display.display();
}

// Flash "CAL" on Screen
void flashCal() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(40, 10);
  display.println("CAL");
  display.display();
}
