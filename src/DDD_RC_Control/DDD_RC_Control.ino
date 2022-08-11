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
#include "Servo.h"

// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================
#define CAR_TRIM 0          // Compensate left turn (40)
#define CAR_DEADBAND 40     // Get rid of PWM whine on low values, where car doesn't even move, so stop it completely
#define PIN_SERVO_SONIC 11  // Ultrasonic servo pin

#define RC_IDX_PRND RC_CH_IDX_PITCH // Forward/Backward Radio Channel From RCInputPWM
#define RC_IDX_TURN RC_CH_IDX_ROLL  // Left/Right Radio Channel From RCInputPWM
#define RC_IDX_SERVO RC_CH_IDX_ROLL // Left/Right Radio Channel From RCInputPWM

// ================================================================================================================
// Declaring Objects
// ================================================================================================================
DualHBridgeController Car;  // Car object to control motors
Servo servoSonic;           // Servo object to control servo

// ================================================================================================================
// Declaring Variables
// ================================================================================================================
bool initialized = false;
int rx_pwm_signal[RX_CHANNELS];

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  // Tell board which pins are used for RC input
  setupRCPWMInterruptPins();

  // Attach servo on defined pin to servo object
  servoSonic.attach(PIN_SERVO_SONIC);

  // Tell board which pins are used for h-bridge motor board
  Car.setHBridgePins();

  // Trim the car so it goes straight if mechanical alignment of motors is off.
  Car.setTrim(CAR_TRIM);

  // Motor board placed opposite car direction
  Car.setReverseDirection(true);

  // Set deadband
  Car.setDeadband(CAR_DEADBAND);

  // Stop the car
  Car.activateMotors(0, 0);

  // Car.setDebug(true);
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
  } else {
    // Read RC Input
    readPWMIn(rx_pwm_signal);
    
    // Move servo
    servoSonic.writeMicroseconds(flipSignal(rx_pwm_signal[RC_IDX_SERVO]));
    // Serial.print("Servo: ");
    // Serial.println(flipSignal(rx_pwm_signal[RC_IDX_SERVO]));

    // Move the car
    int velocity = pwmToAnalogSignal(flipSignal(rx_pwm_signal[RC_IDX_PRND]));
    int turn = pwmToAnalogSignal(rx_pwm_signal[RC_IDX_TURN]);
    // printRCPWMValues();
    // printCarValues(velocity, turn);
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

// ================================================================= //
// ======================= DEBUG Output ============================ //
// ================================================================= //

// Print out PWM Values and the directions
void printCarValues(int v, int t) {
  Serial.print("Velocity:");
  Serial.print(v);
  Serial.print("  Turn:");
  Serial.print(t);
  Serial.println();
}
