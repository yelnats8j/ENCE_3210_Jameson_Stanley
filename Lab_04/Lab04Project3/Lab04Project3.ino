#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Define Analog Pin for Voltage Sensing
#define SOLAR_VOLTAGE_PIN A0

// OLED Display Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Define voltage thresholds
const float SOLAR_THRESHOLD = 2.5;  // Minimum voltage for solar power
float previousVoltage = 0.0;        // Stores last voltage reading
bool batteryCharging = false;       // Flag to track charging status

void setup() {
    Serial.begin(9600);

    // Initialize OLED Display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
        Serial.println("SSD1306 allocation failed");
        for (;;); // Stop program if display initialization fails
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
}

// Function to Read Voltage from Analog Pin
float readVoltage() {
    int rawValue = analogRead(SOLAR_VOLTAGE_PIN);  // Read raw analog value (0 - 1023)
    float voltage = (rawValue / 1023.0) * 5.0; // Convert to voltage (assuming 5V reference)
    return voltage;
}

void loop() {
    float solarVoltage = readVoltage();  // Get voltage reading

    // Check if voltage is increasing (Charging Started)
    if (solarVoltage > previousVoltage + 0.02) {  // Small threshold to avoid noise fluctuations
        batteryCharging = true; // Set charging flag
    } else if (solarVoltage < previousVoltage - 0.02) {
        batteryCharging = false; // Stop charging flag if voltage is stable or decreasing
    }

    // Print values to Serial Monitor for debugging
    Serial.print("Solar/Battery Voltage: ");
    Serial.print(solarVoltage);
    Serial.println("V");

    // Clear and set up OLED display
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);

    // Display real-time voltage
    display.print("Voltage: ");
    display.print(solarVoltage, 2); // Display 2 decimal places
    display.println("V");

    display.println(); // Line spacing

    // Power Routing Logic 
    display.setTextSize(1);
    if (solarVoltage > SOLAR_THRESHOLD) {
        if (batteryCharging) {
            display.println("Battery Charging...");
            Serial.println("Battery Charging...");
        } else {
            display.println("Solar Power");
            Serial.println("Microcontroller powered by Solar Panel");
        }
    } else {
        display.println("Battery Power");
        Serial.println("Microcontroller powered by Battery");
    }

    display.display(); // Refresh OLED display

    // Update previous voltage for next cycle
    previousVoltage = solarVoltage;

    delay(500); // Update display 
}
