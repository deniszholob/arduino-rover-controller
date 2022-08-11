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
#include <Servo.h> // Servo library

// ================================================================================================================
// Declaring Constants
// ================================================================================================================

// Servo pins
#define PIN_SERVO 10

#define SERVO_RANGE_MAX   155 // Degrees 135deg = 2.0ms PWM
#define SERVO_RANGE_MIN   25  // Degrees 45deg  = 1.0ms PWM
#define SERVO_RANGE_MID   90  // Degrees 90deg  = 1.5ms PWM
#define SERVO_TRIM_OFFSET 8   // Degrees

// Ultrasonics
#define MAX_SENSORS              1 // The number of sensors
#define MAX_RANGE                400
#define DISTANCE_AVG_COUNT       5 // Number of distance values to average
#define DISTANCE_DRIVE_THRESHOLD 40
#define PIN_ULTRASONIC_ECHO      A4 // Echo (yellow)
#define PIN_ULTRASONIC_TRIG      A5 // Trigger (orange)

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

#define CAR_SPEED 150 // 130 Value range 0 - 255

// ================================================================================================================
// Declaring Objects
// ================================================================================================================
Servo myservo; // create servo object to control servo

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

// Ultrasonic Dist M, R, L (middle, right left)
// Index           0  1  2
int distance[3] = {0, 0, 0};
int *stdev_elim_threshold;

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  // Set the defined Ultrasonic pins to the output
  pinMode(PIN_ULTRASONIC_ECHO, INPUT);  // Sets the Echo Pin as an Input
  pinMode(PIN_ULTRASONIC_TRIG, OUTPUT); // Sets the Trig Pin as an Output

  // Set the defined motor pins to the output
  pinMode(PIN_MOTOR_LEFT_SPEED, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_LOGIC1, OUTPUT);
  pinMode(PIN_MOTOR_LEFT_LOGIC2, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_SPEED, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_LOGIC1, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT_LOGIC2, OUTPUT);

  // Set up the standard deviation filter thresholds
  stdev_elim_threshold = malloc(MAX_RANGE * sizeof(int));

  // Create standard deviation thresholds
  createStdevRanges(stdev_elim_threshold);

  // Attach servo on defined pin to servo object
  myservo.attach(PIN_SERVO);

  // Make sure the car is stopped initially.
  car_Stop();
  // Center the Servo
  lookStraight();
}

// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
  obstacleAvoid();
}

// ================================================================================================================
// Helper Functions
// ================================================================================================================

// Algorithm to avoid obstacles in the car path as it moves forward
void obstacleAvoid() {
  // What is the distance ahead
  lookStraight();
  delay(500);
  distance[0] = getUltrasonicDistance();
  Serial.print("    Distance Center: ");
  Serial.println(distance[0]);

  // Check if way is clear if not, check left and right
  if (distance[0] > DISTANCE_DRIVE_THRESHOLD - 10) {
    car_Forward(CAR_SPEED - 30);
  } else {
    car_Stop();
    checkDistances();

    // More distance to the right then left
    if (distance[1] > distance[2] && distance[1] > DISTANCE_DRIVE_THRESHOLD / 2) {
      car_Right(CAR_SPEED + 20);
      delay(80);
    }
    // More distance to the left then right
    else if (distance[2] > distance[1] && distance[2] > DISTANCE_DRIVE_THRESHOLD / 2) {
      car_Left(CAR_SPEED + 20);
      delay(80);
    }
    // Check if way is clear if not, back up
    else if (distance[1] < DISTANCE_DRIVE_THRESHOLD || distance[2] < DISTANCE_DRIVE_THRESHOLD) {
      car_Back(CAR_SPEED);
      delay(500);
      car_Stop();
      checkDistances();

      // More distance to the right then left
      if (distance[1] > distance[2] && distance[1] > DISTANCE_DRIVE_THRESHOLD / 2) {
        car_Right(CAR_SPEED + 10);
        delay(100);
      }
      // More distance to the left then right
      else if (distance[2] > distance[1] && distance[2] > DISTANCE_DRIVE_THRESHOLD / 2) {
        car_Left(CAR_SPEED + 10);
        delay(100);
      }
    }
  }
}

