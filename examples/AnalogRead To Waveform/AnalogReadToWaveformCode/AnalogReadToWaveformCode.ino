
/*
 * AnalogReadToWaveformCode.ino - Simple example code for EasyNextionLibrary
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
 
 /* This example aims to show how you can use and manage single waveform
  * on Nextion displays. As an example we are going to display the value of an analog input.
  * With the same way you can manage more than one channels or waveforms
  */
  
  /* Every waveform can have up to 4 channels. They are numbered from 0 to 3
   * Don't mistake the number of channels with the ID of the channel
   * ID is 0 for the first channel, 1 for the second, 2 for the third, 3 for the fourth  
  */
  //********** Waveform's channel data range is min 0 max 255 **********
  //         **** A re-map of value range must de done ****
  /* The command to Add single value to Waveform Channel is:
   * add <waveform>,<channel>,<value>
   * <waveform> is the .id of the waveform component
   * <channel> is the channel the data will be added to
   * <value> is ASCII text of data value, or numeric value
   */
   
   /* TIP: Waveform 1 pixel column used for every data value added on x - axis
    * on y - axis as the Data range is from 0 to 255 the height of waveform must be 255 pixel
    * in order to have a represent of zero to the lower point and 255 to the higher point of the waveform
    * If the height of the waveform is not 255 pixel use the * dis * attribute for scaling the data from 10 to 1000%
    * if waveform has a height of 127 pixel set data scaling * dis * to 50. Set dis to 200 for 510 pixel waveform height
    * You can use this math type < dis = 100*(waveform height in pixels)/255 >
    */
   
   /* There is no need to send from Arduino the command add <waveform>,<channel>,<value>
    * as most times the waveform is not the only component on a page that must be updated
    * usually the display of the value on a box also needed 
    * On some cases a  change of color on a box also needed when value reach a point.
    * As the value to waveform must be re-mapped to 0-255 and in order to not send the same value multiple times
    * with different formats and limit the data from Serial, use the advantages of Nextion graphics to
    * create a variable and save there the value you want to appear in the waveform.
    * Use the features provided by Nextion via the user code on a timer to update the waveform 
    * and at the same time display the value or whatever else you need
    */

#include "EasyNextionLibrary.h" // Include EasyNextionLibrary

EasyNex myNex(Serial);   // Create an object of EasyNex class with the name < myNex >
                         // Set as parameter the Hardware Serial you are going to use
                        
uint16_t voltageGraph;   // a variable to store the reading
                 // for simplicity reasons, we do not use float and we are going to take the measure in millivolts
 

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
      voltageGraph = map(tempData, 0, 1024, 0, 5000); // same like: voltageGraph = analogRead(A0)*5000/1024
    
      /* We Re-map the value of tempData from 0-1024 (steps) to 0-5000 millivolt
       * connect the pins of a Potentiometer on A0 pin, 5v (5000 millivolt) and GND. Outer pins to 5v and GND, middle pin to A0
       * https://www.arduino.cc/en/tutorial/potentiometer
       * Turn it over and read values from 0 to 5000 millivolts
       */
               
      myNex.writeNum("NvoltageGraph.val", voltageGraph); // NvoltageGraph.val is a variable that we have create on Nextion.        
                                                        // we send the value of the voltageGraph variable on Arduino
                             // you can use the same name for variables on Nextion for easy recognition with a capital N infront
                             // Avoid the use of big variable names as every character is one byte to serial. 
                             // In here we use big names for the sake of the example.
      refresh_timer = millis();  // Set the timer equal to millis, create a time stamp to start over the "delay"
     
    }        
}

/* The rest work is on Nextion with the code on a timers user event

   sys0=NvoltageGraph.val*255/5000  // use sys0 to make the calculations
   add 2,0,sys0                  // add the value to the waveform with id=2 at first channel (0) 
   n0.val=NvoltageGraph.val      // write NvoltageGraph.val to n0.val
 //
 // Waveform can take values from 0-250
 // we map the value from arduino 0-5000 :
 // the math type for map the range is:
 // return = (value - low1) * (high2 - low2) / (high1 - low1) + low2
 // as both ranges start from zero low1 and low2 = 0 
 // the type becomes
 // return = value*hight2/hight1
 // return=value*255/5000
 //
 
 
 // And some graphic effects
if(n0.val>3300)
{
  n0.bco=RED
}else
{
  n0.bco=YELLOW
}
*/

