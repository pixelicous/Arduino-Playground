
//#include <Servo.h>


#include <SPI.h>
#include "hsv2rgb.h"

// Clock and data pins are pins from the hardware SPI, you cannot choose them yourself.
// Data pin is MOSI (Arduino: 11, Arduino Mega: 51, Teensy 2.0: 2, Teensy 2.0++: 22) 
// Clock pin is SCK (Arduino: 13, Arduino Mega: 52, Teensy 2.0: 1, Teensy 2.0++: 21)

// You can choose the latch pin yourself.
const int ShiftPWM_latchPin=8;

// If your LED's turn on if the pin is low, set this to 1, otherwise set it to 0.
const bool ShiftPWM_invertOutputs = 0; 

#include <ShiftPWM.h>   // include ShiftPWM.h after setting the pins!

// Here you set the number of brightness levels, the update frequency and the number of shift registers.
// These values affect the load of ShiftPWM.
// Choose them wisely and use the PrintInterruptLoad() function to verify your load.
unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
int numRegisters = 4;

// HARDWARE SET UP
int ledStatus;
int lastLedStatus = 1;
int sensor = A0;
int sensorSensitivity = 0;
int volumeThreshold;
int volumeDivider = 0;
int colorRandom = 360;
int colorCounter = 0;
  
  ///// Silence Average checker
const int volumeReadingsAmount = 30;     // How many readings to check for average
int volumeReadings[volumeReadingsAmount];      // Readings array for the analog input
int index = 0;                  // Index in the array
int volumeTotal = 0;                  // Total readings
int volumeAverage = 0;          // the average
float volume=0,volumeLast=0,volumeLastMin=1024;


  int hue, sat, val; 
  unsigned char red, green, blue;
  
  
  
void setup()   {                
  pinMode(ShiftPWM_latchPin, OUTPUT);

  SPI.setBitOrder(LSBFIRST); // The least significant bit shoult be sent out by the SPI port first.
  // Here you can set the clock speed of the SPI port. Default is DIV4, which is 4MHz with a 16Mhz system clock.
  // If you encounter problems due to long wires or capacitive loads, try lowering the SPI clock.
  SPI.setClockDivider(SPI_CLOCK_DIV4); 
  SPI.begin(); 

  Serial.begin(9600);

  ShiftPWM.SetAmountOfRegisters(numRegisters);
  ShiftPWM.Start(pwmFrequency,maxBrightness);  

 // Primary Silence Average check
  for (int x =0; x<volumeReadingsAmount; x++){
    delay(1);
  volume = analogRead(A0); // Reads the value from the Analog PIN A0
       Serial.println(volume);
  volumeTotal= volumeTotal - volumeReadings[index];         // subtract the last reading:
  volumeReadings[index] = analogRead(sensor);               // read from the sensor:  
  volumeTotal= volumeTotal + volumeReadings[index];         // add the reading to the total:
  index = index + 1;                                        // advance to the next position in the array:  

  if (index >= volumeReadingsAmount)  { index = 0;}  // if we're at the end of the array wrap around to the beginning: 
  volumeAverage = volumeTotal / volumeReadingsAmount;           // calculate the average:
  } 

  volumeThreshold = volumeAverage+volumeDivider; // set volume threshold

  for (int x=0; x<5; x++)
  {
    LedsOn();
    delay(100);
    LedsOff();
    delay(100);
  }
}



void loop()
{    
  volumeDivider = analogRead(A1);
  volumeThreshold = volumeAverage+volumeDivider;
  if (colorCounter == 300) {
    colorCounter = 0;
    colorRandom = random(360); 
  }
    volume = analogRead(A0); // Reads the value from the Analog PIN A0
        delay(1);


//delay(1);
  if (volume>volumeThreshold) {
  LedsOn();
    ledStatus = 1;
  }  
  else {
  LedsOff();
    ledStatus = 0;
  }
  /*
  if (ledStatus != lastLedStatus) {ShiftPWM.SetAll(0);}
   else if (ledStatus == 1){LedsOn();}
         */
           lastLedStatus = ledStatus;
           ShiftPWM.SetAll(0);
           colorCounter += 1;
        }
        
 void LedsOn(){
        hue = colorRandom;
        sat = 255;
        val = 255;
       for(int led=0;led<(numRegisters*8/3);led++){ // loop over all LED's
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        ShiftPWM.SetGroupOf3(led, red, green, blue); // write the rgb values
      }

 }
 
  void LedsOff(){
   ShiftPWM.SetAll(0);

 }