void checkDistances() {
  // What is the distance to the left?
  lookLeft();
  delay(500);
  distance[2] = getUltrasonicDistance();
  Serial.print("  Distance Left: ");
  Serial.println(distance[2]);


  // What is the distance to the right?
  lookRight();
  delay(500);
  distance[1] = getUltrasonicDistance();
  Serial.print("      Distance Right: ");
  Serial.println(distance[1]);
}

void distanceTest() {
  activateUltrasonic();
  float distance = getUltrasonicAverageDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print("    Filtered: ");
  Serial.print(emaFilter(0, distance));
  // Serial.print(emaFilter(0, stdevEliminationFilter(0, distance)));
  Serial.println("cm");
}

/**
  * The standard deviation filter performs a single-point based elimination.
  * If the difference between the last and current readings is greater than the specified value and the last reading was not eliminated,
  * the last reading is returned and the elimination flag is set.
  * Otherwise, the current reading is returned and the elimination flag is unset.
  * The algorithm will never eliminate two successive readings.
  */
int stdevEliminationFilter(int sensor, int current) {
  // Static variables to hold the last value information
  static int last_value[MAX_SENSORS] = {0};
  static short last_eliminated[MAX_SENSORS] = {0};

  // If the last value hasn't been initialized, start now and return the current value
  if (last_value[sensor] == 0) {
    last_value[sensor] = current;
    last_eliminated[sensor] = 0;
    return current;
  }
  // Check if |current - last| is greater than the stdev for the last value
  int difference = abs(current - last_value[sensor]);
  if (difference > stdev_elim_threshold[last_value[sensor]] && last_eliminated[sensor] == 0) {
    last_eliminated[sensor] = 1;
    return last_value[sensor];
  } else {
    last_value[sensor] = current;
    last_eliminated[sensor] = 0;
    return current;
  }
}

/**
  * Helper function that creates the standard deviation thresholds.
  */
void createStdevThreshold(int *threshold, int low, int high, int value) {
  for (int i = low; i < high; i++) {
    threshold[i] = value;
  }
}

/**
  * Helper function that creates the standard deviation ranges.
  */
void createStdevRanges(int *threshold) {
  createStdevThreshold(threshold, 0, 50, 8);
  createStdevThreshold(threshold, 50, 100, 10);
  createStdevThreshold(threshold, 100, 150, 15);
  createStdevThreshold(threshold, 150, 200, 18);
  createStdevThreshold(threshold, 200, 250, 20);
  createStdevThreshold(threshold, 250, 300, 30);
  createStdevThreshold(threshold, 300, 400, 50);

  // Assign the global threshold variable to the pointer parameter
  stdev_elim_threshold = threshold;
}

/**
 * Calculates an exponential moving average given a sensor and the current value.
 */
int emaFilter(int sensor, int current) {
  // Define static variables
  static float alpha = 0.3;
  static float lastema[MAX_SENSORS];
  static short initialized[MAX_SENSORS] = {0};
  float ema;

  // If initialized is set to 0, then we must be on the first observation.
  // Set the returned ema value to the current value and set the initialized flag to true.
  if (initialized[sensor] == 0) {
    ema = current;
    initialized[sensor] = 1;
  }

  // If the initialized flag has already been set, then there was a prior value.
  // We can now use the recursive formula to calculate the EMA.
  else {
    ema = (alpha * current + (1 - alpha) * lastema[sensor]);
  }

  // Set the previous value to the current EMA calculation to be used on the next round.
  lastema[sensor] = ema;

  // Return the EMA as an integer (will truncate decimal)
  return (int)ema;
}

float getUltrasonicAverageDistance() {
  float distanceSum = 0;
  for (int i = 0; i < DISTANCE_AVG_COUNT; i++) {
    distanceSum += getUltrasonicDistance();
  }
  return distanceSum / (float)DISTANCE_AVG_COUNT;
}

