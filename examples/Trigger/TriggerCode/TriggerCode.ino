/*
 * TriggerCode.ino - Simple example code for EasyNextionLibrary
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
 
// Compatible for Arduino

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
  
  pinMode(LED_BUILTIN, OUTPUT); // The built-in LED is initialized as an output
  digitalWrite(LED_BUILTIN, LOW);
}

void loop(){
  myNex.NextionListen(); // WARNING: This function must be called repeatedly to response touch events
                         // from Nextion touch panel. Actually, you should place it in your loop function.
}

void trigger0(){
  // To call this void send from Nextion's component's Event:  printh 23 02 54 00
  // In this exmaple, we send this command from the Release Event of b0 button (see the HMI of this example)
  // You can send  the same `printh` command, to call the same function, from more than one component, depending on your needs

  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // If LED_BUILTIN is ON, turn it OFF, or the opposite
  if(digitalRead(LED_BUILTIN) == HIGH){
    myNex.writeNum("b0.bco", 2016); // Set button b0 background color to GREEN (color code: 2016)
    myNex.writeStr("b0.txt", "ON"); // Set button b0 text to "ON"
    myNex.writeNum("p0.pic", 1);    // Set picture 1 as background picture for p0
    
  }else if(digitalRead(LED_BUILTIN) == LOW){
    myNex.writeNum("b0.bco", 63488); // Set button b0 background color to RED (color code: 63488)
    myNex.writeStr("b0.txt", "OFF"); // Set button b0 text to "OFF"
    myNex.writeNum("p0.pic", 0);     // Set picture 0 as background picture for p0 picture component
  }
}

void trigger1(){
  // To call this void send from Nextion's component's Event:  printh 23 02 54 01
  // In this exmaple, we send this command from the Release Event of b1 button (see the HMI of this example)
  // You can send  the same `printh` command, to call the same function, from more than one component, depending on your needs
  
  if(digitalRead(LED_BUILTIN) == HIGH){
    digitalWrite(LED_BUILTIN, LOW);  // Start the function with the LED off
    myNex.writeNum("b0.bco", 63488); // Set button b0 background color to RED (color code: 63488)
    myNex.writeStr("b0.txt", "OFF"); // Set button b0 text to "OFF"
    myNex.writeNum("p0.pic", 0);     // Set picture 0 as background picture for p0 picture component
  }
  
  myNex.writeStr("t0.txt", "LED STROBE\\rON");
  for(int i = 0; i < 10; i++){
    for(int x = 0; x < 10; x++){
      digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // If LED_BUILTIN is ON, turn it OFF, or the opposite
      delay(50);
    }
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   // If LED_BUILTIN is ON, turn it OFF, or the opposite
    delay(500);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));   // If LED_BUILTIN is ON, turn it OFF, or the opposite
  }
  myNex.writeStr("t0.txt", "LED STROBE\\rOFF"); // Setting the text of t0 textbox to "LED STROBE OFF"
                                                // \\r is the newline character for Nextion.
                                                // The text will look like this:
                                                // 1st line: LED STROBE
                                                // 2nd line: OFF                                                
}
