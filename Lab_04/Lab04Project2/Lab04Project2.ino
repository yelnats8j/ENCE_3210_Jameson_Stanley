#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define Analog Pin for Solar Voltage
#define SOLAR_VOLTAGE_PIN A0

// OLED Display Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define voltage threshold
const float THRESHOLD_VOLTAGE = 2.5; // Switching threshold

void setup() {
    Serial.begin(9600);

    // Initialize OLED Display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println("SSD1306 allocation failed");
        for (;;); // Halt program if display initialization fails
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
}

// Function to Read Solar Voltage from A0
float readSolarVoltage() {
    int rawValue = analogRead(SOLAR_VOLTAGE_PIN);  // Read raw analog value (0 - 1023)
    float voltage = (rawValue / 1023.0) * 5.0; // Convert to voltage (assuming 5V reference)
    return voltage;
}

void loop() {
    float solarVoltage = readSolarVoltage(); // Get the solar panel voltage

    // Print values to Serial Monitor for debugging
    Serial.print("Solar Voltage: ");
    Serial.print(solarVoltage);
    Serial.println("V");

    // Clear and set up OLED display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);

    // Display real-time solar voltage
    display.print("Solar: ");
    display.print(solarVoltage, 2); // Display 2 decimal places
    display.println("V");

    display.println(); // Line spacing

    // Decision Logic for Power Routing
    display.setTextSize(1.5);
    if (solarVoltage > THRESHOLD_VOLTAGE) {
        display.println("Solar Power");
        Serial.println("Microcontroller powered by Solar Panel");
    } else {
        display.println("Battery Power");
        Serial.println("Microcontroller powered by Battery");
    }

    display.display(); // Refresh OLED display
    delay(500); // Update display every 500ms
}
