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

// Right Motor Pins
#define PIN_MOTOR_RIGHT_SPEED  3  // ENB 3 <= 5
#define PIN_MOTOR_RIGHT_LOGIC1 9  // IN3 9 <= 7
#define PIN_MOTOR_RIGHT_LOGIC2 8  // IN4 8 <= 6

// Left Motor Pins
#define PIN_MOTOR_LEFT_LOGIC1  7  // IN1 7 <= 9
#define PIN_MOTOR_LEFT_LOGIC2  6  // IN2 6 <= 8
#define PIN_MOTOR_LEFT_SPEED   5  // ENA 5 <= 3

// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================
// #define CAR_TRIM 50 // Compensate left turn

// ================================================================================================================
// Declaring Objects
// ================================================================================================================
// DualHBridgeController Car;

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  // Tell board which pins are used for h-bridge motor board
  // Car.setHBridgePins();
  
  pinMode(PIN_MOTOR_LEFT_SPEED,   OUTPUT);
  pinMode(PIN_MOTOR_LEFT_LOGIC1,  OUTPUT);
  pinMode(PIN_MOTOR_LEFT_LOGIC2,  OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_SPEED,  OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_LOGIC1, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_LOGIC2, OUTPUT);

  // Trim the car so it goes straight if mechanical alignment of motors is off.
  // Car.setTrim(CAR_TRIM);

  // Motor board placed opposite car direction
  // Car.setReverseDirection(true);
}
// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
  // Stop
  digitalWrite(PIN_MOTOR_LEFT_SPEED, 0);

  // Forward
  Serial.println("Forward");
  analogWrite(PIN_MOTOR_RIGHT_SPEED, 200);
  digitalWrite(PIN_MOTOR_LEFT_LOGIC1, HIGH);
  digitalWrite(PIN_MOTOR_LEFT_LOGIC2, LOW);
  delay(2000);

  // Bark
  Serial.println("Bark");
  digitalWrite(PIN_MOTOR_LEFT_LOGIC1, HIGH);
  digitalWrite(PIN_MOTOR_LEFT_LOGIC2, HIGH);
  delay(2000);

  // Back
  Serial.println("Back");
  analogWrite(PIN_MOTOR_RIGHT_SPEED, 50);
  digitalWrite(PIN_MOTOR_LEFT_LOGIC1, LOW);
  digitalWrite(PIN_MOTOR_LEFT_LOGIC2, HIGH);
  delay(2000);

  // Break
  Serial.println("Break");
  digitalWrite(PIN_MOTOR_LEFT_LOGIC1, LOW);
  digitalWrite(PIN_MOTOR_LEFT_LOGIC2, LOW);
  delay(2000);
  
  
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