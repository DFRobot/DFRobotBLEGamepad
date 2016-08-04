/*!
* @file morebuttonBlink.ino
* @brief DFRobot's Wireless Gamepad - Arduino with Bluetooth 4.0 (SKU:DFR0304)
* @n [Get the gamepad here](http://www.dfrobot.com/index.php?route=product/product&keyword=wirele&product_id=1170#.V43enPmEA9R)
* @n This example get response of all button.
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

DFRobotBLEGamepad myDFRobotBLEGamepad;                                   
int joystickRightX, joystickRightY;                                      //Right joystick value         
int joystickLeftX, joystickLeftY;                                        //Left joystick value    
boolean buttonState[14];                                                 //buttons buffer
void setup() {
  Serial.begin(115200);                                                 //init the bluetooth Serial port
  myDFRobotBLEGamepad.begin(Serial);    
 }

void loop() {
  if ( myDFRobotBLEGamepad.available() ) 
  {
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
    
	//print the state of Left Joystick
    Serial.print( "Joystick Left Value: " );                                   
    Serial.print( joystickLeftX );
    Serial.print("  ");
    Serial.println( joystickLeftY );
	//print the state of Right Joystick
    Serial.print( "Joystick Right Value: " );                                  
    Serial.print( joystickRightX );
    Serial.print("  ");
    Serial.println( joystickRightY );
    //print the state of buttons
    for ( int i = 0; i < 14; i++ ){
      if (buttonState[i] == PRESSED) {
        Serial.print(" ID: ");
        Serial.print(i);
        Serial.print("\t ");
        Serial.print("Pressed!");
       }
     }
    Serial.println("");
  }
 }
