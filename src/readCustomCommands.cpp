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
      callTriggerFunction(); // We made a separate file callTriggers.cpp for this function, 
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