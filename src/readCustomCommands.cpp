/*!
 * readCustomCommands.cpp - Easy library for Nextion Displays
 * Copyright (c) 2020 Athanasios Seitanis < seithagta@gmail.com >. 
 * All rights reserved under the library's licence
 */

// include this library's description file
#ifndef EasyNextionLibrary_h
#include "EasyNextionLibrary.h"
#endif

#ifndef trigger_h
#include "trigger.h"
#endif
// This was a part of the NextionListen(), but we seperated it, 
// in order to make easier the modifications for it, especially in the case of a custom protocol
// Now, we only have two command groups: the "Trigger" and "Page"

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
                 * as we have declare them on trigger.cpp file. Starting from trigger0()......up to trigger50()
                 * The maximum number of predefined void is 255
                 * We declare the trigger() at this file: trigger.cpp
                 * 
                 * You can use the trigger() in your code as a function, 
                 * in order to write any kind of code and run it, by sending the < printh > command needed,
                 * from a Touch event on Nextion, such as pressing a button. Example:
                 */
                /**                   void trigger0(){
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
      callTriggerFunction(); // We made a separate file callTriggers.cpp for this function, 
                             // as it would be too long to fit in here, because there are
                             // 50 predefined cases for the triggers. 
      break;
    
    default:
    cmdGroup = _cmd1;
    cmdLength = _len;
    easyNexReadCustomCommand();
                    
     break;
              /* easyNexReadCustomCommand() has a weak attribute and will be created only when user
               * declare this function on the main code
               * More for custom protocol and commands https://seithan.com/Easy-Nextion-Library/Custom-Protocol/
               * The motivate to move this function out of the library's files comes from Ricardo Reis
               * thanks to his issue https://github.com/Seithan/EasyNextionLibrary/issues/15
               *
               * our commands will have this format: <#> <len> <cmd> <id> <id2>
               *  <#> start marker, declares that a command is followed
               *  <len> declares the number of bytes that will follow
               *  <cmd> declares the task of the command or command group
               *  <id> declares the properties of the command
               *  <id2> a second property of the command
               * To get here means that we have read a command successfully ( we have all the bytes declared with len in the Serial buffer)
               * with the above cases we check if we have a match with the predefined cmd of <P> and <T>
               * if there is no match we continue with the easyNexReadCustomCommand() and ONLY if we have declare the function in main code
               * from here we must handle the assign from the easyNexReadCustomCommand() in the user code where we can go on with a switch case
               * for the cmd that we have stored on the cmdGroup global variable, we can call it with myObject.cmdGroup 
               */
  }
}


/*
 abstract from main code

void easyNexReadCustomCommand(){

  int arrayPlace; // temp variable
  int value;      // temp variable
  
  switch(myNex.cmdGroup){
    
    case 'L': // Or <case 0x4C:>  If 'L' matches
    // we are going to write values in specific places in the dataL[] table
    // read the next byte that determines the position on the table
    arrayPlace = myNex.readByte();
    
    // read the next byte that keeps the value for the position
    value = myNex.readByte();
    
    // update the array with the new values
    dataL[arrayPlace] = value;  
    
    break; 

    case 'S': // Or <case 0x53:>  If 'S' matches 
    
    // we are going to write values in specific places in the dataS[] table
    // from Nextion printh 23 03 53 00 00
    // read the next byte that determines the position on the table
    arrayPlace = myNex.readByte();
    
    // read the next byte that keeps the value for the position
    value = myNex.readByte();
    
    // update the array with the new values
    dataS[arrayPlace] = value;  
    
    break;

*/