float getUltrasonicDistance() {
  activateUltrasonic();
  float distance = calcUltrasonicDistance();
  if (distance > MAX_RANGE)
    distance = MAX_RANGE;
  else if (distance < 0)
    distance = 0;
  return distance;
}

// Calculate distance (cm) from ultrasonic time (us)
// Returns distance in centimeters
float calcUltrasonicDistance() {
  float echoDuration = pulseIn(PIN_ULTRASONIC_ECHO, HIGH); // us (microseconds for 2 way trip), need to divide by 2 to get 1 way distance

  // Distance = Time * Speed
  // m = s * (m/s)
  // cm/us = (m/s) / 10000
  // cm = us * (m/s) / 10000
  // distance = echoDuration / 2 * SPEED_OF_SOUND / 10000;
  // SPEED_OF_SOUND = 340, 1/(340/1000/2) ~ 58
  // distance = echoDuration / 58
  return echoDuration / 58;
}

// Sets the Trigger Pin on HIGH state for 10 micro seconds
void activateUltrasonic() {
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_ULTRASONIC_TRIG, LOW);
}

// Returns the correct PWM value from given percentage
int getSpeed(int percent) {
  return map(percent, 0, 100, 0, 255);
}

// Turns On/Off motors base on the values on the drive_state_matrix
// @param matrix_index Which drive mode to apply (integer value as index into the drive_state_matrix array)
// @param duty_cycle value b/w 0-255 makes the motors spin slow or fast
void activateMotors(int matrix_index, int duty_cycle) {
  if (duty_cycle) {
    analogWrite(PIN_MOTOR_LEFT_SPEED, duty_cycle + 5);
    analogWrite(PIN_MOTOR_RIGHT_SPEED, duty_cycle - 5);
  } else {
    digitalWrite(PIN_MOTOR_LEFT_SPEED, 0);
    digitalWrite(PIN_MOTOR_RIGHT_SPEED, 0);
  }
  digitalWrite(PIN_MOTOR_LEFT_LOGIC1, drive_state_matrix[matrix_index][0]);
  digitalWrite(PIN_MOTOR_LEFT_LOGIC2, drive_state_matrix[matrix_index][1]);
  digitalWrite(PIN_MOTOR_RIGHT_LOGIC1, drive_state_matrix[matrix_index][2]);
  digitalWrite(PIN_MOTOR_RIGHT_LOGIC2, drive_state_matrix[matrix_index][3]);
}

// Stop the car
void car_Stop() {
  activateMotors(CAR_STATE_STOP, 0);
  Serial.println(" STOPPED");
}
// Car moves Forward
// @param duty_cycle value b/w 0-255
void car_Forward(int duty_cycle) {
  activateMotors(CAR_STATE_FORWARD, duty_cycle);
  Serial.println(" FORWARD");
}
// Car moves Backward
// @param duty_cycle value b/w 0-255
void car_Back(int duty_cycle) {
  activateMotors(CAR_STATE_BACK, duty_cycle);
  Serial.println(" REVERSE");
}
// Car moves Right
// @param duty_cycle value b/w 0-255
void car_Right(int duty_cycle) {
  activateMotors(CAR_STATE_RIGHT, duty_cycle);
  Serial.println(" RIGHT");
}
// Car moves Left
// @param duty_cycle value b/w 0-255
void car_Left(int duty_cycle) {
  activateMotors(CAR_STATE_LEFT, duty_cycle);
  Serial.println(" LEFT");
}

// Turns Servo to center
void lookStraight() {
  myservo.write(SERVO_RANGE_MID - SERVO_TRIM_OFFSET); // set servo position according to scaled value
  //  delay(500);                     // Wait half a sec
}

// Turns Servo left
void lookLeft() {
  myservo.write(SERVO_RANGE_MAX - SERVO_TRIM_OFFSET); // set servo position according to scaled value
  //  delay(500);                     // Wait half a sec
}

// Turns Servo Right
void lookRight() {
  myservo.write(SERVO_RANGE_MIN - SERVO_TRIM_OFFSET); // set servo position according to scaled value
  //  delay(500);                     // Wait half a sec
}
