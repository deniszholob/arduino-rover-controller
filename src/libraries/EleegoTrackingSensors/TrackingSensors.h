/* ===================================
 * Car Line Tracking Sensor Library
 * ===================================
 * Contains functions to use with the IR Tracking Sensors
 *
 * @author Denis Zholob
 */

#ifndef TrackingSensors_h
#define TrackingSensors_h

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include <Arduino.h>

// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================

// Tracking Sensor Pins (IR)
#define PIN_TRACKING_SENSOR_L A0 // Previous Digital 11
#define PIN_TRACKING_SENSOR_M A1 // Previous Digital 4
#define PIN_TRACKING_SENSOR_R A2 // Previous Digital 2

// Structure to hold values for tracking sensors  Left, Middle, Right
struct TrackSensorVals {
    int L;
    int M;
    int R;
};

// Tracking Class holds functions to read and print sensor values
class TrackingSensorsClass {
  public:
    // TrackingSensorsClass();
    // ~TrackingSensorsClass();
    void setTrackingSensorPins();
    void readTrackingSensorValues(TrackSensorVals *track_sensor_vals);
    void printTrackingSensorValues(TrackSensorVals *track_sensor_vals);
};

#endif
