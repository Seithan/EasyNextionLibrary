/*!
 * EasyNextionLibrary.cpp - Easy library for Nextion Displays
 * Copyright (c) 2020 Athanasios Seitanis < seithagta@gmail.com >. 
 * All rights reserved under the library's licence
 */

// include this library's description file
#ifndef EasyNextionLibrary_h
#include "EasyNextionLibrary.h"
#endif

#ifndef trigger_cpp
#include "trigger.cpp"             // This file contains the predefined void for function trigger
#endif

#ifndef callTriggers_h     // This file contains a code with a switch(){case} command. 
#include "callTriggers.h"  // It runs when we have the command group "Trigger" at the readCommand() function
#endif                     // Reads the Serial and calls the desired trigger()

  //-------------------------------------------------------------------------
 // Constructor : Function that handles the creation and setup of instances
//---------------------------------------------------------------------------

EasyNex::EasyNex(HardwareSerial& serial){  // Constructor's parameter is the Serial we want to use
  _serial = &serial;
}

void EasyNex::begin(unsigned long baud){
  _serial->begin(baud);  // We pass the initialization data to the objects (baud rate) default: 9600
  
  delay(100);            // Wait for the Serial to initialize
  
  _tmr1 = millis();
  while(_serial->available() > 0){     // Read the Serial until it is empty. This is used to clear Serial buffer
    if((millis() - _tmr1) > 400){    // Reading... Waiting... But not forever...... 
      break;                            
    }   
  }
}


/*
 * -- writeNum(String, uint32_t): for writing in components' numeric attribute
 * String = objectname.numericAttribute (example: "n0.val"  or "n0.bco".....etc)
 * uint32_t = value (example: 84)
 * Syntax: | myObject.writeNum("n0.val", 765);  |  or  | myObject.writeNum("n0.bco", 17531);       |
 *         | set the value of numeric n0 to 765 |      | set background color of n0 to 17531 (blue)|
 */
void EasyNex::writeNum(String command, uint32_t val){
	_component = command;
	_numVal = val;
  
	_serial->print(_component);
  _serial->print("=");
  _serial->print(_numVal);
	_serial->print("\xFF\xFF\xFF");
}


/*
 * -- writeStr(String, String): for writing in components' text attributes
 * String No1 = objectname.textAttribute (example: "t0.txt"  or "b0.txt")
 * String No2 = value (example: "Hello World")
 * Syntax: | myObject.writeStr("t0.txt", "Hello World");  |  or  | myObject.writeNum("b0.txt", "Button0"); |
 *         | set the value of textbox t0 to "Hello World" |      | set the text of button b0 to "Button0"  |
 */
void EasyNex::writeStr(String command, String txt){ 
	_component = command;
	_strVal = txt;
  
  if(_strVal == "cmd"){
    _serial->print(_component);
    _serial->print("\xFF\xFF\xFF");
    
  }else if(_strVal != "cmd"){
    _serial->print(_component);
    _serial->print("=\"");
    _serial->print(_strVal);
    _serial->print("\"");
    _serial->print("\xFF\xFF\xFF");
  }
}


/*
 * -- readNumber(String): We use it to read the value of a components' numeric attribute on Nextion
 * In every component's numeric attribute (value, bco color, pco color...etc)
 * String = objectname.numericAttribute (example: "n0.val", "n0.pco", "n0.bco"...etc)
 * Syntax: | myObject.readNumber("n0.val"); |  or  | myObject.readNumber("b0.bco");                       |
 *         | read the value of numeric n0   |      | read the color number of the background of butoon b0 |
 */
uint32_t EasyNex::readNumber(String component){
	
	_comp = component;
	
	_serial->print("printh 23 05 52"); // With this command, Nextion is going to send the HEX values of 23 05 52 
	_serial->print("\xFF\xFF\xFF");    // We use them to recognise that a command is existant 
                                     // and that the command is for the readNumber() function  
	_serial->print("print ");          // With this command, from Nextion we send four bites in little endian order,
                                     // which represent the numeric value of the component
	_serial->print(_comp);             // Here you put the variable you want to read 
	_serial->print("\xFF\xFF\xFF");
  
  _waitingForNumber = false;  // set it to true false while debug
  _numberValue = readNumberFromSerial();
  
  if(_waitingForNumber == true){ // If 
    _waitingForNumber = false;
    _countRetry = 0;
     
	}else if(_waitingForNumber == false && _countRetry == 0){ // In case of failing to find a command of group 'R', 
                                                            // check again, by calling readNumberFromSerial(). 
                                                            // We do this only once, and that is why we add 1 to _countRetry 
    _countRetry++;
    _numberValue = readNumberFromSerial();
  }
  if(_waitingForNumber == false && _countRetry == 1){       // If we still have not read the value after the second try,
                                                            // _countRetry = 0 and return _numberValue, which will be 7777
    _countRetry = 0;
  }
  return _numberValue;
}


