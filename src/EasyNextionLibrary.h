/*!
 * EasyNextionLibrary.h - Easy library for Nextion Displays
 * Copyright (c) 2020 Athanasios Seitanis < seithagta@gmail.com >. 
 * All rights reserved under the library's licence
 */

#if ARDUINO >= 100    
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

  //------------------------------------------------------
 // ensure this library description is only included once
//--------------------------------------------------------
#ifndef EasyNextionLibrary_h
#define EasyNextionLibrary_h



/**************************************************************************/
/** 
 *  @brief Class for functions that can easily contol Nextion Displays
 */
/**************************************************************************/
  //---------------------------------------
 // library interface description
//-----------------------------------------
class EasyNex {
  
    //--------------------------------------- 
	 // user-accessible "public" interface
  //-----------------------------------------
  
  /* five main functions of the library:
   *
   * -- begin(): the begin() method of the class in which we pass the initialization data to the objects. 
   * initialization data: unsigned long baud = 9600 (default) if nothing written in the begin()
   * myObject.begin(115200); for baud rate 115200
   * 
   * -- EasyNex(HardwareSerial& serial): The constructor of the class that has the parameter of the Serial we use
   * EasyNex.myObject(Serial);  or Serial1, Serial2....
   *
   * -- writeNum(String, unsigned int): for writing in components' numeric attribute
   * String = objectname.numericAttribute (example: "n0.val"  or "n0.bco".....etc)
   * unsigned int = value (example: 84)
   * Syntax: | myObject.writeNum("n0.val", 765);  |  or  | myObject.writeNum("n0.bco", 17531);       |
   *         | set the value of numeric n0 to 765 |      | set background color of n0 to 17531 (blue)|
   * 
   * 
   * -- writeStr(String, String): for writing in components' text attributes
   * String No1 = objectname.textAttribute (example: "t0.txt"  or "b0.txt")
   * String No2 = value (example: "Hello World")
   * Syntax: | myObject.writeStr("t0.txt", "Hello World");  |  or  | myObject.writeNum("b0.txt", "Button0"); |
   *         | set the value of textbox t0 to "Hello World" |      | set the text of button b0 to "Button0"  |
   * 
   * -- NextionListen(): It uses a custom protocol to identify commands from Nextion Touch Events
   * For advanced users: You can modify the custom protocol to add new group commands.
   * More info on custom protocol: https://www.seithan.com/  and on the documentation of the library
   * WARNING: This function must be called repeatedly to response touch events
   * from Nextion touch panel. Actually, you should place it in your loop function.
   * 
   * -- readNumber(String): We use it to read the value of a components' numeric attribute
   * In every component's numeric attribute (value, bco color, pco color...etc)
   * String = objectname.numericAttribute (example: "n0.val", "n0.pco", "n0.bco"...etc)
   * Syntax: | myObject.readNumber("n0.val"); |  or  | myObject.readNumber("b0.bco");                       |
   *         | read the value of numeric n0   |      | read the color number of the background of button b0 |
   */
   

	public:
    EasyNex(HardwareSerial& serial);
		void begin(unsigned long baud = 9600);
    void writeNum(String, uint32_t);
    void writeStr(String, String txt = "cmd");
		void NextionListen(void);
    uint32_t readNumber(String);
    
      //--------------------------------------- 
     // public variables
    //-----------------------------------------
    
    /* currentPageId: shows the id of the current page shown on Nextion
     * WARNING: At the Preinitialize Event of every page, we must write: 
     * printh 23 02 50 xx , where xx the id of the page in hex 
     * (example: for page0, we write: printh 23 02 50 00 , for page9: printh23 02 50 09, for page10: printh 23 02 50 0A) 
     * Use can call it by writing in the .ino file code:  variable = myObject.currentPageId;
     * 
     * lastCurrentPageId: stores the value of the previous page shown on Nextion
     * No need to write anything in Preinitialize Event on Nextion
     * You can call it by writing in the .ino file code:  variable = myObject.lastCurrentPageId;
     */ 
    int currentPageId;  
    int lastCurrentPageId;
    
    
    //--------------------------------------- 
	 // library-accessible "private" interface
  //-----------------------------------------
	private:
    HardwareSerial* _serial;
    uint32_t readNumberFromSerial(void);
		void readCommand(void);
    void callTriggerFunction(void);
    
      //----------------------------------------------
     // for void writeNum (write to numeric attribute)
    //------------------------------------------------
		String _component;
    uint32_t _numVal;
    
      //--------------------------------------------
     // for void writeStr (write to text attribute)
    //----------------------------------------------
    String _strVal;
    
		  //---------------------------------------
		 // for void readNumber
    //-----------------------------------------
		String _comp;
		uint32_t _readValue;
		bool _waitingForNumber;
    uint8_t _countRetry;
    
      //---------------------------------------
		 // for void readNumberFromSerial
    //-----------------------------------------    
		uint8_t _numericBuffer[4];
		uint32_t _numberValue;
    char _start_char;
    uint8_t _len;
    unsigned long _tmr1;
    boolean _cmdFound;
    uint8_t _cmd1;
    uint8_t _tempRead;
};

#endif

