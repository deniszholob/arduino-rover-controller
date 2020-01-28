/* ===================================
 * Dual H-bridge motor controller library
 * ===================================
 * Contains functions to use with the IR Tracking Sensors
 *
 * @author Denis Zholob
 */

#ifndef DualHBridgeController_h
#define DualHBridgeController_h

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include <Arduino.h>

// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================

// Left Motor Pins
#define PIN_MOTOR_LEFT_LOGIC1  7  // IN1
#define PIN_MOTOR_LEFT_SPEED   6  // ENA
#define PIN_MOTOR_LEFT_LOGIC2  5  // IN2
// Left Motor Pins
#define PIN_MOTOR_RIGHT_LOGIC1 4  // IN3
#define PIN_MOTOR_RIGHT_SPEED  3  // ENB
#define PIN_MOTOR_RIGHT_LOGIC2 2  // IN4

// Analog Signals
#define ANALOG_OUT_MIN 0      // 
#define ANALOG_OUT_MID 127.5  // 
#define ANALOG_OUT_MAX 255    // 

// Motor rotation states for matrix below
#define MOTOR_STATE_STOP 0  // 
#define MOTOR_STATE_FWD  1  // 
#define MOTOR_STATE_BWD  2  // 

// Tracking Class holds functions to read and print sensor values
class DualHBridgeController {
  public:
    // DualHBridgeController();
    // ~DualHBridgeController();
    void setHBridgePins();
    void setTrim(int trim);
    void setDeadband(int deadband);
    void setReverseDirection(bool reverse);
    void activateMotors(int velocity, int turn);

   private:
    bool _reverse_motor_direction = false;
    int _turn_trim = 0;
    int _deadband = 10;
    const int h_bridge_motor_spin_state[3][2] = {
      {0, 0}, // MOTOR_STATE_STOP
      {0, 1}, // MOTOR_STATE_FWD Spins Forward (CCW)
      {1, 0}, // MOTOR_STATE_BWD Spins Backward (CW)
    };

    int boundSignal(int val);
    void activateHbridgeLeft(int matrix_index, int duty_cycle);
    void activateHbridgeRight(int matrix_index, int duty_cycle);
};

#endif