uint32_t EasyNex::readNumberFromSerial(){   // With this functions, we check if the command group is 'R'
                                            // That means that a number follows, and after we read and return the value
                                            
  _numberValue = 7777;                      // The function will return this number in case it fails to read the new number
                                            // You can use it in your code, to confirm if you have the right number
                                            // You can check it with an if() , in which you will ignore the value of 7777 and
                                            // run the readNumber() again. 
                                            //
                                            // The example:
                                            /* int x = myObject.readNumber();
                                             * if(x == 7777){
                                             *  x = myObject.readNumber();
                                             * }
                                             */
                                            // Instead of the if() , you can use while() , but have in mind that when using a while()
                                            // You must have a timer to break the while, as the code could get stuck in the while()
                                             
  _tmr1 = millis();
  while(_serial->available() < 2){   // Waiting for bytes to come to Serial
    if((millis() - _tmr1) > 500){    // Waiting... But not forever... 
      break;                            
    }
  }
  
	if(_serial->available() > 1){         // Read if more than 2 bytes come (we always send more than 2 <#> <len> <cmd> <id>
    _start_char = _serial->read();      // Create a local variable (start_char) read and store the first byte on it  
    _tmr1 = millis();
    while(_start_char != '#'){         // Read the Serial until start_char is found
      _start_char = _serial->read();   // Just in case that "garbage" data lying on Serial
      if((millis() - _tmr1) > 400){    // Reading... Waiting... But not forever...... 
        break;                            
      }   
    }
    if(_start_char == '#'){        // And when we find the character #
      _len = _serial->read();      // Create local variable (len) / read and store the value of the second byte
                                   // <len> is the lenght (number of bytes following) 
      _tmr1 = millis();
      _cmdFound = true;
          
      while(_serial->available() < _len){ // Waiting for all the bytes that we declare with <len> to arrive              
        if((millis() - _tmr1) > 400){     // Waiting... But not forever...... 
          _cmdFound = false;              // tmr_1 a timer to avoid the stack in the while loop if there is not any bytes on _serial
          break;                            
        }                                     
      }                                   
      
      if(_cmdFound == true){          // So..., A command is found (bytes in _serial buffer egual more than len)
        _cmd1 = _serial->read();      // Create local variable (cmd). Read and store the next byte. This is the command group
				if(_cmd1 == 'R'){
					for(int i = 0; i < 4; i++){
		        _numericBuffer[i] = _serial->read();
	        }
          
          // Little endian conversion
          _numberValue = _numericBuffer[3];
          _numberValue <<= 8;
          _numberValue |= _numericBuffer[2];
          _numberValue <<= 8;
          _numberValue |= _numericBuffer[1];
          _numberValue <<= 8;
          _numberValue |= _numericBuffer[0]; 
          
          _waitingForNumber = true;   // means that a readNumber command found and we have the number stored
          
          
				}else if(_cmd1 != 'R'){
          if(_waitingForNumber == false && _countRetry == 1){
            _waitingForNumber = true;    // If _waitingForNumber == false and _countRetry == 1, means it is the second time we search
                                         // for a group 'R' command. In this way, we set the _waitingForNumber == true, 
                                         // to break the readNumber() function and run the rest of the code.
          }else{
            _waitingForNumber = false;  // The command that was found is not a command of readNumber group 'R'
            readCommand();              // We call the readCommand function and send the command where it belongs
                                        // by checking the other possible cases for command groups at the readCommand() function.
                                        // After that the readNumber() function as the _waitingForNumber == false and _countRetry == 0
                                        // will check once more time for a group 'R' command
                                        
          }       
        }
			}
		}
	}
  
  
  return _numberValue;
} 


/*
 * -- NextionListen(): It uses a custom protocol to identify commands from Nextion Touch Events
 * For advanced users: You can modify the custom protocol to add new group commands.
 * More info on custom protocol: https://www.seithan.com/  and on the documentation of the library
 */
