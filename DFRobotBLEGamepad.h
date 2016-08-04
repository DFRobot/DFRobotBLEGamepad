/*!!
* @file DFRobotBLEGamepad.h
* @brief DFRobot's Wireless Gamepad - Arduino with Bluetooth 4.0 (SKU:DFR0304)
* @n This is the library for DFRobot's Wireless Gamepad
* @n [Schematics]([http://www.dfrobot.com/wiki/index.php?title=BLE_Wireless_Gamepad_V2_SKU:_DFR0304])
*
* @copyright	[DFRobot](http://www.dfrobot.com), 2016
* @copyright	GNU Lesser General Public License
*
* @author [Jeffrey](Angelo.qiao@dfrobot.com)
* @version  V2.0
* @date  2016-07-18
*/
#ifndef DFROBOTBLEGAMEPAD_H_
#define DFROBOTBLEGAMEPAD_H_

#include "Arduino.h"

/*!*************** Debugger Configuration of the library decoding protocol *************************/

#define DEBUGDATARECEIVER	false
#define DEBUGDATARAW     	false
#define DEBUGPARSER      	false
#define DEBUGCHECKSUM       false
#define DEBUGUPDATEBUTTON	false

#define debugSerial			Serial

/*!
* These constants can be use for comparison with the value returned
* by the readButton() method.
*/
const boolean PRESSED = LOW;
const boolean RELEASED = HIGH;



/*!*************** Data structure for the command buffer ******************/

// Package protocol configuration
#define PACKHEADER					1
#define PACKHEADER2					2
#define PACKADDRESS					3
#define PACKBUTTONSIGN 			4
#define PACKJOYSTICKSIGN		5
#define PACKPAYLOAD					6


#define DEFAULTHEADER1      0x55
#define DEFAULTHEADER2      0xAA
#define DEFAULTADDRESS      0x11

#define DEFAULTPACKLENGTH		14

#define DIGITALBUTTONUp			0x01
#define DIGITALBUTTONDown			0x02
#define DIGITALBUTTONLeft			0x04
#define DIGITALBUTTONRight			0x08
#define DIGITALBUTTONLeftF1		0x01
#define DIGITALBUTTONLeftF2		0x02
#define DIGITALBUTTONLeftStick		0x08

#define DIGITALBUTTON4			0x10
#define DIGITALBUTTON2			0x20
#define DIGITALBUTTON1			0x40
#define DIGITALBUTTON3			0x80
#define DIGITALBUTTONRightF1		0x20
#define DIGITALBUTTONRightF2		0x40
#define DIGITALBUTTONRightStick		0x80


#define MAXBUTTONNUMBER     8													//Effect of the number buttons from operating mode
#define MAXBUTTONID         14

#define PARSESUCCESS        0x10


typedef struct buttonPayload {
	boolean switchButtonUp;
	boolean switchButtonDown;
	boolean switchButtonLeft;
	boolean switchButtonRight;
	boolean switchButtonLeftF1;
	boolean switchButtonLeftF2;
	boolean switchButtonLeftStick;

	boolean switchButton4;
	boolean switchButton2;
	boolean switchButton1;
	boolean switchButton3;
	boolean switchButtonRightF1;
	boolean switchButtonRightF2;
	boolean switchButtonRightStick;
}buttonPayloadType;

//Data Link package
//the package like this: 55 aa 0x11 0x0e 0x01/0x00 0x01-0x01 0x00 0x00 4*byte crc
#pragma pack(1)
typedef struct
{
	byte header1;          														//0x55
	byte header2;          														//0xAA
	byte address;          														//0x11

	byte buttonCommond;															//0x00-0x01

	byte digitalButton[4];

	buttonPayloadType	bpayType;
	buttonPayloadType	prbpayType;

	byte joystickPosition;
	byte joystickPayload[4];
	byte checkSum;

	byte dataLength;
	byte commandLength;
	byte parseState;
	boolean commandFlag;
} sDataLink;
/*!!
*  @brief Data Link package
*
*  @param header1           If header1 equals to 0x55, input the data.
*  @param header2           0xAA.
*  @param address           Address byte, 0x11
*  @param buttonCommond     if any button pressed output 0x01, otherwise 0x00
*  @param digitalButton     Pressing state of the digitalbuttons
*  @param bpayType            A struct involving bool type about all digitalbuttons' current state
*  @param prbpayType          A struct involving bool type about all digitalbuttons' previous state
*  @param joystickPayload   Pressing state of the joystick
*  @param checkSum          XOR sum of all received byte except the last one
*  @param dataLength        The length of received data bytes
*  @param commandLength     14
*  @param parseState        State of parsement,
*  @param commandFlag       Debug switch
*/
#pragma pack()


/*!*************** Main class ******************/

