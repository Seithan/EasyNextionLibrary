/*
 * ChangePagesAndSentFloatValuesCode.ino - Simple example code for EasyNextionLibrary
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

// Compatible for Arduino and WeMos D1 mini
 
/* This project aims to show how to manage page changes,
 * how to send different data in different pages and 
 * how to limit the data sent to Nextion to minimum
 * and to avoid unnecessary use of the Serial and 
 * a possible buffer overflow.
 */
 
/* This project also demonstrates the two ways you can send float values
 * on Nextion, as Nextion DOES NOT SUPPORT float numbers. Uses integer math
 * and does not have real or floating support.
 * The Xfloat component is used for signed 32-bit integer values. 
 * The .vvs0 sets the number of digits shown to the left of the decimal (useful for leading zeros). 
 * The .vvs1 sets the number of digits shown to the right of the decimal. 
 * Go to the refreshPage0() function and see the comments on how we can send float values 
 */
 
/* The best way to demonstrate the change of the pages,
 * is to use the DHT11 sensor, as it is very common and
 * the chances of someone having one, are very high.
 * Also, it gives us at least 3 different types of data.
 * DHT11, DHT21 or DHT22 can be used.
 */
 
//---------------------------------- 
// EasyNextionLibrary Initialization
//---------------------------------- 
#include "EasyNextionLibrary.h"  // Include EasyNextionLibrary

EasyNex myNex(Serial); // Create an object of EasyNex class with the name < myNex >
                       // Set as parameter the Hardware Serial you are going to use

//--------------------------------------
// DHT Library and Sensor Initialization
//--------------------------------------
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

#include "DHT.h"   // Include DHT library

#define DHTPIN 2   // Digital pin connected to the DHT sensor D2 for UNO, NANO, MEGA,  D4 for WeMos D1 mini

// Uncomment whatever type you're using!
#define DHTTYPE DHT11       // DHT 11
//#define DHTTYPE DHT22     // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21     // DHT 21 (AM2301)

#define GET_DATA_EVERY 2000 // DHT sensors can give us measurements every 2 seconds,
                            // as Adafruit suggests
                            
unsigned long getDataTimer = millis(); // Timer for GET_DATA_EVERY 

DHT dht(DHTPIN, DHTTYPE);

// Reading temperature or humidity takes about 250 milliseconds!

// We create the lastXX value variables, in order to store the last value that we have sent on Nextion,
// compare the last value with the new value (current value that we have take from sensor) and send 
// the new value ONLY if it is different from the last and not send data to Nextion if there is no need.

// For the needs of this project, the last value comparison might be not needed,
// but I write these, just to show how you can do it, if it is really needed, at more complicated projects.

float humidity = 0.0;     // Store the value of humidity
float lastSentHumidity = 0.0; // Store the last value of humidity that we have Sent on Nextion

float temperature = 0.0;     // Store the value of the temperature in Celsius
float lastSentTemperature = 0.0; // Store the last value of the temperature in Celsius that we have sent on Nextion

float fahrenheit = 0.0;     // Store the value of the temperature in Fahrenheit
float lastSentFahrenheit = 0.0; // Store the last value of the temperature in Fahrenheit that we have sent on Nextion

float heatIndexFahrenheit = 0.0;     // Store the value of the heat index in Fahrenheit
float lastSentHeatIndexFahrenheit = 0.0; // Store the last value of the heat index in Fahrenheit that we have sent on Nextion

float heatIndexCelsius = 0.0;     // Store the value of the heat index in Celsius
float lastSentHeatIndexCelsius = 0.0; // Store the last value of the heat index in Celsius that we have sent on Nextion

//---------------------------------- 
// Change Page Initialization
//---------------------------------- 

/* In order to update currentPageId variable with the current Id of the page, 
 * you must write at the Preinitialize Event of every page: `printh 23 02 50 XX` , where `XX` the id of the page in HEX.  
 *      For page0: `printh 23 02 50 00`
 *      For page9: `printh23 02 50 09`
 *      For page10: `printh 23 02 50 0A`
 */
#define DATA_REFRESH_RATE 1000 // The time between each Data refresh of the page
                               // Depending on the needs of the project, the DATA_REFRESH_RATE can be set
                               // to 50ms or 100ms without a problem. In this example, we use 1000ms, 
                               // as DHT sensor is a slow sensor and gives measurements every 2 seconds

unsigned long pageRefreshTimer = millis(); // Timer for DATA_REFRESH_RATE

bool newPageLoaded = false; // true when the page is first loaded ( lastCurrentPageId != currentPageId )

 
void setup(){

  myNex.begin(9600); // Begin the object with a baud rate of 9600
                     // If no parameter was given in the begin(), the default baud rate of 9600 will be used 
  dht.begin();
  
  delay(500);               // give Nextion some time to finish initialize
  myNex.writeStr("page 0"); // For synchronizing Nextion page in case of reset to Arduino
  delay(50);
  myNex.lastCurrentPageId = 1; // At the first run of the loop, the currentPageId and the lastCurrentPageId
                               // must have different values, due to run the function firstRefresh()
}