/*! WARNING: This function must be called repeatedly to response touch events
 * from Nextion touch panel. 
 * Actually, you should place it in your loop function.
 */
void EasyNex::NextionListen(){
	if(_serial->available() > 2){         // Read if more then 2 bytes come (we always send more than 2 <#> <len> <cmd> <id>
    _start_char = _serial->read();      // Create a local variable (start_char) read and store the first byte on it  
    _tmr1 = millis();
    
    while(_start_char != '#'){
      _start_char = _serial->read();
      if((millis() - _tmr1) > 100){     // Waiting... But not forever...... 
        break;                            
      }   
    }
    if(_start_char == '#'){            // And when we find the character #
      _len = _serial->read();          // Create local variable (len) / read and store the value of the second byte
                                       // <len> is the lenght (number of bytes following) 
      _tmr1 = millis();
      _cmdFound = true;
      
      while(_serial->available() < _len){     // Waiting for all the bytes that we declare with <len> to arrive              
        if((millis() - _tmr1) > 100){         // Waiting... But not forever...... 
          _cmdFound = false;                  // tmr_1 a timer to avoid the stack in the while loop if there is not any bytes on _serial
          break;                            
        }                                     
      }                                   
  
      if(_cmdFound == true){                  // So..., A command is found (bytes in _serial buffer egual more than len)
        _cmd1 = _serial->read();              // Read and store the next byte. This is the command group
        readCommand();                        // We call the readCommand(), 
                                              // in which we read, seperate and execute the commands 
			}
		}
	}
}


// This was a part of the NextionListen(), but we seperated it, 
// in order to make easier the modifications for it, especially in the case of a custom protocol
// Now, we only have two command groups: the "Trigger" and "Page"
// And two extra, as an example, can be defined by the user, to write their code there
// You can add as many cases as you want :)

void EasyNex::readCommand(){

				
  switch(_cmd1){
    case 'P': /*or <case 0x50:>  If 'P' matches, we have the command group "Page". 
               *The next byte is the page <Id> according to our protocol.
               *
               * We have write in every page's "preinitialize page" the command
               *  printh 23 02 50 xx (where xx is the page id in HEX, 00 for 0, 01 for 1, etc).
               * <<<<Every event written on Nextion's pages preinitialize page event will run every time the page is Loaded>>>>
               *  it is importand to let the Arduino "Know" when and which Page change.
               */
      lastCurrentPageId = currentPageId;
      currentPageId = _serial->read();                   
      break;
      
      
    case 'T':   /* Or <case 0x54:>  If 'T' matches, we have the command group "Trigger". 
                 * The next byte is the trigger <Id> according to our protocol.
                 *
                 * We have to write in a Touch Event on Nextion the following: < printh 23 02 54 xx >
                 * (where xx is the trigger id in HEX, 01 for 1, 02 for 2, ... 0A for 10 etc).
                 * With the switch(){case}, we call a predefined void with prefixed names
                 * as we have declare them on trigger.cpp file. Starting from trigger1()......up to trigger50()
                 * The maximum number of predefined void is 255
                 * We declare the trigger() at this file: trigger.cpp
                 * 
                 * You can use the trigger() in your code as a function, 
                 * in order to write any kind of code and run it, by sending the < printh > command needed,
                 * from a Touch event on Nextion, such as pressing a button. Example:
                 */
                /**                   void trigger1(){
                                        digitalWrite(13, HIGH); // sets the digital pin 13 on
                                        delay(1000);            // waits for a second
                                        digitalWrite(13, LOW);  // sets the digital pin 13 off
                                      }
                 */
                /* In Touch Press Event of a button on Nextion write <printh 23 02 54 01>
                 * Every time we press the Button the command <printh 23 02 54 01> will be sent over Serial
                 * Then, the library will call the void trigger1()
                 * the code inside the trigger1() will run once ...
                 */
      callTriggerFunction(); // We made a separate file callTriggers.h for this function, 
                             // as it would be too long to fit in here, because there are
                             // 50 predefined cases for the triggers. 
      break;
    

    case 'X': // or < case 0x58 > If 'X' matches, we have the command group "X"
              
              // for future use by the user
      
      break;
      
    case 'Y': // or < case 0x59 > If 'Y' matches, we have the command group 'Y'
      
              // for future use by the user
      
      break;
  }
}