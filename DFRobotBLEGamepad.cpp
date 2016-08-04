/*!
* @file DFRobotBLEGamepad.cpp
* @brief DFRobot's Wireless Gamepad - Arduino with Bluetooth 4.0 (SKU:DFR0304)
* @n This is the library for DFRobot's Wireless Gamepad V2.0
* @n [Schematics]([http://www.dfrobot.com/wiki/index.php?title=BLE_Wireless_Gamepad_V2_SKU:_DFR0304])
*
* @copyright	[DFRobot](http://www.dfrobot.com), 2016
* @copyright	GNU Lesser General Public License
*
* @author [Jeffrey](Angelo.qiao@dfrobot.com)
* @version  V2.0
* @date  2016-07-18
*/

#include "DFRobotBLEGamepad.h"
#include <Arduino.h>
#include "string.h"
/*
 * Call Constructor
 */
 
//DFRobotBLEGamepad::DFRobotBLEGamepad(HardwareSerial& theSerial ):bleSerial( theSerial ){}

DFRobotBLEGamepad::DFRobotBLEGamepad(){}
/*
 * Initialize Class Variables 
 */
void (*DFRobotBLEGamepad::userButtonUpCallback)(void);
void (*DFRobotBLEGamepad::userButtonDownCallback)(void);
void (*DFRobotBLEGamepad::userButtonLeftCallback)(void);
void (*DFRobotBLEGamepad::userButtonRightCallback)(void);
void (*DFRobotBLEGamepad::userButtonLeftF1Callback)(void);
void (*DFRobotBLEGamepad::userButtonLeftF2Callback)(void);
void (*DFRobotBLEGamepad::userButtonLeftStickCallback)(void);

void(*DFRobotBLEGamepad::userButton4Callback)(void);
void(*DFRobotBLEGamepad::userButton2Callback)(void);
void(*DFRobotBLEGamepad::userButton1Callback)(void);
void(*DFRobotBLEGamepad::userButton3Callback)(void);
void(*DFRobotBLEGamepad::userButtonRightF1Callback)(void);
void(*DFRobotBLEGamepad::userButtonRightF2Callback)(void);                  
void(*DFRobotBLEGamepad::userButtonRightStickCallback)(void);

/*
 * User configuration for the baudrate of the BLE communication
 */
void DFRobotBLEGamepad::begin(Stream& theSerial) {
	bleSerial = &theSerial;
	debugSerial.begin(115200);															// config the default debug Serial port
    initRecvDataPack();
  
  _joystickRightX = 127;
  _joystickRightY = 127;
  _joystickLeftX = 127;                                                                        
  _joystickLeftY = 127;                                                                       

  rDataPack.prbpayType.switchButtonUp = RELEASED;
  rDataPack.prbpayType.switchButtonDown = RELEASED;
  rDataPack.prbpayType.switchButtonLeft = RELEASED;
  rDataPack.prbpayType.switchButtonRight = RELEASED;
  rDataPack.prbpayType.switchButtonLeftF1 = RELEASED;
  rDataPack.prbpayType.switchButtonLeftF2 = RELEASED;
  rDataPack.prbpayType.switchButtonLeftStick = RELEASED;

  rDataPack.prbpayType.switchButton4 = RELEASED;
  rDataPack.prbpayType.switchButton2 = RELEASED;
  rDataPack.prbpayType.switchButton1 = RELEASED;
  rDataPack.prbpayType.switchButton3 = RELEASED;
  rDataPack.prbpayType.switchButtonRightF1 = RELEASED;
  rDataPack.prbpayType.switchButtonRightF2 = RELEASED;  
  rDataPack.prbpayType.switchButtonRightStick = RELEASED;

  memset(bleDataArray, 0x00, sizeof(bleDataArray));
  bleDataIndex = 0;
}

/*
 * Function: Check if any new commands valid from BLE communication
 * True 	- new command received
 * False	- no new command
 */
  
