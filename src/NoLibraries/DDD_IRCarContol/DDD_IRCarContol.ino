/* ===================================
 * Car motor control with IR remote
 * ===================================
 * Contains logic to move motors in different direction
 * v1.0
 * @author Denis Zholob
 *
 * Required Libraries:
 *    Arduino-IRremote by Ken Shirriff
 *    https://github.com/shirriff/Arduino-IRremote
 */

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include "IRremote.h"
 
// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================
#define IR_CODE_UP_ARROW 0xFF629D
#define IR_CODE_DOWN_ARROW 0xFFA857
#define IR_CODE_RIGHT_ARROW 0xFFC23D
#define IR_CODE_LEFT_ARROW 0xFF22DD
#define IR_CODE_OK 0xFF02FD
#define IR_CODE_1 0xFF6897
#define IR_CODE_2 0xFF9867
#define IR_CODE_3 0xFFB04F
#define IR_CODE_4 0xFF30CF
#define IR_CODE_5 0xFF18E7
#define IR_CODE_6 0xFF7A85
#define IR_CODE_7 0xFF10EF
#define IR_CODE_8 0xFF38C7
#define IR_CODE_9 0xFF5AA5
#define IR_CODE_0 0xFF4AB5
#define IR_CODE_STAR 0xFF42BD
#define IR_CODE_POUND 0xFF52AD
#define IR_CODE_REPEAT 0xFFFFFFFF

#define IR_CODE_INPUT_PIN 12

#define MOTOR_LEFT_SPEED_PIN 5    // ENA
#define MOTOR_LEFT_LOGIC1_PIN 6   // IN1
#define MOTOR_LEFT_LOGIC2_PIN 7   // IN2
#define MOTOR_RIGHT_LOGIC1_PIN 8  // IN3
#define MOTOR_RIGHT_LOGIC2_PIN 9  // IN4
#define MOTOR_RIGHT_SPEED_PIN 10  // ENB

// Indecies for drive matrix below and car state
#define CAR_STATE_STOP 0
#define CAR_STATE_FORWARD 1
#define CAR_STATE_BACK 2
#define CAR_STATE_RIGHT 3
#define CAR_STATE_LEFT 4

// ================================================================================================================
// Declaring Objects
// ================================================================================================================
IRrecv irrecv(IR_CODE_INPUT_PIN);  // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'

// ================================================================================================================
// Declaring Variables
// ================================================================================================================

// IN1, IN2, IN3, IN4
const int drive_state_matrix[5][4] = {
  {0, 0, 0, 0}, // CAR_STATE_STOP
  {0, 1, 0, 1}, // CAR_STATE_FORWARD: Left and Right go same direction
  {1, 0, 1, 0}, // CAR_STATE_BACK: Left and right go same direction
  {0, 1, 1, 0}, // CAR_STATE_RIGHT: Left forward, Right Back
  {1, 0, 0, 1}  // CAR_STATE_LEFT: Right forward, Left Back
};

const int turn_speed_matrix[] = {
  5, 10, 15, 20, 25, 30, 35, 40, 45, 50
};

int turn_speed_percent = turn_speed_matrix[0];

int curState = CAR_STATE_STOP;  // Current car drive state, stopped by default
int prevState = CAR_STATE_STOP; // Previous car drive state, stopped by default

#define timeout_delay 250000  // milliseconds - I'm guessing here, this needs to be longer than the IR repeat interval 
unsigned long loop_timer;

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
  pinMode(MOTOR_RIGHT_LOGIC2_PIN,  OUTPUT);

  // Make sure the car is stopped initially.
  car_Stop();

  // Start IR Reciever
  irrecv.enableIRIn();
  loop_timer = micros();                             // Set the loop_timer for the first loop.
}

// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
//  while(loop_timer + timeout_delay > micros());               // Start the pulse after IR_REFRESH_TIME micro seconds.
//  loop_timer = micros();                                     // Reset the zero timer.
  irLogic();
  statefullCarMotion();
  Serial.print("STATE: ");
  Serial.println(curState);
}

// ================================================================================================================
// Helper Functions
// ================================================================================================================

