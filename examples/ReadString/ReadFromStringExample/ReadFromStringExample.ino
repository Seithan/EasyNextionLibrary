/*
 * ReadFromStringExample.ino - Simple example code for EasyNextionLibrary
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
 
 /* This example aims to show how you can use the readStr() function of the Library
  * With this Function you can read a string component from Nextion
  */
 
 #include "EasyNextionLibrary.h"  // Include EasyNextionLibrary
                                 // Download the latest version https://github.com/Seithan/EasyNextionLibrary
                                 // or from Arduino's IDE Library Manager

EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex >
                       // Set as parameter the Hardware Serial you are going to use

const int REFRESH_TIME = 1000;           // time to refresh the Nextion data every 1000 ms
unsigned long refresh_timer = millis();  // timer for refreshing Nextion's page
                 
String stringFromNextion;


void setup(){
  myNex.begin(9600); // Begin the object with a baud rate of 9600
                     // If no parameter was given in the begin(), the default baud rate of 9600 will be used
  
}

void loop(){
  myNex.NextionListen(); // WARNING: This function must be called repeatedly to response touch events
                         // from Nextion touch panel. Actually, you should place it in your loop function.
                         
 if((millis() - refresh_timer) > REFRESH_TIME){ //IMPORTANT do not have serial print commands in the loop without a delay
                                                // or an if statement with a timer condition like this one.
                                              
    stringFromNextion = myNex.readStr("t0.txt");
    myNex.writeStr("t1.txt",stringFromNextion);
    
      refresh_timer = millis();  // Set the timer equal to millis, create a time stamp to start over the "delay"
     
    }                          
                         
}




