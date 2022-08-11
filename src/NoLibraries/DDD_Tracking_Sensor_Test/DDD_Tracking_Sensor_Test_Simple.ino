/* ===================================
 * Car Line Tracking Sensor Test Program
 * ===================================
 * Prints the tracking sensor values to serial monitor
 *
 * @author Denis Zholob
 */

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include <Arduino.h>

// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================

// Tracking Sensors (IR)
#define PIN_TRACKING_SENSOR_L A0
#define PIN_TRACKING_SENSOR_M A1
#define PIN_TRACKING_SENSOR_R A2

// Indicies for tracking sensor array below
#define SENSOR_TRACKING_LEFT 0
#define SENSOR_TRACKING_MID 1
#define SENSOR_TRACKING_RIGHT 2

// Tracking sensor values   L, M, R
// Index                    0  1  2
int track_sensor_vals[3] = {0, 0, 0};

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  // Tracking IR Sensors
  pinMode(PIN_TRACKING_SENSOR_R,   INPUT);
  pinMode(PIN_TRACKING_SENSOR_M,   INPUT);
  pinMode(PIN_TRACKING_SENSOR_L,   INPUT);
}

// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
  readTrackingSensors();
  printSensorValues();
}

// ================================================================================================================
// Helper Functions
// ================================================================================================================
void readTrackingSensors(){
  track_sensor_vals[SENSOR_TRACKING_LEFT]  = digitalRead(PIN_TRACKING_SENSOR_L);
  track_sensor_vals[SENSOR_TRACKING_MID]   = digitalRead(PIN_TRACKING_SENSOR_M);
  track_sensor_vals[SENSOR_TRACKING_RIGHT] = digitalRead(PIN_TRACKING_SENSOR_R);
}

void printSensorValues(){
  Serial.print("Left: ");
  Serial.print(track_sensor_vals[SENSOR_TRACKING_LEFT]);
  Serial.print("  Mid: ");
  Serial.print(track_sensor_vals[SENSOR_TRACKING_MID]);
  Serial.print("  Right: ");
  Serial.println(track_sensor_vals[SENSOR_TRACKING_RIGHT]);
}
