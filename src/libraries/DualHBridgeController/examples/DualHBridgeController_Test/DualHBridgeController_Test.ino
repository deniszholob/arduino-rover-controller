/* ===================================
 * Dual H-bridge motor controller Test Program
 * ===================================
 * Tests the the Dual H-bridge motor controller works as expected
 * Verify visually car does what the commands say.
 *
 * @author Denis Zholob
 */

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include "DualHBridgeController.h"

#include <Arduino.h>

// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================
#define CAR_TRIM 0 // Compensate left turn

// ================================================================================================================
// Declaring Objects
// ================================================================================================================
DualHBridgeController Car;

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  // Tell board which pins are used for h-bridge motor board
  Car.setHBridgePins();

  // Trim the car so it goes straight if mechanical alignment of motors is off.
  Car.setTrim(CAR_TRIM);

  // Motor board placed opposite car direction
  Car.setReverseDirection(true);
}

// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
  carUncoupledMotion();
  delay(3000);
  carCoupledMotion();
  delay(5000);
}

// Test Uncoupled car movements (Pure Forward/Back, Turns, and Stops)
void carUncoupledMotion() {
  // Go forward
  Serial.println("Forward");
  Car.activateMotors(200, 0);
  delay(1000);

  // Stop
  Serial.println("Stop");
  Car.activateMotors(0, 0);
  delay(2000);

  // Go back
  Serial.println("Reverse");
  Car.activateMotors(-200, 0);
  delay(1000);

  // Stop
  Serial.println("Stop");
  Car.activateMotors(0, 0);
  delay(2000);

  // Turn right in one spot
  Serial.println("Right");
  Car.activateMotors(0, 200);
  delay(1000);

  // Stop
  Serial.println("Stop");
  Car.activateMotors(0, 0);
  delay(2000);

  // Turn left in one spot
  Serial.println("Left");
  Car.activateMotors(0, -200);
  delay(1000);

  // Stop
  Serial.println("Stop");
  Car.activateMotors(0, 0);
  delay(2000);
}

// Test coupled motions
void carCoupledMotion() {
  // Go forward-right
  Serial.println("Forward-Right");
  Car.activateMotors(200, 50);
  delay(1000);

  // Stop
  Serial.println("Stop");
  Car.activateMotors(0, 0);
  delay(2000);

  // Go back/left
  Serial.println("Reverse-Left");
  Car.activateMotors(-200, -50);
  delay(1000);

  // Stop
  Serial.println("Stop");
  Car.activateMotors(0, 0);
  delay(2000);

  // Go forward-left
  Serial.println("Forward-Left");
  Car.activateMotors(200, -50);
  delay(1000);

  // Stop
  Serial.println("Stop");
  Car.activateMotors(0, 0);
  delay(2000);

  // Go back/right
  Serial.println("Reverse-Right");
  Car.activateMotors(-200, 50);
  delay(1000);

  // Stop
  Serial.println("Stop");
  Car.activateMotors(0, 0);
  delay(2000);
}
