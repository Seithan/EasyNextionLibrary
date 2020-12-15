/*
 * FourStepExample.ino - Simple example code
 * Copyright (c) 2020 Athanasios Seitanis < seithagta@gmail.com >.
 * https://www.seithan.com 
 * All rights reserved. EasyNextionLibrary is licensed under the MIT License
 * https://opensource.org/licenses/MIT
 */
 
/* I have invested time and resources providing open source codes, like this one. 
 * Please do not hesitate to support my work!
 * If you found  this work useful and has saved you time and effort,
 * Just simply paypal me at: seithagta@gmail.com
 */

 //********************************************************************************
 //  You can find more examples, tutorials and projects with Nextion on my website
 //  https://www.seithan.com 
 //********************************************************************************
 
// Compatible for WeMos D1 mini
 
/* This is the most important method of the library. 
 * And this is because, it gives you the ability to use the predefined functions and run your code from there. 
 * These predefined functions are named trigger0(), trigger1(), trigger2()... up to trigger50(). 
 * You can use them as a simple void out of the loop, in which you will have written a block of code to run every time it is called.
 * You can call those trigger() functions and run the code they contain anytime by simply writing in a Nextion Event the command:
 * `printh 23 02 54 XX` , where `XX` the id for the triggerXX() in HEX.
 * Example: printh 23 02 54 00 to call trigger0() ... printh 23 02 54 0A to call trigger10() and so on...
 */

/*
  Declare the void by simply writing:
  void trigger0(){
  [ put your code here !!!!]
  }
*/
 
#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary

EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex > 
                       // Set as parameter the Hardware Serial you are going to use

void setup(){
  myNex.begin(9600); // Begin the object with a baud rate of 9600
                     // If no parameter was given in the begin(), the default baud rate of 9600 will be used 
  pinMode(LED_BUILTIN, OUTPUT);    // The built-in LED is initialized as an output            
  digitalWrite(LED_BUILTIN, HIGH); // Setting the built-in LED to HIGH = off
}

void loop(){
  myNex.NextionListen(); // This function must be called repeatedly to response touch events
                         // from Nextion touch panel. Actually, you should place it in your loop function.
}

void trigger0(){
  /* Create a button on Nextion
   * Write in the Touch Release Event of the button
   * this command:    printh 23 02 54 00
   * Every time the button is pressed, the trigger0() function will run
   * and the code inside will be executed once
   */
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // If LED_BUILTIN is ON, turn it OFF, or the opposite
  if(digitalRead(LED_BUILTIN) == LOW){
    myNex.writeNum("b0.bco", 2016); // Set button b0 background color to GREEN (color code: 2016)
    myNex.writeStr("b0.txt", "ON"); // Set button b0 text to "ON"
    
  }else if(digitalRead(LED_BUILTIN) == HIGH){
    myNex.writeNum("b0.bco", 63488); // Set button b0 background color to RED (color code: 63488)
    myNex.writeStr("b0.txt", "OFF"); // Set button b0 text to "ON"
  }
}
