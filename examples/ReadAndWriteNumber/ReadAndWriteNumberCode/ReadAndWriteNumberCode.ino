/*
 * ReadAndWriteNumberCode.ino - Simple example code for EasyNextionLibrary
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
 // https://www.seithan.com 
 //********************************************************************************

 // Compatible for Arduino and WeMos D1 mini ESP8266 
 
#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary 

EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex >
                       // Set as parameter the Hardware Serial you are going to use
uint32_t number = 0;
uint32_t lastnumber = 0;

#define LOOP_TIME 2000
unsigned long timer ;

void setup(){
  myNex.begin(9600); // Begin the object with a baud rate of 9600
                     // If no parameter was given in the begin(), the default baud rate of 9600 will be used
  delay(500);        // Wait for Nextion to start
  
  timer = millis();
}

void loop(){

  if((millis() - timer) > LOOP_TIME){
    number = myNex.readNumber("n0.val");   // We read the value of n0 and store it to number variable
    
    if(number != 777777){                     // 777777 is the return value if the code fails to read the new value
      lastnumber = number;                  // The chances of getting a wrong value is one in a million.
                                            // Use this if() to ensure it if you believe it is needed.    
                                            // You can either call the readNumber funtion again 
                                            // or set a safe value in case of failure.
                                            // Ex: number = 2222; or use the last value method
    } else if(number == 777777){
      number = lastnumber;
    }
    
    myNex.writeNum("n1.val", number);       // After that, we send the number variable, as value to n1
    
    number = myNex.readNumber("page0.bco"); // Read and store the background color code to number variable
    if(number == 33823){
      myNex.writeNum("page0.bco", 63488);   // Change background color to RED(63488) if it was BLUE(33823)
    }else if(number == 63488){
      myNex.writeNum("page0.bco", 33823);   // Change background color to BLUE(33823) if it was RED(63488)
    }
                  // As these commands are using Serial to read and write, 
                  // it is more preferred not to run them in the loop() without delay(); 
                  // or some other method of not running them with the frequency of the loop
                  // and use them only when it is needed.
                  // A delay in the loop can be noticed, especially when reading from Serial
                  // And of course to avoid a Serial buffer overflow
  
    timer = millis();
               
  }
}
