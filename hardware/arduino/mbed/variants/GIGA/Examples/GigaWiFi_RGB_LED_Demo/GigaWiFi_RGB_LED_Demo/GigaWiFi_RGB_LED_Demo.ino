/*
  Arduino Giga WiFi – RGB LED Demonstration

  This sketch showcases how to control the built-in RGB LED connected to the digital pins D86, D87 and D88 (Only RGB, not accessible as GPIO) on the Arduino Giga R1 WiFi board.

  It demonstrates basic color mixing by setting the Red, Green, and Blue channels independently, allowing you to experiment with various color outputs without needing any external wiring.

  Designed specifically for the Giga R1 WiFi, this example helps new users interface with the onboard RGB LED (whose channels are predefined) and understand timing and sequential transitions.

  Board Compatibility:
    * Arduino Giga R1 WiFi (only) connected via USB-C.

  Features Demonstrated:
    * Direct control of the built-in RGB LED channels
    * Simple timing with delay()
    * Sequential color transitions

  Available Commands (case-sensitive):
    RLED       - Turn on the LED in Red color.
    GLED       - Turn on the LED in Green color.
    BLED       - Turn on the LED in Blue color.
    LEDoff     - Turn off the LED.
    BlinkR     - Blink the LED in the Red channel (default 5 times).
    BlinkG     - Blink the LED in the Green channel (default 5 times).
    BlinkB     - Blink the LED in the Blue channel (default 5 times).
    BlinkR(n)  - Blink the LED in the Red channel n times.
    BlinkG(n)  - Blink the LED in the Green channel n times.
    BlinkB(n)  - Blink the LED in the Blue channel n times.
    ToggleRGB  - Blink the LED in an RGB sequence.

  Usage:
    1. Upload the sketch to your Arduino Giga R1 WiFi.
    2. Open the Serial Monitor.
    3. Type one of the above commands to control the LED.

  Created:    09 April 2025
  Author:     Ahmed Sharaf (Tech Forge International LLC)
  License:    MIT
*/

// Global variables and constants
String inputCommand = "";         // Buffer to store incoming Serial data.
const int commandDelay = 500;     // Delay for LED blinking in milliseconds.
const int defaultBlinkCount = 5;  // Default number of blinks if not specified.

// Function declarations
void processCommand(String command);
void TurnOn(int led, String colorName);
void BlinkLED(int led, String colorName, int numBlinks);
void TurnOffLED();
void ToggleRGB();

void setup() {
  // Initialize Serial communication at 115200 baud.
  Serial.begin(115200);
  while (!Serial) { /* Wait for Serial to be ready. */ }

  // The built-in RGB LED channels are predefined—set them as outputs.
  pinMode(LEDR, OUTPUT);  //Digital Pin D86 (Only RGB, not accessible as GPIO)
  pinMode(LEDG, OUTPUT);  //Digital Pin D87 (Only RGB, not accessible as GPIO)
  pinMode(LEDB, OUTPUT);  //Digital Pin D88 (Only RGB, not accessible as GPIO)

  // Ensure the LED is off at startup.
  TurnOffLED();

  // Display the welcome message and available commands.
  Serial.println("=========================================================================");
  Serial.println("                  Arduino Giga WiFi | RGB LED Control");
  Serial.println("=========================================================================");
  Serial.println("Enter one of the following commands (case-sensitive):");
  Serial.println("-------------------------------------------------------------------------");
  Serial.println("  RLED        : Switch ON the LED in RED color.");
  Serial.println("  GLED        : Switch ON the LED in GREEN color.");
  Serial.println("  BLED        : Switch ON the LED in BLUE color.");
  Serial.println("  LEDoff      : Switch OFF the LED.");
  Serial.println("  BlinkR      : Blink the LED in the RED channel (default 5 times).");
  Serial.println("  BlinkG      : Blink the LED in the GREEN channel (default 5 times).");
  Serial.println("  BlinkB      : Blink the LED in the BLUE channel (default 5 times).");
  Serial.println("  BlinkR(n)   : Blink the LED in RED n times.");
  Serial.println("  BlinkG(n)   : Blink the LED in GREEN n times.");
  Serial.println("  BlinkB(n)   : Blink the LED in BLUE n times.");
  Serial.println("  ToggleRGB   : Blink the LED in an RGB sequence.");
  Serial.println("-------------------------------------------------------------------------");
}

