/* ===================================
 * Car Motor Direction Test Program
 * ===================================
 * Contains logic to move motors in different direction
 *
 * @author Denis Zholob
 */

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include <Arduino.h>

// ================================================================================================================
// Declaring Constants
// ================================================================================================================
// Left Motor Pins
#define PIN_MOTOR_LEFT_SPEED   5  // ENA (gray)   (PWM req for variable speed)
#define PIN_MOTOR_LEFT_LOGIC1  2  // IN1 (purple) 
#define PIN_MOTOR_LEFT_LOGIC2  3  // IN2 (blue)   

// Right Motor Pins
#define PIN_MOTOR_RIGHT_LOGIC1 9  // IN3 (green)  
#define PIN_MOTOR_RIGHT_LOGIC2 10 // IN4 (yellow) 
#define PIN_MOTOR_RIGHT_SPEED  6  // ENB (orange) (PWM req for variable speed)

// Indicies for drive matrix below
#define MATRIX_STOP 0
#define MATRIX_FORWARD 1
#define MATRIX_BACK 2
#define MATRIX_RIGHT 3
#define MATRIX_LEFT 4

// {IN1, IN2, IN3, IN4}
const int drive_state_matrix[5][4] = {
  {0, 0, 0, 0}, // MATRIX_STOP
  {1, 0, 1, 0}, // MATRIX_FORWARD: Left and right go same direction
  {0, 1, 0, 1}, // MATRIX_BACK:    Left and Right go same direction
  {0, 1, 1, 0}, // MATRIX_RIGHT:   Left forward, Right Back
  {1, 0, 0, 1}  // MATRIX_LEFT:    Right forward, Left Back
};

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  // Set the defined pins to the output
  pinMode(MOTOR_LEFT_SPEED_PIN,   OUTPUT);
  pinMode(MOTOR_LEFT_LOGIC1_PIN,  OUTPUT);
  pinMode(MOTOR_LEFT_LOGIC2_PIN,  OUTPUT);
  pinMode(MOTOR_RIGHT_SPEED_PIN,  OUTPUT);
  pinMode(MOTOR_RIGHT_LOGIC1_PIN, OUTPUT);
  pinMode(MOTOR_RIGHT_LOGIC2_PIN, OUTPUT);

  // Make sure the car is stopped initially.
  car_Stop();
}

// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
  autoCarMotion();
}

// ================================================================================================================
// Helper Functions
// ================================================================================================================

// Preset of commands to move the car
void autoCarMotion(){
  Serial.println("Stopped");
  car_Stop();
  delay(1000);
  Serial.println("Forward");
  car_Forward(getSpeed(80));
  delay(2000);
  Serial.println("Stopped");
  car_Stop();
  delay(1000);
  Serial.println("Back");
  car_Back(getSpeed(80));
  delay(2000);
  Serial.println("Stopped");
  car_Stop();
  delay(1000);
  Serial.println("Right");
  car_Right(getSpeed(50));
  delay(2000);
  Serial.println("Stopped");
  car_Stop();
  delay(1000);
  Serial.println("Left");
  car_Left(getSpeed(50));
  delay(2000);
  Serial.println("Stopped");
  car_Stop();
  delay(1000);
}

// Returns the correct PWM value from given percentage
int getSpeed(int percent){
  return map(percent, 0, 100, 0, 255);
}

// Turns On/Off motors base on the values on the drive_state_matrix
// @param matrix_index Which drive mode to apply (integer value as index into the drive_state_matrix array)
// @param duty_cycle value b/w 0-255 makes the motors spin slow or fast
void activateMotors(int matrix_index, int duty_cycle){
  if(duty_cycle){
    analogWrite(MOTOR_LEFT_SPEED_PIN, duty_cycle);
    analogWrite(MOTOR_RIGHT_SPEED_PIN, duty_cycle);
  } else {
    digitalWrite(MOTOR_LEFT_SPEED_PIN, 0);
    digitalWrite(MOTOR_RIGHT_SPEED_PIN, 0);
  }
  digitalWrite(MOTOR_LEFT_LOGIC1_PIN, drive_state_matrix[matrix_index][0]);
  digitalWrite(MOTOR_LEFT_LOGIC2_PIN, drive_state_matrix[matrix_index][1]);
  digitalWrite(MOTOR_RIGHT_LOGIC1_PIN, drive_state_matrix[matrix_index][2]);
  digitalWrite(MOTOR_RIGHT_LOGIC2_PIN, drive_state_matrix[matrix_index][3]);
}

// Stop the car
void car_Stop(){
  activateMotors(MATRIX_STOP, 0);
}
// Car moves Forward
// @param duty_cycle value b/w 0-255
void car_Forward(int duty_cycle){
  activateMotors(MATRIX_FORWARD, duty_cycle);
}
// Car moves Backward
// @param duty_cycle value b/w 0-255
void car_Back(int duty_cycle){
  activateMotors(MATRIX_BACK, duty_cycle);
}
// Car moves Right
// @param duty_cycle value b/w 0-255
void car_Right(int duty_cycle){
  activateMotors(MATRIX_RIGHT, duty_cycle);
}
// Car moves Left
// @param duty_cycle value b/w 0-255
void car_Left(int duty_cycle){
  activateMotors(MATRIX_LEFT, duty_cycle);
}