class DFRobotBLEGamepad {
public:
	// Constructor: the Serial port BLE connected
	// Default setting for Bluno using Serial
	
	DFRobotBLEGamepad();

	/*!
	* @brief 	init the baudrate of the bluetooth.
	*/
	void begin(Stream& theSerial);

	/*!
	* @brief check the valid command package.
	*/
	boolean available();
	/*!
	* @brief read the right joystick's x-axis value from package.
	*/
	int readJoystickRightX();
	/*!
	* @brief read the right joystick's y-axis value from package.
	*/
	int readJoystickRightY();
	/*!
	* @brief read the left joystick's x-axis value from package.
	*/
	int readJoystickLeftX();
	/*!
	* @brief read the left joystick's y-axis value from package.
	*/
	int readJoystickLeftY();

	/*!
	* @brief Reads the current state of LA button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitchUp();
	/*!
	* @brief Reads the current state of LB button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitchDown();
	/*!
	* @brief Reads the current state of LC button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitchLeft();
	/*!
	* @brief Reads the current state of LD button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitchRight();
	/*!
	* @brief Reads the current state of LF1 button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitchLeftF1();
	/*!
	* @brief Reads the current state of LF2 button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitchLeftF2();
	/*!
	* @brief Reads the current state of Left stick button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitchLeftStick();


	/*!
	* @brief Reads the current state of RA button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitch4();
	/*!
	* @brief Reads the current state of RB button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitch2();
	/*!
	* @brief Reads the current state of RC button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitch1();
	/*!
	* @brief Reads the current state of RD button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitch3();
	/*!
	* @brief Reads the current state of RF1 button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitchRightF1();
	/*!
	* @brief Reads the current state of RF2 button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitchRightF2();
	/*!
	* @brief Reads the current state of right stick button. It will return LOW if the button is pressed, and HIGH otherwise.
	*/
	boolean readSwitchRightStick();

	/*!
	* @brief Call back functions for switchLA pressed event
	*/
	void ButtonUpIsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchLB pressed event
	*/
	void ButtonDownIsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchLC pressed event
	*/
	void ButtonLeftIsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchLD pressed event
	*/
	void ButtonRightIsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchLF1 pressed event
	*/
	void ButtonLeftF1IsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchLF2 pressed event
	*/
	void ButtonLeftF2IsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchLeftstick pressed event
	*/
	void ButtonLeftStickIsPressed(void(*)(void));

	/*!
	* @brief Call back functions for switchRA pressed event
	*/
	void Button4IsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchRB pressed event
	*/
	void Button2IsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchRC pressed event
	*/
	void Button1IsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchRD pressed event
	*/
	void Button3IsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchRF1 pressed event
	*/
	void ButtonRightF1IsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchRF2 pressed event
	*/
	void ButtonRightF2IsPressed(void(*)(void));
	/*!
	* @brief Call back functions for switchRightstick pressed event
	*/
	void ButtonRightStickIsPressed(void(*)(void));

private:

	Stream* bleSerial;
	//HardwareSerial or SoftSerial;
	sDataLink rDataPack;
	// create a queue of characters for handling with the bluetooth Serial data
	byte bleDataArray[DEFAULTPACKLENGTH];
	// a array to save the bluetooth serial data

	int bleDataIndex;
	int _joystickRightX, _joystickRightY;
	int _joystickLeftX, _joystickLeftY;                                                 //joystick raw data
	int _button[MAXBUTTONID];														//button value from application
	int _prevButton[MAXBUTTONID];													//previous button value

   /*!
    * @brief update Joystick variables.
	*/
	void updateJoystickVal();
	/*!
	* @brief update ButtonState.
	*/
	void updateButtonState() {};
	/*!
	* @brief initiate the recvived data pack.
	*/
	void initRecvDataPack();
	/*!
	* @brief parser the data Package, return 0x10 if checkSum equals to the last byte of data.
	*/
	int bleDataPackageParser();
	/*!
	* @brief parser the data Package, return 0x10 if checkSum equals to the last byte of data.
	*/
	void bleDataReceiver();
	/*!
	* @brief read Serial data and push them to the queue.
	*/
	byte digitalButtonParser(void);

	static void(*userButtonUpCallback)(void);
	static void(*userButtonDownCallback)(void);
	static void(*userButtonLeftCallback)(void);
	static void(*userButtonRightCallback)(void);
	static void(*userButtonLeftF1Callback)(void);
	static void(*userButtonLeftF2Callback)(void);
	static void(*userButtonLeftStickCallback)(void);

	static void(*userButton4Callback)(void);
	static void(*userButton2Callback)(void);
	static void(*userButton1Callback)(void);
	static void(*userButton3Callback)(void);
	static void(*userButtonRightF1Callback)(void);
	static void(*userButtonRightF2Callback)(void);
	static void(*userButtonRightStickCallback)(void);
};

#endif 
