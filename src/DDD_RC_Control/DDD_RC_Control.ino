/* ===================================
 * RC PWM Input library
 * ===================================
 * Tests the RC input by printing it out on serial
 * Verify correct values by moving transmitter sticks
 * Modify the library to change pins (coded this way for max speed)
 *
 * @author Denis Zholob
 */

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include "RCInputPWM.h"
#include "DualHBridgeController.h"

// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================
#define CAR_TRIM 0      // Compensate left turn
#define CAR_DEADBAND 40 // Ger rid of PWM whine on low values, where car doesnt even move, so stop it completely

// ================================================================================================================
// Declaring Objects
// ================================================================================================================
DualHBridgeController Car;

// ================================================================================================================
// Declaring Variables
// ================================================================================================================
bool initialized = false;

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {  
  // Open the serial port and set the baud rate to 9600
  // Serial.begin(9600);

  // Tell board which pins are used for RC input
  setup_setRCPWMInteruptPins();

  // Tell board which pins are used for h-bridge motor board
  Car.setHBridgePins();

  // Trim the car so it goes straight if mechanical alignment of motors is off.
  Car.setTrim(CAR_TRIM);

  // Motor board placed opposide car direction
  Car.setReverseDirection(true);

  // Set deadband
  Car.setDeadband(CAR_DEADBAND);

  // Stop the car
  Car.activateMotors(0, 0);
}

// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
  if(!initialized){
    // Stop the car
    Car.activateMotors(0, 0);
    delay(1000);
    initialized = true;
  }else{
    // Move the car
    int velocity = pwmToAnalogSignal(flipSignal(rx_signal[1]));
    int turn = pwmToAnalogSignal(rx_signal[3]);
    Car.activateMotors(velocity, turn);
  }
}

// Returns the correct analog value from given PWM value
int pwmToAnalogSignal(int pwm){
  return map(pwm, PWM_MIN, PWM_MAX, -ANALOG_OUT_MAX, ANALOG_OUT_MAX);
}

// Flip the channel signal, use if dont want to / cannot flip in transmitter.
int flipSignal(int pwm){
  return map(pwm, PWM_MIN, PWM_MAX, PWM_MAX, PWM_MIN);
}
