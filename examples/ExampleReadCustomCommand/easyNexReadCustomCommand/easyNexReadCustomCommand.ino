/*
 * easyNexReadCustomCommand.ino - Simple example code for EasyNextionLibrary
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


// As example we use 2 arrays (tables) of integers where we are going
// to change their values with custom commands. 
/*
      The format is the known: # len cmd id id2

  where the id referred to the position (element) of the array we want to write on
  And id2 carries the value to write on the element of array
  the custom command from Nextion: printh 23 03 4C 00 0A

  4C is the Hex for letter L and we referred to the array dataL[]
  00 Hex of Dec number 0 used as the index for each array element
  0A Hex of Dec number 10 is the value we are going to write on element 0
  After the command executed by our code, the values on dataL[] array will be

  dataL[4] = {10,0,0,0}
  Same for the dataS[] intead that cmd is the 53 Hex for letter S
        Documentation to the end of the code

*/
 int dataL[4] = {0,0,0,0}; //values 0 or 1
                          // printh 23 03 4C 00 00 
 
 int dataS[4] = {0,0,0,0}; // values from 0 to 100
                          // printh 23 03 53 00 00

#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary

EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex >
                       // Set as parameter the Hardware Serial you are going to use

void setup(){
  myNex.begin(9600); // Begin the object with a baud rate of 9600
                     // If no parameter was given in the begin(), the default baud rate of 9600 will be used
 
  delay(500);               // give Nextion some time to finish initialize
  myNex.writeStr("page 0"); // For synchronizing Nextion page in case of reset to Arduino
  delay(50);
  myNex.lastCurrentPageId = 1; // At the first run of the loop, the currentPageId and the lastCurrentPageId
                               // must have different values, due to run the function firstRefresh()
}

void loop(){
  myNex.NextionListen(); // WARNING: This function must be called repeatedly to response touch events
                         // from Nextion touch panel. Actually, you should place it in your loop function.

  firstRefresh(); 
}

void easyNexReadCustomCommand(){

 int arrayPlace; // temp variable
 int value;      // temp variable
 String numericAttribute;

 
  switch(myNex.cmdGroup){
  
    case 'L': // Or <case 0x4C:>  If 'L' matches
      // we are going to write values in specific places in the dataL[] table
      // read the next byte that determines the position on the table
      arrayPlace = myNex.readByte();
      
      // read the next byte that keeps the value for the position
      value = myNex.readByte();
      
      // update the array with the new values
      dataL[arrayPlace] = value;
      
      // show the updated values of the array on Nextion to check them
      // use this Line to update only the one that you have change
      numericAttribute = "n"; numericAttribute += arrayPlace; numericAttribute += ".val";
      myNex.writeNum(numericAttribute, dataL[arrayPlace]);
       
       // Uncomment this block to update all the values on Nextion
       /*
      myNex.writeNum("n0.val", dataL[0]);
      myNex.writeNum("n1.val", dataL[1]);
      myNex.writeNum("n2.val", dataL[2]);
      myNex.writeNum("n3.val", dataL[3]); */
      
      break;
    
    case 'S': // Or <case 0x53:>  If 'S' matches 
      //  ********* PAGE2 ON NEXTION ********
      // we are going to write values in specific places in the dataS[] table
      // from Nextion printh 23 03 53 00 and after that prints n0.val,1 in order to have one byte with the value of n0 attribute.
      // The value of n0 can take values from 0 to 255 as it is only one byte
      
      // read the next byte that determines the position on the table
      arrayPlace = myNex.readByte();
      
      // read the next byte that keeps the value for the position
      value = myNex.readByte();
      
      // update the array with the new values
      dataS[arrayPlace] = value;
      
      // show the updated values of the array on Nextion to check them
      // use this Line to update only the one that you have change
      numericAttribute = "n"; numericAttribute += (arrayPlace + 4); numericAttribute += ".val";
      myNex.writeNum(numericAttribute, dataS[arrayPlace]);
  /*     
      myNex.writeNum("n4.val", dataS[0]);
      myNex.writeNum("n5.val", dataS[1]);
      myNex.writeNum("n6.val", dataS[2]);
      myNex.writeNum("n7.val", dataS[3]);
           */
      
      break;
  }

}

void firstRefresh(){ // This function's purpose is to update the values of a new page when is first loaded,
                     // and refreshing all the components with the current values as Nextion shows the Attribute val.

  if(myNex.currentPageId != myNex.lastCurrentPageId){ // If the two variables are different, means a new page is loaded.

    
    switch(myNex.currentPageId){
      case 0:
        myNex.writeNum("n0.val", dataL[0]);
        myNex.writeNum("n1.val", dataL[1]);
        myNex.writeNum("n2.val", dataL[2]);
        myNex.writeNum("n3.val", dataL[3]);  
      break;
      
      case 1:
        myNex.writeNum("n4.val", dataS[0]);
        myNex.writeNum("n5.val", dataS[1]);
        myNex.writeNum("n6.val", dataS[2]);
        myNex.writeNum("n7.val", dataS[3]);
        
        myNex.writeNum("n0.val", dataS[0]);
        myNex.writeNum("n1.val", dataS[1]);
        myNex.writeNum("n2.val", dataS[2]);
        myNex.writeNum("n3.val", dataS[3]);
      break;

    }

    
    myNex.lastCurrentPageId = myNex.currentPageId; // Afer the refresh of the new page We make them equal,
                                                   // in order to identify the next page change.
  }
}

            /*   More for custom protocol and commands https://seithan.com/Easy-Nextion-Library/Custom-Protocol/
               
      easyNexReadCustomCommand() has a weak attribute and will be created only when user
      declare this function on the main code
      More for custom protocol and commands https://seithan.com/Easy-Nextion-Library/Custom-Protocol/
      our commands will have this format: <#> <len> <cmd> <id> <id2>
      and we must send them from Nextion as HEX with the printh command
      like: printh 23 03 4C 01 01

      <#> start marker, declares that a command is followed
      <len> declares the number of bytes that will follow
      <cmd> declares the task of the command or command group
      <id> declares the properties of the command
      <id2> a second property of the command
      
      When we send a custom command with the above format, the function NextionListen() will capture the start marker # and the len (first 2 bytes)
      and it will wait until all the bytes of the command, as we have declared with the len byte, arrive to the Serial buffer and inside the timeout limits.
      
      After that, the function will read the next byte, which is the command group and the function readCommand() takes over and through a switch command
      tries to match the _cmd variable that holds the command group value with the statements of the cases.
      
      If we do NOT have a match with the predefined, cmd of P for page and T for triggers, it will continue to the default where we store the _cmd and _len to the public variables
      cmdGroup and cmdLenght as we are going to need access to them from the main code in the next step.
      
      Next we call the the easyNexReadCustomCommand() with the precondition and ONLY if we have declared the function in the main code.
      
      From this point we can handle the assign of cmdGroup and IDs from the easyNexReadCustomCommand() in the user code, where we can go on with a switch case
      for the cmdGroup, the one that we have stored the _cmd for public use and we can call it with myObject.cmdGroup. This is why we made cmdGroup a public variable.
                     */