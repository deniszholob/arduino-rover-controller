/* ===================================
 * Car Motor Direction Test Program
 * ===================================
 * Contains logic to move motors in different direction
 *
 * @author Denis Zholob
 */
 
// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================
#define MOTOR_LEFT_SPEED_PIN 5    // ENA
#define MOTOR_LEFT_LOGIC1_PIN 6   // IN1
#define MOTOR_LEFT_LOGIC2_PIN 7   // IN2
#define MOTOR_RIGHT_LOGIC1_PIN 8  // IN3
#define MOTOR_RIGHT_LOGIC2_PIN 9  // IN4
#define MOTOR_RIGHT_SPEED_PIN 10  // ENB

// Indecies for drive matrix below
#define MATRIX_STOP 0
#define MATRIX_FORWARD 1
#define MATRIX_BACK 2
#define MATRIX_RIGHT 3
#define MATRIX_LEFT 4

// IN1, IN2, IN3, IN4
const int drive_matrix[5][4] = {
  {0, 0, 0, 0}, // MATRIX_STOP
  {0, 1, 0, 1}, // MATRIX_FORWARD: Left and Right go same direction
  {1, 0, 1, 0}, // MATRIX_BACK: Left and right go same direction
  {0, 1, 1, 0}, // MATRIX_RIGHT: Left forward, Right Back
  {1, 0, 0, 1}  // LMATRIX_LEFT: Right forward, Left Back
};

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  //Open the serial port and set the baud rate to 9600
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

// Truns On/Off motors base on the valus on the drive_matrix
// @param matrix_index Which drive mode to apply (integer value as index into the drive_matrix array)
// @param duty_cycle value b/w 0-255 makes the motors spin slow or fast
void activateMotors(int matrix_index, int duty_cycle){
  if(duty_cycle){
    analogWrite(MOTOR_LEFT_SPEED_PIN, duty_cycle);
    analogWrite(MOTOR_RIGHT_SPEED_PIN, duty_cycle);
  }else{
    digitalWrite(MOTOR_LEFT_SPEED_PIN, 0);
    digitalWrite(MOTOR_RIGHT_SPEED_PIN, 0);
  }
  digitalWrite(MOTOR_LEFT_LOGIC1_PIN, drive_matrix[matrix_index][0]);
  digitalWrite(MOTOR_LEFT_LOGIC2_PIN, drive_matrix[matrix_index][1]);
  digitalWrite(MOTOR_RIGHT_LOGIC1_PIN, drive_matrix[matrix_index][2]);
  digitalWrite(MOTOR_RIGHT_LOGIC2_PIN, drive_matrix[matrix_index][3]);
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