boolean DFRobotBLEGamepad::available() {
 /*
	* function introduction:
	* push the new valid data to the data buffer package
	* throw away the invalid byte
	* parse the data package when the command length is matching the protocol
  */
	byte inputByte = 0x00;
 while (bleSerial->available())
	{	
		inputByte = bleSerial->read();
		if (inputByte == DEFAULTHEADER1)
		{
			memset(bleDataArray, 0x00, sizeof(bleDataArray));          // Reset the BLEdataArraay
			bleDataIndex = 0;
			rDataPack.commandFlag = true;
			bleDataArray[bleDataIndex] = inputByte;
			bleDataIndex++;
		}
		else if(rDataPack.commandFlag == true)
		{
			bleDataArray[bleDataIndex] = inputByte;
			bleDataIndex++;
			if (bleDataIndex == DEFAULTPACKLENGTH)
				break;
		}
	}

 if (bleDataIndex < 14)   return false;

  /*
   * print the raw data package pushed to the queue.
   * It will disable the later functions!!! So please don't print it if it's not necessary for you
	 */
#if DEBUGDATARAW
  if ( DEBUGDATARAW ) {
    debugSerial.println(F("DFRobotBLEGamepad availalbe -> new data package!"));
    for ( int i = 0; i < rDataPack.commandLength; i++ ) {
      debugSerial.print(bleDataArray[i], HEX);
    }
    debugSerial.println();
  }
#endif  
  //print the data package length 
#if DEBUGPARSER
  if (DEBUGPARSER) {
    debugSerial.print( F("DFRobotBLEGamepad availalbe -> bleQueue Counter: ") );
    debugSerial.print(sizeof(bleDataArray));
    debugSerial.println();
  }
#endif

  /*
   * check if the length and command Flag is valid, then parse the new command package received
	 */
  if ( rDataPack.commandFlag ) {
	  
	  rDataPack.parseState = bleDataPackageParser();												//parse the command package
    if(rDataPack.parseState == PARSESUCCESS){                                                       //if parse successfully 
		//debugSerial.println(F("parse sucess"));
		updateJoystickVal();  																														//update the variables
	  
		// if press the button, trigger the user button event functions
		if( ( rDataPack.bpayType.switchButtonUp == PRESSED ) && 
				userButtonUpCallback ){
				userButtonUpCallback();	
				rDataPack.bpayType.switchButtonUp = RELEASED;
			}				
		if(( rDataPack.bpayType.switchButtonDown == PRESSED ) &&
				userButtonDownCallback )  {
				userButtonDownCallback();	
				rDataPack.bpayType.switchButtonDown = RELEASED;
				}
		if( ( rDataPack.bpayType.switchButtonLeft == PRESSED ) &&
				userButtonLeftCallback )   {
				userButtonLeftCallback();
				rDataPack.bpayType.switchButtonLeft = RELEASED;
				}				
		if( ( rDataPack.bpayType.switchButtonRight == PRESSED ) &&
				userButtonRightCallback )   {
				userButtonRightCallback();	
				rDataPack.bpayType.switchButtonRight = RELEASED;
				}
		if( ( rDataPack.bpayType.switchButtonLeftF1 == PRESSED) && 
				userButtonLeftF1Callback )  {
				userButtonLeftF1Callback();	
				rDataPack.bpayType.switchButtonLeftF1 = RELEASED;
				}
		if(( rDataPack.bpayType.switchButtonLeftF2 == PRESSED) &&
				userButtonLeftF2Callback ) {
				userButtonLeftF2Callback();	
				rDataPack.bpayType.switchButtonLeftF2 = RELEASED;
				}
		if ((rDataPack.bpayType.switchButtonLeftStick == PRESSED) &&
			userButtonLeftStickCallback) {
			userButtonLeftStickCallback();
			rDataPack.bpayType.switchButtonLeftStick = RELEASED;
		}

		// if press the button, trigger the user button event functions
		if ( (rDataPack.bpayType.switchButton4 == PRESSED) &&
			userButton4Callback) {
			userButton4Callback();
			rDataPack.bpayType.switchButton4 = RELEASED;
		}
		if ( (rDataPack.bpayType.switchButton2 == PRESSED) &&
			userButton2Callback) {
			userButton2Callback();
			rDataPack.bpayType.switchButton2 = RELEASED;
		}
		if ( (rDataPack.bpayType.switchButton1 == PRESSED) &&
			userButton1Callback) {
			userButton1Callback();
			rDataPack.bpayType.switchButton1 = RELEASED;
		}
		if ( (rDataPack.bpayType.switchButton3 == PRESSED) &&
			userButton3Callback) {
			userButton3Callback();
			rDataPack.bpayType.switchButton3 = RELEASED;
		}
		if ( (rDataPack.bpayType.switchButtonRightF1 == PRESSED) &&
			userButtonRightF1Callback) {
			userButtonRightF1Callback();
			rDataPack.bpayType.switchButtonRightF1 = RELEASED;
		}
		if ((rDataPack.bpayType.switchButtonRightF2 == PRESSED) &&
			userButtonRightF2Callback) {
			userButtonRightF2Callback();
			rDataPack.bpayType.switchButtonRightF2 = RELEASED;
		}  
		if ((rDataPack.bpayType.switchButtonRightStick == PRESSED) &&
			userButtonRightStickCallback) {
			userButtonRightStickCallback();
			rDataPack.bpayType.switchButtonRightStick = RELEASED;
		}
		return true;
    }
	else {
		//debugSerial.println(F("parse error"));
	}
  }
  return false;
}

