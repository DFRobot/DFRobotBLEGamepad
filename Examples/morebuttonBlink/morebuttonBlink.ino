/*!
* @file morebuttonBlink.ino
* @brief DFRobot's Wireless Gamepad - Arduino with Bluetooth 4.0 (SKU:DFR0304)
* @n [Get the gamepad here](http://www.dfrobot.com/index.php?route=product/product&keyword=wirele&product_id=1170#.V43enPmEA9R)
* @n This example get response of all buttons and make LED blink.
* @n [Schematics]([http://www.dfrobot.com/wiki/index.php?title=BLE_Wireless_Gamepad_V2_SKU:_DFR0304])
*
* @copyright  [DFRobot](http://www.dfrobot.com), 2016
* @copyright  GNU Lesser General Public License
*
* @author [Jeffrey](Angelo.qiao@dfrobot.com)
* @version  V2.0
* @date  2016-07-18
*/
#include "DFRobotBLEGamepad.h"

#define SWITCH_UP   0
#define SWITCH_DOWN   1
#define SWITCH_LEFT   2
#define SWITCH_RIGHT   3
#define SWITCH_LEFT_F1  4
#define SWITCH_LEFT_F2  5
#define SWITCH_LEFT_STICK  6

#define SWITCH_4   7
#define SWITCH_2   8
#define SWITCH_1   9
#define SWITCH_3   10
#define SWITCH_RIGHT_F1  11
#define SWITCH_RIGHT_F2  12
#define SWITCH_RIGHT_STICK  13

DFRobotBLEGamepad myDFRobotBLEGamepad;                                          //init the bluetooth Serial port
int joystickRightX, joystickRightY;                                             //Right joystick value
int joystickLeftX, joystickLeftY;                                               //Left joystick value
boolean buttonState[14];                                                        //buttons buffer

const int ledPin = 13;                                                          //the pin of the pilot lamp

void setup() {
  Serial.begin(115200);
  myDFRobotBLEGamepad.begin(Serial);     
  pinMode( ledPin, OUTPUT );                                                    //init LED
  digitalWrite( ledPin, LOW );                                                  //default pinMode is LOW
}

void loop() {
  /*
   * check if any new command from bluetooth communication
   * Note: available function must be used in the loop function to keep the serial data receiver running
   *       Without this function, whole features will be invalid
   */
 if ( myDFRobotBLEGamepad.available() ) {
    Serial.println("received");
	//get the joystick value
    joystickRightX = myDFRobotBLEGamepad.readJoystickRightX();                   
    joystickRightY = myDFRobotBLEGamepad.readJoystickRightY();                   
    joystickLeftX = myDFRobotBLEGamepad.readJoystickLeftX();                     
    joystickLeftY = myDFRobotBLEGamepad.readJoystickLeftY();

    //read button state when there's valid command from bluetooth
    buttonState[SWITCH_UP] = myDFRobotBLEGamepad.readSwitchUp();
    buttonState[SWITCH_DOWN] = myDFRobotBLEGamepad.readSwitchDown();
    buttonState[SWITCH_LEFT] = myDFRobotBLEGamepad.readSwitchLeft();
    buttonState[SWITCH_RIGHT] = myDFRobotBLEGamepad.readSwitchRight();
    buttonState[SWITCH_LEFT_F1] = myDFRobotBLEGamepad.readSwitchLeftF1();
    buttonState[SWITCH_LEFT_F2] = myDFRobotBLEGamepad.readSwitchLeftF2();
    buttonState[SWITCH_LEFT_STICK] = myDFRobotBLEGamepad.readSwitchLeftStick();

    buttonState[SWITCH_1] = myDFRobotBLEGamepad.readSwitch1();
    buttonState[SWITCH_2] = myDFRobotBLEGamepad.readSwitch2();
    buttonState[SWITCH_3] = myDFRobotBLEGamepad.readSwitch3();
    buttonState[SWITCH_4] = myDFRobotBLEGamepad.readSwitch4();
    buttonState[SWITCH_RIGHT_F1] = myDFRobotBLEGamepad.readSwitchRightF1();
    buttonState[SWITCH_RIGHT_F2] = myDFRobotBLEGamepad.readSwitchRightF2();
    buttonState[SWITCH_RIGHT_STICK] = myDFRobotBLEGamepad.readSwitchRightStick();
    

    Serial.print( "Joystick Left Value: " );                                     //debug bluetooth data received
    Serial.print( joystickLeftX );
    Serial.print("  ");
    Serial.println( joystickLeftY );

    Serial.print( "Joystick Right Value: " );                                    //debug bluetooth data received
    Serial.print( joystickRightX );
    Serial.print("  ");
    Serial.println( joystickRightY );
    
    for ( int i = 0; i < 14; i++ ) {
      if (buttonState[i] == PRESSED) {
        Serial.print(" ID: ");
        Serial.print(i+1);
        Serial.print("\t ");
        Serial.print("Pressed!");
      }
     }
Serial.println("");
    
    // When 4 buttons of the control panel is pressed, illuminate the onboard LED of the Arduino!
    // Turn off the LED when none button pressed!
    if (buttonState[SWITCH_UP] == PRESSED ||
        buttonState[SWITCH_DOWN] == PRESSED ||
        buttonState[SWITCH_LEFT] == PRESSED ||
        buttonState[SWITCH_RIGHT] == PRESSED) {
      digitalWrite(ledPin, HIGH);
    }else
      digitalWrite(ledPin, LOW);
  }
}
