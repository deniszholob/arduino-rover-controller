/* ===================================
 * Pins motor controller Test Program
 * ===================================
 * Tests the the motor controller pins are correct and work as expected
 * Verify visually car does what the commands say.
 *
 * @author Denis Zholob
 */

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include <Arduino.h>
// #include "DualHBridgeController.h"

// ================================================================================================================
// Declaring Constants
// ================================================================================================================
// Left Motor Pins
#define PIN_MOTOR_LEFT_SPEED  5 // ENA (gray)   (PWM req for variable speed)
#define PIN_MOTOR_LEFT_LOGIC1 2 // IN1 (purple)
#define PIN_MOTOR_LEFT_LOGIC2 3 // IN2 (blue)

// Right Motor Pins
#define PIN_MOTOR_RIGHT_LOGIC1 9  // IN3 (green)
#define PIN_MOTOR_RIGHT_LOGIC2 10 // IN4 (yellow)
#define PIN_MOTOR_RIGHT_SPEED  6  // ENB (orange) (PWM req for variable speed)

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  // Setup the output pins
  pinMode(PIN_MOTOR_LEFT_SPEED, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_LOGIC1, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_LOGIC2, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_SPEED, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_LOGIC1, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_LOGIC2, OUTPUT);
}

// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
  directionLoop(PIN_MOTOR_LEFT_SPEED, PIN_MOTOR_LEFT_LOGIC1, PIN_MOTOR_LEFT_LOGIC2);
  directionLoop(PIN_MOTOR_RIGHT_SPEED, PIN_MOTOR_RIGHT_LOGIC1, PIN_MOTOR_RIGHT_LOGIC2);
}

// ## Motor controller board states
// | Speed | Logic1| Logic2| Result    |
// | ENA   | IN1   | IN2   | Result    |
// |-------|-------|-------|-----------|
// | O     | X     | X     | Stop      |
// | 1     | 0     | O     | Break     |
// | 1     | 0     | 1     | Forward   |
// | 1     | 1     | 0     | Backward  |
// | 1     | 1     | 1     | Bark      |
void directionLoop(int motorSpeed, int motorLogic1, int motorLogic2) {
  // Stop
  digitalWrite(motorSpeed, 0);

  // Forward
  Serial.println("Forward");
  analogWrite(motorSpeed, 200);
  //  digitalWrite(motorSpeed, 1);
  digitalWrite(motorLogic1, 1);
  digitalWrite(motorLogic2, 0);
  delay(2000);

  // Bark
  Serial.println("Bark");
  digitalWrite(motorLogic1, 1);
  digitalWrite(motorLogic2, 1);
  delay(2000);

  // Backward
  Serial.println("Backward");
  analogWrite(motorSpeed, 100);
  //  digitalWrite(motorSpeed, 1);
  digitalWrite(motorLogic1, 0);
  digitalWrite(motorLogic2, 1);
  delay(2000);

  // Break
  Serial.println("Break");
  digitalWrite(motorLogic1, 0);
  digitalWrite(motorLogic2, 0);
  delay(2000);
}