int DFRobotBLEGamepad::readJoystickRightX() {
  return  _joystickRightX;
}
int DFRobotBLEGamepad::readJoystickRightY() {
  return  _joystickRightY;
}
int DFRobotBLEGamepad::readJoystickLeftX() {
	return  _joystickLeftX;
}
int DFRobotBLEGamepad::readJoystickLeftY() {
	return  _joystickLeftY;
}

boolean DFRobotBLEGamepad::readSwitchUp() {
  return this->rDataPack.bpayType.switchButtonUp;
}

boolean DFRobotBLEGamepad::readSwitchDown() {
  return this->rDataPack.bpayType.switchButtonDown;
}

boolean DFRobotBLEGamepad::readSwitchLeft() {
  return this->rDataPack.bpayType.switchButtonLeft;
}

boolean DFRobotBLEGamepad::readSwitchRight() {
  return this->rDataPack.bpayType.switchButtonRight;
}

boolean DFRobotBLEGamepad::readSwitchLeftF1() {
  return this->rDataPack.bpayType.switchButtonLeftF1;
}

boolean DFRobotBLEGamepad::readSwitchLeftF2() {
  return this->rDataPack.bpayType.switchButtonLeftF2;
}

boolean DFRobotBLEGamepad::readSwitchLeftStick() {
	return this->rDataPack.bpayType.switchButtonLeftStick;
}


boolean DFRobotBLEGamepad::readSwitch4() {
	return this->rDataPack.bpayType.switchButton4;
}

boolean DFRobotBLEGamepad::readSwitch2() {
	return this->rDataPack.bpayType.switchButton2;
}

boolean DFRobotBLEGamepad::readSwitch1() {
	return this->rDataPack.bpayType.switchButton1;
}

boolean DFRobotBLEGamepad::readSwitch3() {
	return this->rDataPack.bpayType.switchButton3;
}

boolean DFRobotBLEGamepad::readSwitchRightF1() {
	return this->rDataPack.bpayType.switchButtonRightF1;
}

boolean DFRobotBLEGamepad::readSwitchRightF2() {
	return this->rDataPack.bpayType.switchButtonRightF2;
} 

boolean DFRobotBLEGamepad::readSwitchRightStick() {
	return this->rDataPack.bpayType.switchButtonRightStick;
}


/* 
 * sets function called on Switch UP pressed
 */
void DFRobotBLEGamepad::ButtonUpIsPressed(void (*function)(void)){
  userButtonUpCallback = function;  
}
void DFRobotBLEGamepad::ButtonDownIsPressed(void (*function)(void)){
  userButtonDownCallback = function;  
}
void DFRobotBLEGamepad::ButtonLeftIsPressed(void (*function)(void)){
  userButtonLeftCallback = function;  
}
void DFRobotBLEGamepad::ButtonRightIsPressed(void (*function)(void)){
  userButtonRightCallback = function;  
}
void DFRobotBLEGamepad::ButtonLeftF1IsPressed(void (*function)(void)){
  userButtonLeftF1Callback = function;  
}
void DFRobotBLEGamepad::ButtonLeftF2IsPressed(void (*function)(void)){
  userButtonLeftF2Callback = function;  
}
void DFRobotBLEGamepad::ButtonLeftStickIsPressed(void(*function)(void)) {
	userButtonLeftStickCallback = function;
}


void DFRobotBLEGamepad::Button4IsPressed(void(*function)(void)) {
	userButton4Callback = function;
}
void DFRobotBLEGamepad::Button2IsPressed(void(*function)(void)) {
	userButton2Callback = function;
}
void DFRobotBLEGamepad::Button1IsPressed(void(*function)(void)) {
	userButton1Callback = function;
}
void DFRobotBLEGamepad::Button3IsPressed(void(*function)(void)) {
	userButton3Callback = function;
}
void DFRobotBLEGamepad::ButtonRightF1IsPressed(void(*function)(void)) {
	userButtonRightF1Callback = function;
}
void DFRobotBLEGamepad::ButtonRightF2IsPressed(void(*function)(void)) {
	userButtonRightF2Callback = function;
}   
void DFRobotBLEGamepad::ButtonRightStickIsPressed(void(*function)(void)) {
	userButtonRightStickCallback = function;
}


