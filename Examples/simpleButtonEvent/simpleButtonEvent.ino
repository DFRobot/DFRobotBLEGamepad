/*!
* @file morebuttonBlink.ino
* @brief DFRobot's Wireless Gamepad - Arduino with Bluetooth 4.0 (SKU:DFR0304)
* @n [Get the gamepad here](http://www.dfrobot.com/index.php?route=product/product&keyword=wirele&product_id=1170#.V43enPmEA9R)
* @n This example call the button callback function to return pressed state.
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

DFRobotBLEGamepad myDFRobotBLEGamepad; 										 //init the bluetooth Serial port

int joystickRightX, joystickRightY;                                          //Right joystick value
int joystickLeftX, joystickLeftY; 					                         //Left joystick value
	
const int ledPin = 13;													     //the pin of the pilot lamp
boolean ledState = LOW;													     //define LED state flag

void setup(){
  Serial.begin(115200);
  myDFRobotBLEGamepad.begin(Serial);    		
  
	//init LED
  pinMode(ledPin,OUTPUT);	
	
	//default pinMode is LOW
  digitalWrite(ledPin,LOW);
  
  myDFRobotBLEGamepad.ButtonUpIsPressed( switchUpEvent );  			        // call the custom switch event function
  myDFRobotBLEGamepad.ButtonDownIsPressed( switchDownEvent );				// call the custom switch event function
  myDFRobotBLEGamepad.ButtonLeftIsPressed( switchLeftEvent );				// call the custom switch event function
  myDFRobotBLEGamepad.ButtonRightIsPressed( switchRightEvent );			    // call the custom switch event function
  myDFRobotBLEGamepad.ButtonLeftF1IsPressed( switchLeftF1Event );		    // call the custom switch event function
  myDFRobotBLEGamepad.ButtonLeftF2IsPressed( switchLeftF2Event );		    // call the custom switch event function
  myDFRobotBLEGamepad.ButtonLeftStickIsPressed( switchLeftStickEvent );        // call the custom switch event function
  myDFRobotBLEGamepad.Button4IsPressed( switch4Event );                     // call the custom switch event function
  myDFRobotBLEGamepad.Button2IsPressed( switch2Event );                     // call the custom switch event function
  myDFRobotBLEGamepad.Button1IsPressed( switch1Event );                     // call the custom switch event function
  myDFRobotBLEGamepad.Button3IsPressed( switch3Event );                     // call the custom switch event function
  myDFRobotBLEGamepad.ButtonRightF1IsPressed( switchRightF1Event );         // call the custom switch event function
  myDFRobotBLEGamepad.ButtonRightF2IsPressed( switchRightF2Event );         // call the custom switch event function
  myDFRobotBLEGamepad.ButtonRightStickIsPressed( switchRightStickEvent );        // call the custom switch event function
}

void loop() {
  /*
	 * check if any new command from bluetooth communication
	 * Note: available function must be used in the loop function to keep the serial data receiver running
   *	     Without this function, whole features will be invalid
	 */
  if( myDFRobotBLEGamepad.available() ){
    joystickLeftX = myDFRobotBLEGamepad.readJoystickLeftX();
    joystickLeftY = myDFRobotBLEGamepad.readJoystickLeftY();
  }
  
  digitalWrite(ledPin,ledState);											//write the latest status of the LED
}

/*
 *if button-Up is pressed,the system will call this function
 */
void switchUpEvent( void ){
  ledState = !ledState;														//change the LED state
  Serial.println("Trigger Switch ButtonUp Event!");						    //print the debug log
}

/*
 *if button-Down is pressed,the system will call this function
 */
void switchDownEvent( void ){
  ledState = !ledState;														//change the LED state
  Serial.println("Trigger Switch ButtonDown Event!");						//print the debug log
}

/*
 *if button-Left is pressed,the system will call this function
 */
void switchLeftEvent( void ){
  ledState = !ledState;														//change the LED state
  Serial.println("Trigger Switch ButtonLeft Event!");						//print the debug log
}

/*
 *if button-Right is pressed,the system will call this function
 */
void switchRightEvent( void ){
  ledState = !ledState;													    //change the LED state
  Serial.println("Trigger Switch ButtonRight Event!");						//print the debug log
}

/*
 *if button-LeftF1 is pressed,the system will call this function
 */
void switchLeftF1Event( void ){
  ledState = !ledState;														//change the LED state
  Serial.println("Trigger Switch ButtonLeftF1 Event!");						//print the debug log
}

/*
 *if button-LeftF2 is pressed,the system will call this function
 */
void switchLeftF2Event( void ){
  ledState = !ledState;														//change the LED state
  Serial.println("Trigger Switch ButtonLeftF2 Event!");						//print the debug log
}
/*
 *if button-LeftStick is pressed,the system will call this function
 */
void switchLeftStickEvent( void ){
  ledState = !ledState;                            //change the LED state
  Serial.println("Trigger Switch ButtonLeftStick Event!");           //print the debug log
}

/*
 *if button-4 is pressed,the system will call this function
 */
void switch4Event( void ){
  ledState = !ledState;                                                    //change the LED state
  Serial.println("Trigger Switch Button4 Event!");                         //print the debug log
}

/*
 *if button-2 is pressed,the system will call this function
 */
void switch2Event( void ){
  ledState = !ledState;                                                    //change the LED state
  Serial.println("Trigger Switch Button2 Event!");                         //print the debug log
}

/*
 *if button-1 is pressed,the system will call this function
 */
void switch1Event( void ){
  ledState = !ledState;                                                   //change the LED state
  Serial.println("Trigger Switch Button1 Event!");                        //print the debug log
}

/*
 *if button-3 is pressed,the system will call this function
 */
void switch3Event( void ){
  ledState = !ledState;                                                  //change the LED state
  Serial.println("Trigger Switch Button3 Event!");                       //print the debug log
}

/*
 *if button-RightF1 is pressed,the system will call this function
 */
void switchRightF1Event( void ){
  ledState = !ledState;                                                 //change the LED state
  Serial.println("Trigger Switch ButtonRightF1 Event!");                //print the debug log
}

/*
 *if button-RightF2 is pressed,the system will call this function
 */
void switchRightF2Event( void ){
  ledState = !ledState;                                                //change the LED state
  Serial.println("Trigger Switch ButtonRightF2 Event!");               //print the debug log
}
/*
 *if button-RightStick is pressed,the system will call this function
 */
void switchRightStickEvent( void ){
  ledState = !ledState;                            //change the LED state
  Serial.println("Trigger Switch ButtonRightStick Event!");           //print the debug log
}

