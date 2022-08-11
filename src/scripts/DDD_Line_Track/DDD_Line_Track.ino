/* ===================================
 * Car Line Tracking Program
 * ===================================
 * Contains logic to move car along a tracked line
 *
 * @author Denis Zholob
 */

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include <Arduino.h>
// #include "DualHBridgeController.h"

// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================

// RC Channels for matrix
#define RC_THROTTLE = 0
#define RC_TURN     = 1


// PWM Signals
#define PWM_MIN 1000 // Microseconds (us)
#define PWM_MID 1500 // Microseconds (us)
#define PWM_MAX 2000 // Microseconds (us)

// Analog Signals
#define ANALOG_MIN 0     //
#define ANALOG_MID 127.5 //
#define ANALOG_MAX 255   //

// Tracking Sensors (IR)
#define PIN_TRACKING_SENSOR_R A2
#define PIN_TRACKING_SENSOR_M A1
#define PIN_TRACKING_SENSOR_L A0

// Left Motor Pins
#define PIN_MOTOR_LEFT_SPEED  5 // ENA (gray)   (PWM req for variable speed)
#define PIN_MOTOR_LEFT_LOGIC1 2 // IN1 (purple)
#define PIN_MOTOR_LEFT_LOGIC2 3 // IN2 (blue)

// Right Motor Pins
#define PIN_MOTOR_RIGHT_LOGIC1 9  // IN3 (green)
#define PIN_MOTOR_RIGHT_LOGIC2 10 // IN4 (yellow)
#define PIN_MOTOR_RIGHT_SPEED  6  // ENB (orange) (PWM req for variable speed)

// Indicies for drive matrix below and car state
#define CAR_STATE_STOP    0
#define CAR_STATE_FORWARD 1
#define CAR_STATE_BACK    2
#define CAR_STATE_RIGHT   3
#define CAR_STATE_LEFT    4

#define CAR_SPEED 300 // 294 Value range 0 - 500; 150 Value range 0 - 255 (Added to PWM_MID)
#define CAR_TRIM  5   // Trim car to go straight (adds/subtracts to speed)

// ================================================================================================================
// Declaring Variables
// ================================================================================================================
// {IN1, IN2, IN3, IN4}
const int drive_state_matrix[5][4] = {
    {0, 0, 0, 0}, // CAR_STATE_STOP
    {0, 1, 0, 1}, // CAR_STATE_FORWARD: Left and Right go same direction
    {1, 0, 1, 0}, // CAR_STATE_BACK: Left and right go same direction
    {0, 1, 1, 0}, // CAR_STATE_RIGHT: Left forward, Right Back
    {1, 0, 0, 1}  // CAR_STATE_LEFT: Right forward, Left Back
};

// Tracking sensor values   L, M, R
// Index                    0  1  2
int track_sensor_vals[3] = {0, 0, 0};

// Default PWM_MID values for safety (neutral, no movement)
int rcInput[2] = {PWM_MID};  // RC values range 1000us - 2000us
int rcOutput[2] = {PWM_MID}; // RC values range 1000us - 2000us

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  // Tracking IR Sensors
  pinMode(PIN_TRACKING_SENSOR_R, INPUT);
  pinMode(PIN_TRACKING_SENSOR_M, INPUT);
  pinMode(PIN_TRACKING_SENSOR_L, INPUT);

  // Set the defined motor pins to the output
  pinMode(PIN_MOTOR_LEFT_SPEED, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_LOGIC1, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_LOGIC2, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_SPEED, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_LOGIC1, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_LOGIC2, OUTPUT);

  // Make sure the car is stopped initially for at least 3s.
  car_Stop();
  delay(3000);

  // Set a constant speed for the car as there is no actual inputs
  rcInput[RC_THROTTLE] = PWM_MID + CAR_SPEED;
}

// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
  carTrackLine();
}

// ================================================================================================================
// Helper Functions
// ================================================================================================================

