
/*
 * AnalogReadToProgressbarCode.ino - Simple example code for EasyNextionLibrary
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
 
 /* This example aims to show how you can use and manage progress bar
  * on Nextion displays and change it's value. 
  * As example we are going to display the value of an analog input.
  */
  
  //********** Progress bar's value data range is min 0 max 100 **********
  //         **** A re-map of value range must de done ****
  
  /* To update the value of a progress bar you can use the writeNum() function
   * writeNum("j0.val", 55); set the value of j0(object Name) to 55
   * writeNum("j0.val", volt); set the value of j0(object Name) equal with the value of volt variable
  */
  
  /* To make a vertical progress bar set the **dez** attribute to vertical 
   * And change the values of width and height of the progress bar.
   */
  
  /* To reverse the direction of progress bar where the default in horizontal is: left to right
   * and down to up in vertical align, subtract the value from 100 and reverse the colors
   * of foreground and background between them
   */   
   
   /* There is no need to send the value from Arduino straight to the progress bar with writeNum();
    * as most times the progress bar is not the only component on a page that must be updated
    * usually the display of the value on a box also needed 
    * On some cases a  change of color on a box or on progress bar also needed when value reach a point.
    * As the value of the progress bar must be re-mapped to 0-100 and in order to not send the same value multiple times
    * with different formats and limit the data from Serial, use the advantages of Nextion graphics to
    * create a variable and save there the value you want to appear in the progress bar.
    * Use the features provided by Nextion via the user code on a timer to update the progress bar 
    * and at the same time display the value or whatever else you need.
    */
    

#include "EasyNextionLibrary.h" // Include EasyNextionLibrary

EasyNex myNex(Serial);   // Create an object of EasyNex class with the name < myNex >
                        // Set as parameter the Hardware Serial you are going to use
                        
uint16_t voltage;  // a variable to store the reading
                 // for simplicity Reasons we do not use float and we are going to take the measure in millivolts
 

const int REFRESH_TIME = 100;           // time to refresh the Nextion page every 100 ms
unsigned long refresh_timer = millis();  // timer for refreshing Nextion's page



void setup() {

  pinMode(A0, INPUT); // set A0 pin as INPUT
 
  myNex.begin(9600); // Begin the object with a baud rate of 9600
                     // If no parameter was given in the begin(), the default baud rate of 9600 will be used

}

void loop() {
  
  if((millis()-refresh_timer) > REFRESH_TIME){ //IMPORTANT do not have serial print commands in the loop without a delay
                                              // or an if statement with a timer condition like this one.
                                              
      int tempData = analogRead(A0);  // Read the analog pin
      voltage = map(tempData, 0, 1024, 0, 5000); // same like: voltage = analogRead(A0)*5000/1024
    
      /* We Re-map the value of tempData from 0-1024 (steps) to 0-5000 millivolt
       * connect the pins of a Potentiometer on A0 pin, 5v (5000 millivolt) and GND. Outer pins to 5v and GND, middle pin to A0
       * https://www.arduino.cc/en/tutorial/potentiometer
       * Turn it over and read values from 0 to 5000 millivolts
       */
               
      myNex.writeNum("Nvoltage.val", voltage); // Nvoltage.val is a variable that we have create on Nextion.        
                                              // we send the value of the voltage variable on Arduino
                             // you can use the same name for variables on Nextion for easy recognition with a capital N in front
                             // Avoid the use of big variable names as every character is one byte to serial. 
                             // In here we use big names for the sake of example.
      refresh_timer = millis();  // Set the timer equal to millis, create a time stamp to start over the "delay"
     
    }       
}

/* The rest work is on Nextion with the code on a timers user event

n0.val=Nvoltage.val         // write Nvoltage.val to n0.val
sys0=Nvoltage.val*100/5000  // use sys0 to make the calculations
j0.val=sys0                //  add the value to the Progress bar
//
//Reverse direction progress bar
j1.val=100-sys0
//
// Vertical allign
j2.val=sys0
//
// Progress bar can take values from 0-100
// we map the value from arduino 0-5000 :
// the math type for map the range is:
// return = (value - low1) * (high2 - low2) / (high1 - low1) + low2
// as both ranges start from zero low1 and low2 = 0
// the type becomes
// return = value*hight2/hight1
// return=value*100/5000

//
// And some graphic effects
if(n0.val>3300)
{
  n0.bco=RED
  j0.pco=RED
  j1.bco=RED //bco for the reversed
  j2.pco=RED
}else
{
  n0.bco=YELLOW
  j0.pco=1024
  j1.bco=1024
  j2.pco=1024
}
*/

