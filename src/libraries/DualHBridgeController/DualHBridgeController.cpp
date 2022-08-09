/* ===================================
 * Dual H-bridge motor controller library
 * ===================================
 * Contains functions to control an h-bridge motor board
 *
 * @author Denis Zholob
 */

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include "DualHBridgeController.h"

// ================================================================================================================
// Functions
// ================================================================================================================

// Set the defined motor pins to output
void DualHBridgeController::setHBridgePins(){
  pinMode(PIN_MOTOR_LEFT_SPEED,   OUTPUT);
  pinMode(PIN_MOTOR_LEFT_LOGIC1,  OUTPUT);
  pinMode(PIN_MOTOR_LEFT_LOGIC2,  OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_SPEED,  OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_LOGIC1, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_LOGIC2, OUTPUT);
}

// Reverse the motor direction if motor polarity was physically reversed
void DualHBridgeController::setReverseDirection(bool reverse){
  _reverse_motor_direction = reverse;
}

// Trim the turning to keep the car going straight
// Acceptable value set: (-255, 255)
void DualHBridgeController::setTrim(int trim){
  _turn_trim = boundSignal(trim);
}

// Deadband value witch which within the car is stopped
// Depends on the power setup and personal preference
// Acceptable value set: (0, 255)
// Recommended values: (~10, ~40)
void DualHBridgeController::setDeadband(int deadband){
  _deadband = boundSignal(deadband);
}

// Turns the motors based on the input
// @param velocity desired car velocity; acceptable value set: (-255, 255)
// @param turn desired turn amount to mix with car velocity; acceptable value set: (-255, 255)
void DualHBridgeController::activateMotors(int velocity, int turn){
  // Reverse motor direction if flag is set.
  // No need to reverse the turn as that is cw or ccw so its the same regardles of the car direction.
  if(_reverse_motor_direction){
    velocity *= -1;
  }

  int velocity_left = velocity + turn + _turn_trim;    // Increase left motor speed to turn right
  int velocity_right = velocity - turn - _turn_trim;   // Increase right motor speed to turn left
  int speed_left = boundSignal(abs(velocity_left));
  int speed_right = boundSignal(abs(velocity_right));

  // --- Left Motor --- //
  // Rotate Forward
  if(velocity_left > _deadband) {
    activateHbridgeLeft(MOTOR_STATE_FWD, speed_left);
  }
  // Rotate back
  else if(velocity_left < -_deadband) {
    activateHbridgeLeft(MOTOR_STATE_BWD, speed_left);
  }
  // Stop
  else {
    activateHbridgeLeft(MOTOR_STATE_STOP, speed_left);
  }

  // --- Right Motor --- //
  // Rotate Forward
  if(velocity_right > _deadband) {
    activateHbridgeRight(MOTOR_STATE_FWD, speed_right);
  }
  // Rotate back
  else if(velocity_right < -_deadband) {
    activateHbridgeRight(MOTOR_STATE_BWD, speed_right);
  }
  // Stop
  else {
    activateHbridgeRight(MOTOR_STATE_STOP, speed_right);
  }
}

// Keeps the signal in 0 - 255 range
int DualHBridgeController::boundSignal(int val){
  if(val > ANALOG_OUT_MAX) return ANALOG_OUT_MAX;
  if(val < ANALOG_OUT_MIN) return ANALOG_OUT_MIN;
  return val;
}

// Turns left motors clockwise, counterclockwise, or stops them
void DualHBridgeController::activateHbridgeLeft(int matrix_index, int duty_cycle){
  // Set motor rotation direction
  digitalWrite(PIN_MOTOR_LEFT_LOGIC1,  h_bridge_motor_spin_state[matrix_index][0]);
  digitalWrite(PIN_MOTOR_LEFT_LOGIC2,  h_bridge_motor_spin_state[matrix_index][1]);

  // Set motor rotation speed
  // If state is stopped set pin low to eliminate pwm whine
  if(matrix_index == MOTOR_STATE_STOP){
    digitalWrite(PIN_MOTOR_LEFT_SPEED, 0);
  } else {
    analogWrite(PIN_MOTOR_LEFT_SPEED, duty_cycle);
  }
}

// Turns right motors clockwise, counterclockwise, or stops them
void DualHBridgeController::activateHbridgeRight(int matrix_index, int duty_cycle){
  // Set motor rotation direction
  digitalWrite(PIN_MOTOR_RIGHT_LOGIC1,  h_bridge_motor_spin_state[matrix_index][0]);
  digitalWrite(PIN_MOTOR_RIGHT_LOGIC2,  h_bridge_motor_spin_state[matrix_index][1]);

  // Set motor rotation speed
  // If state is stopped set pin low to eliminate pwm whine
  if(matrix_index == MOTOR_STATE_STOP){
    digitalWrite(PIN_MOTOR_RIGHT_SPEED, 0);
  } else {
    analogWrite(PIN_MOTOR_RIGHT_SPEED, duty_cycle);
  }
}