// Algorithm to move the car along a painted line
void carTrackLine() {
  int num1, num2, num3;
  num1 = digitalRead(PIN_TRACKING_SENSOR_R);
  num2 = digitalRead(PIN_TRACKING_SENSOR_M);
  num3 = digitalRead(PIN_TRACKING_SENSOR_L);

  if ((num1 == 0) && num2 && num3) {
    _mleft(); // The sensor detected that right car turn left immediately when it meets black line
    delay(2);
    while (1) {
      num2 = digitalRead(2); // Cycle to judge degree of intermediate sensor,
      if (num2 == 1) {
        _mleft(); // If num2==1 does not go to the middle position, continue to turn left.
        delay(2);
      } else {
        break; // Detection of num2==0 instructions turned over, out of the loop, detection of three sensors’ statusand then make appropriate action
      }
    } // The following and so on
  } else if (num2 && num1 && (num3 == 0)) {
    _mright();
    delay(2);
    while (1) {
      num2 = digitalRead(2);
      if (num2 == 1) {
        _mright();
        delay(2);
      } else {
        break;
      }
    }
  } else {
    _mForward();
    delay(2);
  }
}

// Returns the correct PWM value from given analog value
int analogToPWM(int analog) {
  return map(analog, ANALOG_MIN, ANALOG_MAX, PWM_MIN, PWM_MAX);
}

// Returns the correct analog value from given PWM value
int analogToPWM(int pwm) {
  return map(pwm, PWM_MIN, PWM_MAX, ANALOG_MIN, ANALOG_MAX);
}

// Turns On/Off motors based on the values in the drive_state_matrix
// @param matrix_index Which drive mode to apply (integer value as index into the drive_state_matrix array)
// @param duty_cycle value b/w 0-255 makes the motors spin slow or fast
void activateMotors(int matrix_index, int duty_cycle_l, int duty_cycle_r) {
  // Set left side speed
  if (duty_cycle_l) {
    analogWrite(PIN_MOTOR_LEFT_SPEED, duty_cycle_l + CAR_TRIM);
  } else {
    digitalWrite(PIN_MOTOR_LEFT_SPEED, 0);
  }

  // Set right side speed
  if (duty_cycle_r) {
    analogWrite(PIN_MOTOR_RIGHT_SPEED, duty_cycle_r - CAR_TRIM);
  } else {
    digitalWrite(PIN_MOTOR_RIGHT_SPEED, 0);
  }

  // Set motor rotation direction
  digitalWrite(PIN_MOTOR_LEFT_LOGIC1, drive_state_matrix[matrix_index][0]);
  digitalWrite(PIN_MOTOR_LEFT_LOGIC2, drive_state_matrix[matrix_index][1]);
  digitalWrite(PIN_MOTOR_RIGHT_LOGIC1, drive_state_matrix[matrix_index][2]);
  digitalWrite(PIN_MOTOR_RIGHT_LOGIC2, drive_state_matrix[matrix_index][3]);
}

// Stop the car
void car_Stop() {
  activateMotors(CAR_STATE_STOP, 0, 0);
  Serial.println(" STOPPED");
}
// Car moves Forward
// @param duty_cycle value b/w 0-255
void car_Forward(int duty_cycle) {
  activateMotors(CAR_STATE_FORWARD, duty_cycle, duty_cycle);
  Serial.println(" FORWARD");
}
// Car moves Backward
// @param duty_cycle value b/w 0-255
void car_Back(int duty_cycle) {
  activateMotors(CAR_STATE_BACK, duty_cycle, duty_cycle);
  Serial.println(" REVERSE");
}
// Car moves Right
// @param duty_cycle value b/w 0-255
void car_Right(int duty_cycle) {
  activateMotors(CAR_STATE_RIGHT, duty_cycle, duty_cycle);
  Serial.println(" RIGHT");
}
// Car moves Left
// @param duty_cycle value b/w 0-255
void car_Left(int duty_cycle) {
  activateMotors(CAR_STATE_LEFT, duty_cycle, duty_cycle);
  Serial.println(" LEFT");
}

void car_Move(int state, int duty_cycle_l, int duty_cycle_r) {}