byte DFRobotBLEGamepad::digitalButtonParser( void ){
  byte length = 0x00;
 
	this->rDataPack.prbpayType.switchButtonUp = this->rDataPack.bpayType.switchButtonUp;
	this->rDataPack.prbpayType.switchButtonDown = this->rDataPack.bpayType.switchButtonDown;
	this->rDataPack.prbpayType.switchButtonLeft = this->rDataPack.bpayType.switchButtonLeft;
	this->rDataPack.prbpayType.switchButtonRight = this->rDataPack.bpayType.switchButtonRight;
	this->rDataPack.prbpayType.switchButtonLeftF1 = this->rDataPack.bpayType.switchButtonLeftF1;
	this->rDataPack.prbpayType.switchButtonLeftF2 = this->rDataPack.bpayType.switchButtonLeftF2;
	this->rDataPack.prbpayType.switchButtonLeftStick = this->rDataPack.bpayType.switchButtonLeftStick;

	this->rDataPack.prbpayType.switchButton4 = this->rDataPack.bpayType.switchButton4;
	this->rDataPack.prbpayType.switchButton2 = this->rDataPack.bpayType.switchButton2;
	this->rDataPack.prbpayType.switchButton1 = this->rDataPack.bpayType.switchButton1;
	this->rDataPack.prbpayType.switchButton3 = this->rDataPack.bpayType.switchButton3;
	this->rDataPack.prbpayType.switchButtonRightF1 = this->rDataPack.bpayType.switchButtonRightF1;
	this->rDataPack.prbpayType.switchButtonRightF2 = this->rDataPack.bpayType.switchButtonRightF2;  
	this->rDataPack.prbpayType.switchButtonRightStick = this->rDataPack.bpayType.switchButtonRightStick;

	
  if (rDataPack.digitalButton[0] & DIGITALBUTTONLeftStick) {
		rDataPack.bpayType.switchButtonLeftStick = PRESSED;
		length++;
	}
   else {
		rDataPack.bpayType.switchButtonLeftStick = RELEASED;
	}
  if( rDataPack.digitalButton[0] & DIGITALBUTTONLeftF1 ){
	  rDataPack.bpayType.switchButtonLeftF1 = PRESSED;
	  length++;
  }else{
	  rDataPack.bpayType.switchButtonLeftF1 = RELEASED;
  }
  
  if( rDataPack.digitalButton[0] & DIGITALBUTTONLeftF2 ){
	  rDataPack.bpayType.switchButtonLeftF2 = PRESSED;
	  length++;
  }else{
	  rDataPack.bpayType.switchButtonLeftF2 = RELEASED;
  }
  if( rDataPack.digitalButton[1] & DIGITALBUTTONUp ){
	  rDataPack.bpayType.switchButtonUp = PRESSED;
	  length++;
  }else{
	  rDataPack.bpayType.switchButtonUp = RELEASED;
  }
  
  if( rDataPack.digitalButton[1] & DIGITALBUTTONDown ){
	  rDataPack.bpayType.switchButtonDown = PRESSED;
	  length++;
  }else{
	  rDataPack.bpayType.switchButtonDown = RELEASED;
  }
  
  if( rDataPack.digitalButton[1] & DIGITALBUTTONLeft ){
	  rDataPack.bpayType.switchButtonLeft = PRESSED;
	  length++;
  }else{
	  rDataPack.bpayType.switchButtonLeft = RELEASED;
  }
  
  if( rDataPack.digitalButton[1] & DIGITALBUTTONRight ){
	  rDataPack.bpayType.switchButtonRight = PRESSED;
	  length++;
  }else{
	  rDataPack.bpayType.switchButtonRight = RELEASED;
  }                                                                          


  if (rDataPack.digitalButton[0] & DIGITALBUTTONRightStick) {
	  rDataPack.bpayType.switchButtonRightStick = PRESSED;
	  length++;
  }
  else {
	  rDataPack.bpayType.switchButtonRightStick = RELEASED;
  }
  if (rDataPack.digitalButton[0] & DIGITALBUTTONRightF1) {
	  rDataPack.bpayType.switchButtonRightF1 = PRESSED;
	  length++;
  }
  else {
	  rDataPack.bpayType.switchButtonRightF1 = RELEASED;
  }

  if (rDataPack.digitalButton[0] & DIGITALBUTTONRightF2) {
	  rDataPack.bpayType.switchButtonRightF2 = PRESSED;
	  length++;
  }
  else {
	  rDataPack.bpayType.switchButtonRightF2 = RELEASED;
  }

  if (rDataPack.digitalButton[1] & DIGITALBUTTON4) {
	  rDataPack.bpayType.switchButton4 = PRESSED;
	  length++;
  }
  else {
	  rDataPack.bpayType.switchButton4 = RELEASED;
  }

  if (rDataPack.digitalButton[1] & DIGITALBUTTON2) {
	  rDataPack.bpayType.switchButton2 = PRESSED;
	  length++;
  }
  else {
	  rDataPack.bpayType.switchButton2 = RELEASED;
  }

  if (rDataPack.digitalButton[1] & DIGITALBUTTON1) {
	  rDataPack.bpayType.switchButton1 = PRESSED;
	  length++;
  }
  else {
	  rDataPack.bpayType.switchButton1 = RELEASED;
  }

  if (rDataPack.digitalButton[1] & DIGITALBUTTON3) {
	  rDataPack.bpayType.switchButton3 = PRESSED;
	  length++;
  }
  else {
	  rDataPack.bpayType.switchButton3 = RELEASED;
  }                                                                                
  return length;
}

