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
#include "TrackingSensors.h"

// ================================================================================================================
// Declaring Objects
// ================================================================================================================
TrackingSensorsClass Tracking_Sensors_Obj;
TrackSensorVals track_sensor_vals;

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  // Tracking IR Sensors
  Tracking_Sensors_Obj.setTrackingSensorPins();
}

// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
  Tracking_Sensors_Obj.readTrackingSensorValues(&track_sensor_vals);
  Tracking_Sensors_Obj.printTrackingSensorValues(&track_sensor_vals);
}

