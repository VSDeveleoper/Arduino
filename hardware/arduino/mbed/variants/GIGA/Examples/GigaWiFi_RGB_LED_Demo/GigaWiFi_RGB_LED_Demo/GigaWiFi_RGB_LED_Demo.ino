/*
  Arduino Giga WiFi – RGB LED Demonstration

  This sketch showcases how to control the built-in RGB LED on the Arduino Giga R1 WiFi board. 
  It demonstrates basic color mixing by setting the Red, Green, and Blue channels independently, allowing you to experiment with various color outputs.

  Designed specifically for the Giga R1 WiFi, this example helps new users understand how to interface with onboard components without needing any external wiring.

  Board Compatibility:
  * Arduino Giga R1 WiFi (only) connected via USB-C.

  Features Demonstrated:
  * Direct control of RGB LED pins
  * Simple timing with delay()
  * Sequential color transitions

  Available Commands (case-sensitive):
    BLED   - Switch ON the RED channel of the RGB LED.
    GLED   - Switch ON the GREEN channel of the RGB LED.
    RLED   - Switch ON the BLUE channel of the RGB LED.
    LEDoff - Switch OFF the LED.
    BlinkB - Blink the LED in the Blue color channel.
    BlinkG - Blink the LED in the Green color channel.
    BlinkR - Blink the LED in the Red color channel.

  Usage:
  1. Upload the sketch to your Arduino Giga R1 WiFi.
  2. Start the serial montior.
  3. Interact by typing in any of the previous 7 commands.

  Created:    09 April 2025
  Author:     Ahmed Sharaf (Tech Forge International LLC)
  License:    MIT
*/

// Global variables and constants
String inputCommand = "";      // Buffer to store incoming Serial data.
const int commandDelay = 500;  // Delay used for LED blinking in milliseconds (change according to your personal pereference).
const int blinkCount = 5;      // Number of times to blink the LED (change according to your personal pereference).

// Function declarations
void processCommand(String command);
void TurnOn(int led, String ledName);
void BlinkLED(int led, String ledName);
void TurnOffLED();

void setup() {
  // Initialize Serial communication at 115200 baud.
  Serial.begin(115200);

  // Wait for the serial port to connect (for native USB boards).
  while (!Serial) {
    ;  // Do nothing until Serial is ready.
  }

  // Configure LED pins as outputs.
  pinMode(LEDB, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDR, OUTPUT);

  // Turn off all LEDs at startup.
  TurnOffLED();

  // Display welcome message and available commands.
  Serial.println("=========================================================");
  Serial.println("           Arduino Giga WiFi | RGB LED Control");
  Serial.println("=========================================================");
  Serial.println("Enter one of the following 7 commands (case-sensitive):");
  Serial.println("---------------------------------------------------------");
  Serial.println("  RLED    : Switch ON the RED channel of the RGB LED.");
  Serial.println("  GLED    : Switch ON the GREEN channel of the RGB LED.");
  Serial.println("  BLED    : Switch ON the BLUE channel of the RGB LED.");
  Serial.println("  LEDoff  : Switch OFF the RGB LED.");
  Serial.println("  BlinkR  : Blink the LED in the RED color channel.");
  Serial.println("  BlinkG  : Blink the LED in the GREEN color channel.");
  Serial.println("  BlinkB  : Blink the LED in the BLUE color channel.");
  Serial.println("---------------------------------------------------------");
}

void loop() {
  // Read incoming serial data character-by-character.
  while (Serial.available()) {
    char c = Serial.read();
    // Append characters to command buffer; ignore newline and comma delimiters.
    if (c != '\n' && c != ',') {
      inputCommand += c;
    }
    delay(2);  // Short delay to ensure complete data reception.
  }

  // If a command was received, process it.
  if (inputCommand.length() > 0) {
    inputCommand.trim();  // Remove any leading/trailing whitespace.
    processCommand(inputCommand);
    inputCommand = "";  // Clear the command buffer.
    Serial.flush();     // Clear any leftover data in the Serial buffer.
  }
}

// Processes the received command and triggers the corresponding action.
void processCommand(String command) {
  // Debug print for testing purposes
  Serial.print("Received command: ");
  Serial.println(command);

  if (command == "BLED") {
    TurnOn(LEDB, "Blue");
  } else if (command == "GLED") {
    TurnOn(LEDG, "Green");
  } else if (command == "RLED") {
    TurnOn(LEDR, "Red");
  } else if (command == "LEDoff") {
    TurnOffLED();
    Serial.println("Turned off all LEDs.");
  } else if (command == "BlinkB") {
    BlinkLED(LEDB, "Blue");
  } else if (command == "BlinkG") {
    BlinkLED(LEDG, "Green");
  } else if (command == "BlinkR") {
    BlinkLED(LEDR, "Red");
  } else {
    Serial.println("Unrecognized command. Please try again using one of the 7 commands avaialble.");
  }
}

// Activates the selected color channel of the RGB LED; assumes active LOW.
void TurnOn(int led, String ledName) {
  // Ensure the LED is off before switching to the selected color.
  TurnOffLED();
  digitalWrite(led, LOW);  // Activate the specified color channel.
  Serial.println("Turned on the " + ledName + " LED.");
}

// Blinks the specified color channel of the RGB LED a given number of times.
void BlinkLED(int led, String colorName) {
  // Ensure all LEDs are off before blinking.
  TurnOffLED();
  Serial.println("Blinking the LED in " + colorName + " " + String(blinkCount) + " times.");
  for (int i = 0; i < blinkCount; i++) {
    digitalWrite(led, LOW);  // Turn LED ON with the specified color.
    delay(commandDelay);
    digitalWrite(led, HIGH);  // Turn LED OFF.
    delay(commandDelay);
  }
}

// Turns off all color channels of the integrated RGB LED; assumes active LOW.
void TurnOffLED() {
  digitalWrite(LEDB, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, HIGH);
  delay(2);  // Short delay to ensure state change is registered.
}