/* 
 *Private functions
 */
int DFRobotBLEGamepad::bleDataPackageParser() {
 /*
  * 0x10  - Parse success
  * 0x11  - Wrong header charactors
  * 0x12  - Wrong button number
  * 0x13  - Check Sum Error
  */
  byte calculateSum = 0;

  rDataPack.header1 = bleDataArray[0], calculateSum ^=  rDataPack.header1;                             //get the data from queue
  
  rDataPack.header2 = bleDataArray[1], calculateSum ^=  rDataPack.header2;
  

  if( rDataPack.header1 != DEFAULTHEADER1 )     return 0x11;																//check 1st and 2nd header byte
  if( rDataPack.header2 != DEFAULTHEADER2 )     return 0x11;
  
  rDataPack.address = bleDataArray[2], calculateSum ^=  rDataPack.address;										//get address byte
  
  rDataPack.dataLength = bleDataArray[3],calculateSum ^= rDataPack.dataLength;								//get data length
 
  rDataPack.buttonCommond = bleDataArray[4],calculateSum ^= rDataPack.buttonCommond;					//get commond byte
  
  
  for (int i = 0; i < 4; i++)                                                                        //get digitalButton-bit
  {
	  rDataPack.digitalButton[i] = bleDataArray[i + 5], calculateSum ^= rDataPack.digitalButton[i];
  }
  
  int digitalButtonLength = this->digitalButtonParser();
#if	DEBUGCHECKSUM																																												
  if ( DEBUGCHECKSUM ) {																																		//check if the length is valid
    debugSerial.print(F( "Parser -> digitalButtonLength: ") );
    debugSerial.println( digitalButtonLength );
  }
#endif
	
  if( digitalButtonLength > MAXBUTTONNUMBER )
		return 0x12;
	
  
  //read 4 byte joystick data package
  for (int i = 0; i < 4; i++)
  {
	  rDataPack.joystickPayload[i] = bleDataArray[i+9], calculateSum ^= rDataPack.joystickPayload[i];
  }
  rDataPack.checkSum = bleDataArray[13];	              //get the check sum byte

#if	DEBUGCHECKSUM	
  if ( DEBUGCHECKSUM ) {
    debugSerial.print( F("Parser -> sum calculation: " ));
    debugSerial.println( calculateSum );
    debugSerial.print( F("Parser -> checkSum byte value: ") );
    debugSerial.println( rDataPack.checkSum );
  }
#endif
 /* 
	* check sum and update the parse state value
  * if the checksum byte is not correct, return 0x12
  */
  rDataPack.commandFlag = false;				//clear the command flag to finish the parse step and wait for the next data package
  if ( rDataPack.checkSum == calculateSum ) 
	  return PARSESUCCESS;
  else  
	  return 0x13;
}

/* 
 *init the variables
 */
void  DFRobotBLEGamepad::initRecvDataPack() {
  rDataPack.commandFlag = false;
  rDataPack.commandLength = DEFAULTPACKLENGTH;
  rDataPack.parseState = PARSESUCCESS;
}

void DFRobotBLEGamepad::updateJoystickVal(){
  _joystickRightX = rDataPack.joystickPayload[0];
  _joystickRightY = rDataPack.joystickPayload[1];
  _joystickLeftX = rDataPack.joystickPayload[2];
  _joystickLeftY = rDataPack.joystickPayload[3];
}


