# Double check these pins match in code and board

| Sensor                        | Pin   |
|-------------------------------|-------|
| Tracking Sensor L             | A0    |
| Tracking Sensor M             | A1    |
| Tracking Sensor R             | A2    |
|
| Ultrasonic Echo - yellow      | A4    |
| Ultrasonic Trigger - orange   | A5    |
|
| M_A-Enable (gray)    (PWM)    |  5    |
| M_A-In1    (purple)           |  2    |
| M_A-In2    (blue)             |  3    |
| M_B-In3    (green)            |  9    |
| M_B-In4    (yellow)           | 10    |
| M_B-Enable (orange)  (PWM)    |  6    |
|
| Servo (y/r/b)                 | 11    |
|
| IR Sensor (w/r/b)             | 12    |
|
| RC PWM Ch1 (Roll)             |  4    |
| RC PWM Ch2 (Pitch)            |  7    |
| RC PWM Ch3 (Throttle)         |  8    |
| RC PWM Ch4 (Yaw)              | 12    |
|
| IMU                           |  2    |

# Notes:
* Pin 0 and 1 are Rx/Tx reserved for usb, and do not work unless specifically disabled https://forum.arduino.cc/t/digitalwrite-pins-0-and-1/24691/6
* Servo library disabled PWM on pins 10 and 9 https://www.arduino.cc/reference/en/libraries/servo/
* analogWrite() uses PWM pins https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/

PWM Pins (Output PDW => servo/esc control/etc...)
11   PCINT3    PB3
10   PCINT2    PB2
9    PCINT1    PB1
6    PCINT22   PD6
5    PCINT21   PD5
3    PCINT19   PD3

Non PWM Pins (Can use to read PWM)
13   PCINT5    PB5   // LED
12   PCINT4    PB4
8    PCINT0    PB0
7    PCINT23   PD7
4    PCINT20   PD4
2    PCINT18   PD2   // Interrupt pin (for IMU)

Analog Pins
A0   PCINT8    PC5
A1   PCINT9    PC4
A2   PCINT10   PC3
A3   PCINT11   PC2
A4   PCINT12   PC1   // SDA
A5   PCINT13   PC0   // SCL
