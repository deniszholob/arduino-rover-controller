/* ===================================
 * IR remote Test
 * ===================================
 * Tests the buttons on the IR remote and prints output to Serial monitor
 * v1.0
 * @author Denis Zholob
 * based on code from https://arduino-info.wikispaces.com/IR-RemoteControl
 *
 * Required Libraries:
 *    Arduino-IRremote by Ken Shirriff
 *    https://github.com/shirriff/Arduino-IRremote
 */

// ================================================================================================================
// Importing Libraries
// ================================================================================================================
#include <Arduino.h>
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

// ================================================================================================================
// Declaring Objects
// ================================================================================================================
IRrecv irrecv(IR_CODE_INPUT_PIN);  // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'

// ================================================================================================================
// Setup routine: Runs once when you press reset or power on the board
// ================================================================================================================
void setup() {
  // Open the serial port and set the baud rate to 9600
  Serial.begin(9600);

  Serial.println("Arduino IR Receiver Button Decode Test");

  // Start IR Reciever
  irrecv.enableIRIn();
}

// ================================================================================================================
// Main program loop: Runs over and over again forever
// ================================================================================================================
void loop() {
  if (irrecv.decode(&results)) {          // have we received an IR signal?
    // Serial.println(results.value, HEX);   // UNcomment to see raw values
    translateIR();                        // Prints button press
    irrecv.resume();                      // receive the next value
  }
}

// ================================================================================================================
// Helper Functions
// ================================================================================================================

// takes action based on IR code received
void translateIR() {
  // describing KEYS Remote IR codes
  switch(results.value){
    case IR_CODE_UP_ARROW:     Serial.println(" FORWARD"); break;
    case IR_CODE_DOWN_ARROW:   Serial.println(" REVERSE"); break;
    case IR_CODE_RIGHT_ARROW:  Serial.println(" RIGHT");   break;
    case IR_CODE_LEFT_ARROW:   Serial.println(" LEFT");    break;
    case IR_CODE_OK:           Serial.println(" OK");      break;
    case IR_CODE_1:            Serial.println(" 1");       break;
    case IR_CODE_2:            Serial.println(" 2");       break;
    case IR_CODE_3:            Serial.println(" 3");       break;
    case IR_CODE_4:            Serial.println(" 4");       break;
    case IR_CODE_5:            Serial.println(" 5");       break;
    case IR_CODE_6:            Serial.println(" 6");       break;
    case IR_CODE_7:            Serial.println(" 7");       break;
    case IR_CODE_8:            Serial.println(" 8");       break;
    case IR_CODE_9:            Serial.println(" 9");       break;
    case IR_CODE_0:            Serial.println(" 0");       break;
    case IR_CODE_STAR:         Serial.println(" *");       break;
    case IR_CODE_POUND:        Serial.println(" #");       break;
    case IR_CODE_REPEAT:       Serial.println(" REPEAT");  break;

    default:                   Serial.println(" other button  ");
  }// End Case

//  delay(500); // Delay a bit so as not to get immediate repeat
}
