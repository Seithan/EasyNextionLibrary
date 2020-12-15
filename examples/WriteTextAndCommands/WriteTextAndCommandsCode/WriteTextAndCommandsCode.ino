/*
 * WriteTextAndCommands.ino - Simple example code for EasyNextionLibrary
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
 
#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary

EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex >
                       // Set as parameter the Hardware Serial you are going to use

void setup(){
  myNex.begin(9600); // Begin the object with a baud rate of 9600
                       // If no parameter was given in the begin(), the default baud rate of 9600 will be used
}

void loop(){
  /* Use writeStr(String) for sending commands. 
   * Use ONLY the first parameter
   * In the first parameter, write the command according to the instructions of Nextion's Instruction Set 
   * Do NOT write ANYTHING in the second parameter. Leave it empty.
   * TIP: Write in the debug mode the command to check if it is written correctly
   * example: The command to change the page and go to page0, is:  page page0(page0 is the name of the page) 
   * or page 0(0 is the ID of the page)
   */
  myNex.writeStr("page page0"); // Sending this command to change the page we are on Nextion using pageName
  delay(50); // Some time for Nextion to execute the command
  /* Use writeStr(String, String) to change the text in a textbox  
   * Use BOTH parameters
   * In the first parameter, write the objectName.textAttribute example: t0.txt or b0.txt
   * In the second parameter, write the text you want to "print"
   * Any previous text on the textbox is deleted
   */
  myNex.writeStr("t0.txt", "You are now transferred to page0"); // The text in t0 is now this
  delay(2950);

  
  myNex.writeStr("page 1"); // Sending this command to change the page we are on Nextion using pageId
  delay(50); // Some time for Nextion to execute the command
  /* By writing \\r, you send Nextion the change line character < \r >
   * The second \ is required, in order to print the \ as character
   * and not as an escape character.
   */
  myNex.writeStr("t0.txt", "You are now transferred to page1\\r");
  // Avoid using very big text Strings in the same command, as Nextion will not recognise them.
  // Istead use a second command and in order to add to the existing text, use the + symbol, after the .textAttribute("t0.txt+").
  myNex.writeStr("t0.txt+", "This is the:\\rWriteTextAndCommands example");
  myNex.writeStr("t0.txt+", "\\rEvery 3000ms we change page");
  myNex.writeStr("t0.txt+", "\\rAnd we print a text to t0");
  delay(4950);
  
  myNex.writeStr("page page2");
  delay(50); // Some time for Nextion to execute the command
  myNex.writeStr("t0.txt", "You are now transferred to page2\\r");
  myNex.writeStr("t0.txt+", "Thank you\\rfor choosing my library!!!");
  myNex.writeStr("t0.txt+", "\\rEnjoy the library!!!");
  myNex.writeStr("t0.txt+", "\\r\\rAthanasios Seitanis");
  myNex.writeStr("t0.txt+", "\\rseithagta@gmail.com");
  delay(7950);
  
  myNex.writeStr("t0.txt", "Screen will go to sleep mode in");
  myNex.writeStr("t0.txt+", "\\r3...");
  delay(1000);
  myNex.writeStr("t0.txt+", "2...");
  delay(1000);
  myNex.writeStr("t0.txt+", "1...");
  delay(1000);
  myNex.writeStr("t0.txt", "S L E E P\\rSee you in 10 seconds!!!");
  delay(1000);
  
  myNex.writeStr("sleep=1"); // Screen goes to sleep mode
  delay(10000);
  myNex.writeStr("sleep=0"); // Screen exits sleep mode
  delay(100); // Give some time to Nextion to Exit sleep mode
  
  // As these commands are using Serial to read and write, 
  // it is more preferred not to run them in the loop() without delay(); 
  // or some other method of not running them with the frequency of the loop
  // and use them only when it is needed.
  // A delay in the loop can be noticed, especially when reading from Serial
  // And of course to avoid a Serial buffer overflow
}