void loop() {
  // Read incoming Serial data character-by-character.
  while (Serial.available()) {
    char c = Serial.read();
    // Append characters to the command buffer; ignore newline and comma delimiters.
    if (c != '\n' && c != ',') {
      inputCommand += c;
    }
    delay(2);  // Small delay helps ensure the full command is received.
  }

  // Process the command if any command has been received.
  if (inputCommand.length() > 0) {
    inputCommand.trim(); // Remove any stray whitespace.
    processCommand(inputCommand);
    inputCommand = "";   // Clear the command buffer.
    Serial.flush();      // Clear any leftover Serial data.
  }
}

// Parses and processes incoming commands.
void processCommand(String command) {
  Serial.println("Received command: " + String(command));  

  // Parameterized blink commands.
  if (command.startsWith("BlinkR(") && command.endsWith(")")) {
    int numBlinks = command.substring(command.indexOf('(') + 1, command.indexOf(')')).toInt();
    if (numBlinks > 0)
      BlinkLED(LEDR, "Red", numBlinks);
    else
      Serial.println("Invalid number for BlinkR command.");
  }
  else if (command.startsWith("BlinkG(") && command.endsWith(")")) {
    int numBlinks = command.substring(command.indexOf('(') + 1, command.indexOf(')')).toInt();
    if (numBlinks > 0)
      BlinkLED(LEDG, "Green", numBlinks);
    else
      Serial.println("Invalid number for BlinkG command.");
  }
  else if (command.startsWith("BlinkB(") && command.endsWith(")")) {
    int numBlinks = command.substring(command.indexOf('(') + 1, command.indexOf(')')).toInt();
    if (numBlinks > 0)
      BlinkLED(LEDB, "Blue", numBlinks);
    else
      Serial.println("Invalid number for BlinkB command.");
  }
  // Default commands.
  else if (command == "RLED") {
    TurnOn(LEDR, "Red");
  }
  else if (command == "GLED") {
    TurnOn(LEDG, "Green");
  }
  else if (command == "BLED") {
    TurnOn(LEDB, "Blue");
  }
  else if (command == "LEDoff") {
    TurnOffLED();
    Serial.println("Turned off the LED.");
  }
  else if (command == "BlinkR") {
    BlinkLED(LEDR, "Red", defaultBlinkCount);
  }
  else if (command == "BlinkG") {
    BlinkLED(LEDG, "Green", defaultBlinkCount);
  }
  else if (command == "BlinkB") {
    BlinkLED(LEDB, "Blue", defaultBlinkCount);
  }
  else if (command == "ToggleRGB") {
    ToggleRGB();
  }
  else {
    Serial.println("Unrecognized command. Please try one of the available commands.");
  }
}

// Turns on a specified LED color channel (active LOW logic).
void TurnOn(int led, String colorName) {
  TurnOffLED();                // Ensure only one channel is active.
  digitalWrite(led, LOW);      // Activate the channel.
  Serial.println("Turned on the LED in " + colorName + " color.");
}

// Blinks the given LED channel a specified number of times.
void BlinkLED(int led, String colorName, int numBlinks) {
  TurnOffLED();  // Ensure all channels are off before blinking.
  Serial.println("Blinking the LED in " + colorName + " " + String(numBlinks) + " times.");
  for (int i = 0; i < numBlinks; i++) {
    digitalWrite(led, LOW);    // Turn channel on.
    delay(commandDelay);
    digitalWrite(led, HIGH);   // Turn channel off.
    delay(commandDelay);
  }
}

// Turns off all channels (assumes active LOW logic).
void TurnOffLED() {
  digitalWrite(LEDB, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, HIGH);
  delay(2);  // Brief delay to ensure state change.
}

// Sequentially blinks the RGB LED in a set sequence.
void ToggleRGB() {
  Serial.println("Toggling through RGB sequence.");
  
  // Blink Red
  digitalWrite(LEDR, LOW);
  delay(commandDelay);
  TurnOffLED();
  delay(commandDelay);
  
  // Blink Green
  digitalWrite(LEDG, LOW);
  delay(commandDelay);
  TurnOffLED();
  delay(commandDelay);
  
  // Blink Blue
  digitalWrite(LEDB, LOW);
  delay(commandDelay);
  TurnOffLED();
  delay(commandDelay);
}
