/* ===================================
 * Car Line Traking Sensor Test Program
 * ===================================
 * Prints the traking sensor values to serial monitor
 *
 * @author Denis Zholob
 */

// ================================================================================================================
// Declaring Constants (Magic numbers are BAD!)
// ================================================================================================================

// Traking Sensors (IR)
#define PIN_TRAKING_SENSOR_L A0 //Previous Diginal 11
#define PIN_TRAKING_SENSOR_M A1 //Previous Diginal 4
#define PIN_TRAKING_SENSOR_R A2 //Previous Diginal 2

// Indecies for tracking sensor array below
#define SENSOR_TRAKING_LEFT 0
#define SENSOR_TRAKING_MID 1
#define SENSOR_TRAKING_RIGHT 2

// Tracking sensor valsues  L, M, R
// Index           			0  1  2
int track_sensor_vals[3] = {0, 0, 0};

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  // Traking IR Sensors
  pinMode(PIN_TRAKING_SENSOR_R,   INPUT);
  pinMode(PIN_TRAKING_SENSOR_M,   INPUT);
  pinMode(PIN_TRAKING_SENSOR_L,   INPUT);
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
	track_sensor_vals[SENSOR_TRAKING_LEFT] 	= digitalRead(PIN_TRAKING_SENSOR_L);
	track_sensor_vals[SENSOR_TRAKING_MID] 	= digitalRead(PIN_TRAKING_SENSOR_M);
	track_sensor_vals[SENSOR_TRAKING_RIGHT] = digitalRead(PIN_TRAKING_SENSOR_R);
}

void printSensorValues(){
	Serial.print("Left: ");
	Serial.print(track_sensor_vals[SENSOR_TRAKING_LEFT]);
	Serial.print("  Mid: ");
	Serial.print(track_sensor_vals[SENSOR_TRAKING_MID]);
	Serial.print("  Right: ");
	Serial.println(track_sensor_vals[SENSOR_TRAKING_RIGHT]);
}
