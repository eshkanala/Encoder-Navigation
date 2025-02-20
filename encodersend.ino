#include <Encoder.h>

// Encoder pins
const int encoderPinA = 2; // CLK
const int encoderPinB = 3; // DT
const int encoderButtonPin = 4; // Encoder SW
const int controlButtonPin = 5; // Separate Control Button

Encoder myEncoder(encoderPinA, encoderPinB); // Encoder object

// Variables for encoder and control state
long encoderPosition = 0;
bool controlKeyPressed = false;
bool lastControlButtonState = HIGH; // For button press detection

// Debouncing for encoder and control button (if needed, adjust values)
unsigned long lastEncoderTickTime = 0;
const unsigned long encoderDebounceDelay = 5; // Milliseconds

unsigned long lastControlButtonDebounceTime = 0;
const unsigned long controlButtonDebounceDelay = 50; // Milliseconds (more debounce for buttons)

// Key press rate limiting (similar to joystick, adjust if needed)
unsigned long lastKeyPressTime = 0;
const unsigned long keyPressInterval = 150; // milliseconds

void setup() {
  Serial.begin(9600);

  pinMode(encoderButtonPin, INPUT_PULLUP);
  pinMode(controlButtonPin, INPUT_PULLUP);

  Serial.println("Rotary Encoder Arrow Key Control Started!");
  Serial.println("Rotate encoder for Left/Right, press Control for Up/Down, Encoder button for Click.");
}

void loop() {
  // 1. Read Encoder Input
  long newPosition = myEncoder.read(); // Get the current encoder position

  if (newPosition != encoderPosition) { // Encoder position changed
    if (millis() - lastEncoderTickTime > encoderDebounceDelay) { // Simple debounce
      if (newPosition > encoderPosition) {
        // Encoder rotated clockwise (adjust direction if needed for your encoder)
        sendNavigation('R'); // Send 'R' for right
      } else {
        // Encoder rotated counter-clockwise
        sendNavigation('L'); // Send 'L' for left
      }
      encoderPosition = newPosition;
      lastEncoderTickTime = millis();
    }
  }

  // 2. Read Control Button
  int controlButtonState = digitalRead(controlButtonPin);
  if (controlButtonState == LOW && lastControlButtonState == HIGH) { // Button pressed
    if (millis() - lastControlButtonDebounceTime > controlButtonDebounceDelay) {
      controlKeyPressed = !controlKeyPressed; // Toggle control key state
      Serial.print("Control Key Pressed: "); Serial.println(controlKeyPressed ? "ON" : "OFF"); // For debugging
      lastControlButtonDebounceTime = millis();
    }
  }
  lastControlButtonState = controlButtonState;

  // 3. Read Encoder Button (for Click)
  int encoderButtonState = digitalRead(encoderButtonPin);
  if (encoderButtonState == LOW) {
    sendClick(); // Send click command
  }

  delay(10); // Small delay
}

void sendNavigation(char direction) {
  if (millis() - lastKeyPressTime > keyPressInterval) { // Rate limiting
    if (controlKeyPressed) {
      if (direction == 'L') {
        Serial.println("U"); // Send 'U' for Up when Control is pressed and rotating "Left" (Counter-clockwise) - Adjust if needed
      } else if (direction == 'R') {
        Serial.println("D"); // Send 'D' for Down when Control is pressed and rotating "Right" (Clockwise) - Adjust if needed
      }
    } else {
      if (direction == 'L') {
        Serial.println("L"); // Send 'L' for Left
      } else if (direction == 'R') {
        Serial.println("R"); // Send 'R' for Right
      }
    }
    lastKeyPressTime = millis();
  }
}

void sendClick() {
  Serial.println("C"); // Send 'C' for Click
}