void loop(){

  myNex.NextionListen(); // This function must be called repeatedly to response touch events
                         // from Nextion touch panel. Actually, you should place it in your loop function.
  readSensorValues();
  
  refereshCurrentPage();
  
  firstRefresh();
  
}

void firstRefresh(){ // This function's purpose is to update the values of a new page when is first loaded,
                     // and refreshing all the components with the current values as Nextion shows the Attribute val.

  if(myNex.currentPageId != myNex.lastCurrentPageId){ // If the two variables are different, means a new page is loaded.
    
    newPageLoaded = true;    // A new page is loaded
                             // This variable is used as an argument at the if() statement on the refreshPageXX() voids, 
                             // in order when is true to update all the values on the page with their current values
                             // with out run a comparison with the last value.
    
    switch(myNex.currentPageId){
      case 0:
        refreshPage0();
        break;
      
      case 1:
        refreshPage1();
        break;
        
      case 2:
        refreshPage2();
        break;
        
      case 3:
        refreshPage3();
        break;
    }
    
    newPageLoaded = false;  // After we have updated the new page for the first time, we update the variable to false.
                            // Now the values updated ONLY if the new value is different from the last Sent value.
                            // See void refreshPage0()
    
    myNex.lastCurrentPageId = myNex.currentPageId; // Afer the refresh of the new page We make them equal,
                                                   // in order to identify the next page change.
  }
}

void readSensorValues(){

  if((millis() - getDataTimer) > GET_DATA_EVERY){
  
    humidity = dht.readHumidity(); // Read relative humidity
    
    temperature = dht.readTemperature(); // Read temperature as Celsius (the default)    
    fahrenheit = dht.readTemperature(true);// Read temperature as Fahrenheit (isFahrenheit = true)
    
    heatIndexFahrenheit = dht.computeHeatIndex(fahrenheit, humidity);// Compute heat index in Fahrenheit (the default)
    heatIndexCelsius = dht.computeHeatIndex(temperature, humidity, false); // Compute heat index in Celsius (isFahreheit = false)
    
    getDataTimer = millis();
    
  }
}

void refereshCurrentPage(){
// In this function we refresh the page currently loaded every DATA_REFRESH_RATE
  if((millis() - pageRefreshTimer) > DATA_REFRESH_RATE){
    switch(myNex.currentPageId){
      case 0:
        refreshPage0();
        break;
      
      case 1:
        refreshPage1();
        break;
        
      case 2:
        refreshPage2();
        break;
        
      case 3:
        refreshPage3();
        break;
     
    }
    pageRefreshTimer = millis();
  }
}

void refreshPage0(){
  // Use lastSentTemperature, in order to update the components ONLY when their value has changed 
  // and avoid sending unnecessary data over Serial.
  // Also with the newPageLoaded boolean variable, we bypass the if() argument of temperature != lastSentTemperature (last value comparison)
  // so as to update all the values on Nextion when a new page is loaded, independant of if the values have changed
  
  if(temperature != lastSentTemperature || newPageLoaded == true){
    
    String tempString = String(temperature, 1); // Convert the float value to String, in order to send it to t0 textbox on Nextion
    myNex.writeStr("t0.txt", tempString);       //Write the String value to t0 Textbox component
    
    int tempInt = temperature*10;    // We convert the float to int, in order to send it to x0 Xfloat component on Nextion
                                     // We multiply it x10, because Xfloat will put a comma automatically after the last digit
                                     // if vvs1 is set to 1
    myNex.writeNum("x0.val", tempInt);
    
    lastSentTemperature = temperature;   // We store the last value that we have sent on Nextion, we wait for the next comparison 
                                         // and send data only when the value of temperature changes
  }
}

void refreshPage1(){
  
  if(humidity != lastSentHumidity || newPageLoaded == true){
    
    String tempString = String(humidity, 1);
    myNex.writeStr("t0.txt", tempString);
    
    int tempInt = humidity*10;
    myNex.writeNum("x0.val", tempInt);
    
    lastSentHumidity = humidity;
  }
}

void refreshPage2(){

  if(fahrenheit != lastSentFahrenheit || newPageLoaded == true){
    
    String tempString = String(fahrenheit, 1);
    myNex.writeStr("t0.txt", tempString);
    
    int tempInt = fahrenheit*10;
    myNex.writeNum("x0.val", tempInt);
    
    lastSentFahrenheit = fahrenheit;
  }
}

void refreshPage3(){

  if(heatIndexCelsius != lastSentHeatIndexCelsius || newPageLoaded == true){
    
    String tempString = String(heatIndexCelsius, 1);
    myNex.writeStr("t0.txt", tempString);
    
    int tempInt = heatIndexCelsius*10;
    myNex.writeNum("x0.val", tempInt);
    
    lastSentHeatIndexCelsius = heatIndexCelsius;
  }
  
  if(heatIndexFahrenheit != lastSentHeatIndexFahrenheit || newPageLoaded == true){
    String tempString = String(heatIndexFahrenheit, 1);
    myNex.writeStr("t1.txt", tempString);
    
    int tempInt = heatIndexFahrenheit*10;
    myNex.writeNum("x1.val", tempInt);
    
    lastSentHeatIndexFahrenheit = heatIndexFahrenheit;
  }
  
}