void irLogic(){
  if (irrecv.decode(&results)) {  // have we received an IR signal?
    // RED=results.value;
    // Serial.println(RED);

    switch(results.value){
      case IR_CODE_UP_ARROW:
        curState = CAR_STATE_FORWARD;
        loop_timer = micros();
        Serial.println("IR FORWARD");
        break;
      case IR_CODE_DOWN_ARROW:
        curState = CAR_STATE_BACK;
        loop_timer = micros();
        Serial.println("IR REVERSE");
        break;
      case IR_CODE_RIGHT_ARROW:
        curState = CAR_STATE_RIGHT;
        loop_timer = micros();
        Serial.println("IR RIGHT");
        break;
      case IR_CODE_LEFT_ARROW:
        curState = CAR_STATE_LEFT;
        loop_timer = micros();
        Serial.println("IR LEFT");
        break;
      case IR_CODE_OK:           Serial.println("IR OK");      break;
      case IR_CODE_1:
        turn_speed_percent = turn_speed_matrix[0];
        Serial.println("IR 1");
        break;
      case IR_CODE_2:
        turn_speed_percent = turn_speed_matrix[1];
        Serial.println("IR 2");
        break;
      case IR_CODE_3:
        turn_speed_percent = turn_speed_matrix[2];
        Serial.println("IR 3");
        break;
      case IR_CODE_4:
        turn_speed_percent = turn_speed_matrix[3];
        Serial.println("IR 4");
        break;
      case IR_CODE_5:
        turn_speed_percent = turn_speed_matrix[4];
        Serial.println("IR 5");
        break;
      case IR_CODE_6:
        turn_speed_percent = turn_speed_matrix[5];
        Serial.println("IR 6");
        break;
      case IR_CODE_7:
        turn_speed_percent = turn_speed_matrix[6];
        Serial.println("IR 7");
        break;
      case IR_CODE_8:
        turn_speed_percent = turn_speed_matrix[7];
        Serial.println("IR 8");
        break;
      case IR_CODE_9:
        turn_speed_percent = turn_speed_matrix[8];
        Serial.println("IR 9");
        break;
      case IR_CODE_0:
        turn_speed_percent = turn_speed_matrix[9];
        Serial.println("IR 0");
        break;
      case IR_CODE_STAR:         Serial.println("IR *");       break;
      case IR_CODE_POUND:        Serial.println("IR #");       break;
      case IR_CODE_REPEAT:
        if(curState == CAR_STATE_FORWARD ||
          curState == CAR_STATE_BACK ||
          curState == CAR_STATE_RIGHT ||
          curState == CAR_STATE_LEFT ){
          loop_timer = micros();
        }
        Serial.println("IR REPEAT");
        break;
      default:                   Serial.println("IR other button  ");
    }
    irrecv.resume();              // receive the next value
  } 
//  else { 
//    Serial.println("NO IR");
//    prevState = curState;
//    curState = CAR_STATE_STOP;
//  }
  if(micros() - loop_timer > timeout_delay){
//    Serial.println("TIMEOUT");
    curState = CAR_STATE_STOP;
  }
}

void statefullCarMotion(){
  switch(curState){
    case CAR_STATE_FORWARD: car_Forward(110);  break;
    case CAR_STATE_BACK:    car_Back(255);     break;
    case CAR_STATE_RIGHT:   car_Right(255-(255*turn_speed_percent/100));  break;
    case CAR_STATE_LEFT:    car_Left(255-(255*turn_speed_percent/100));     break;
    case CAR_STATE_STOP:    car_Stop();                       break;
    default:                car_Stop();
  }
}

// Preset of commands to move the car
void autoCarMotion(){
  Serial.println("Stopped");
  car_Stop();
  delay(1000);
  Serial.println("Forward");
  car_Forward(getSpeed(100));
  delay(2000);
  Serial.println("Stopped");
  car_Stop();
  delay(1000);
  Serial.println("Back");
  car_Back(getSpeed(100));
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

// Truns On/Off motors base on the valus on the drive_state_matrix
// @param matrix_index Which drive mode to apply (integer value as index into the drive_state_matrix array)
// @param duty_cycle value b/w 0-255 makes the motors spin slow or fast
void activateMotors(int matrix_index, int duty_cycle){
  if(duty_cycle){
    analogWrite(MOTOR_LEFT_SPEED_PIN,  duty_cycle);
    analogWrite(MOTOR_RIGHT_SPEED_PIN, duty_cycle);
  }else{
    digitalWrite(MOTOR_LEFT_SPEED_PIN,  0);
    digitalWrite(MOTOR_RIGHT_SPEED_PIN, 0);
  }
  digitalWrite(MOTOR_LEFT_LOGIC1_PIN,  drive_state_matrix[matrix_index][0]);
  digitalWrite(MOTOR_LEFT_LOGIC2_PIN,  drive_state_matrix[matrix_index][1]);
  digitalWrite(MOTOR_RIGHT_LOGIC1_PIN, drive_state_matrix[matrix_index][2]);
  digitalWrite(MOTOR_RIGHT_LOGIC2_PIN, drive_state_matrix[matrix_index][3]);
}

// Stop the car
void car_Stop(){
  activateMotors(CAR_STATE_STOP, 0);
  Serial.println(" STOPPED");
}
// Car moves Forward
// @param duty_cycle value b/w 0-255
void car_Forward(int duty_cycle){
  activateMotors(CAR_STATE_FORWARD, duty_cycle);
  Serial.println(" FORWARD");
}
// Car moves Backward
// @param duty_cycle value b/w 0-255
void car_Back(int duty_cycle){
  activateMotors(CAR_STATE_BACK, duty_cycle);
  Serial.println(" REVERSE");
}
// Car moves Right
// @param duty_cycle value b/w 0-255
void car_Right(int duty_cycle){
  activateMotors(CAR_STATE_RIGHT, duty_cycle);
  Serial.println(" RIGHT");
}
// Car moves Left
// @param duty_cycle value b/w 0-255
void car_Left(int duty_cycle){
  activateMotors(CAR_STATE_LEFT, duty_cycle);
  Serial.println(" LEFT");
}

