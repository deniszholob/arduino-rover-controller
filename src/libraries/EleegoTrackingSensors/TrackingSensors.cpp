/* ===================================
 * Car Line Tracking Sensor Library
 * ===================================
 * Contains functions to use with the IR Tracking Sensors
 *
 * @author Denis Zholob
 */

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include "TrackingSensors.h"

#include <Arduino.h>

// ================================================================================================================
// Functions
// ================================================================================================================

// Sets input pins
void TrackingSensorsClass::setTrackingSensorPins() {
  pinMode(PIN_TRACKING_SENSOR_L, INPUT);
  pinMode(PIN_TRACKING_SENSOR_M, INPUT);
  pinMode(PIN_TRACKING_SENSOR_R, INPUT);
}

// Reads data into track_sensor_vals struct values
// Accepts a pointer to a TrackSensorVals type struct
void TrackingSensorsClass::readTrackingSensorValues(TrackSensorVals *track_sensor_vals) {
  track_sensor_vals->L = digitalRead(PIN_TRACKING_SENSOR_L);
  track_sensor_vals->M = digitalRead(PIN_TRACKING_SENSOR_M);
  track_sensor_vals->R = digitalRead(PIN_TRACKING_SENSOR_R);
}

// Prints values in TrackSensorVals struct to Serial
void TrackingSensorsClass::printTrackingSensorValues(TrackSensorVals *track_sensor_vals) {
  Serial.print("Left: ");
  Serial.print(track_sensor_vals->L);
  Serial.print("  Mid: ");
  Serial.print(track_sensor_vals->M);
  Serial.print("  Right: ");
  Serial.println(track_sensor_vals->R);
}
