/////////////////////////////////////////////
///// Sound activated RGB LED Mood Lamp /////
///// Version 1.0 by: Netzer Rom 2014   /////
/////////////////////////////////////////////

 
#include <SPI.h>
#include "hsv2rgb.h"
#include <ShiftPWM.h>   // include ShiftPWM.h after setting the pins!
/////////////////////////////

/////////////// SHIFT PWM Variables
const int ShiftPWM_latchPin=8;
const bool ShiftPWM_invertOutputs = 0; 

unsigned char maxBrightness = 255;
unsigned char pwmFrequency = 75;
int numRegisters = 4;

/////////////// LEDS Variables
int ledStatus;
int lastLedStatus = 1;
long ledColor = 0;

// For Rainbow functions
int hue, sat, val; 
unsigned char red, green, blue;

/////////////// Hardware Set Up
int sensorSound = A0;
int sensorPont1 = A1;
int sensorPont2 = A2;
int sensorButton1 = 7;


////////////// Sensor Value Containers
int sensorValPont1 = 0;
int sensorValPont2 = 0;
int sensorValButton1 = 1;
int sensorLastValButton1 = 1;

/////////////// Sound Variables
int volumeThreshold;
int sensorSensitivity = 25;

/////////////// Led Activation Variables
int LedDisplayTime = 100; // Interval for displays
int LedDisplayMode = 1;
int LedDisplayModeMax = 7;

//////// Different Mode variables

// Color cycle by countering
int colorCounter = 0;

// Rainbow color advancing by amount of activations
int colorShift=0;

// Candle Mode variables


// the start of the flicker (low)
static int CandFlicker_low_min = 200;
static int CandFlicker_low_max = 240;
// the end value of the flicker (high)
static int CandFlicker_high_min = 230;
static int CandFlicker_high_max = 255; 
// delay between each low-high-low cycle
// low->high |flicker_hold| high->low
static int CandFlicker_hold_min = 40; // milliseconds
static int CandFlicker_hold_max = 80; // milliseconds 
// delay after each low-high-low cycle
// low->high->low |flicker_pause| low->high...
static int CandFlicker_pause_min = 100; // milliseconds
static int CandFlicker_pause_max = 200;  // milliseconds 
// delay low to high and high to low cycle
static int CandFlicker_speed_min = 900; // microseconds
static int CandFlicker_speed_max = 1000; // microseconds
 
 
/////////// Silence Average checker variables /////////// 
const int volumeReadingsAmount = 30;     // How many readings to check for average
int volumeReadings[volumeReadingsAmount];      // Readings array for the analog input
int index = 0;                  // Index in the array
int volumeTotal = 0;                  // Total readings
int volumeAverage = 0;          // the average
float volume=0,volumeLast=0,volumeLastMin=1024;


  
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
void setup()   {                
  //// I/O Hardware Setup
  pinMode(ShiftPWM_latchPin, OUTPUT);
  pinMode(sensorSound, INPUT);
  pinMode(sensorPont1, INPUT);
  pinMode(sensorPont2, INPUT);
  pinMode(sensorButton1, INPUT);
  SPI.setBitOrder(LSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4); 
  SPI.begin(); 

  Serial.begin(9600);

  ShiftPWM.SetAmountOfRegisters(numRegisters);
  ShiftPWM.Start(pwmFrequency,maxBrightness);  

 // Primary Silence Average check
  for (int x =0; x<volumeReadingsAmount; x++){
    delay(1);
  volume = analogRead(A0); // Reads the value from the Analog PIN A0       
  volumeTotal= volumeTotal - volumeReadings[index];         // subtract the last reading:
  volumeReadings[index] = analogRead(sensorSound);               // read from the sensor:  
  volumeTotal= volumeTotal + volumeReadings[index];         // add the reading to the total:
  index = index + 1;                                        // advance to the next position in the array:  

  if (index >= volumeReadingsAmount)  { index = 0;}  // if we're at the end of the array wrap around to the beginning: 
  volumeAverage = volumeTotal / volumeReadingsAmount;           // calculate the average:
  } 

  volumeThreshold = volumeAverage+sensorPont1; // set volume threshold

  for (int x=0; x<5; x++)
  {
    LedsOn();
    delay(100);
    LedsOff();
    delay(100);
  }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{    
  
  sensorValButton1 = digitalRead(sensorButton1);
        if (sensorValButton1 != sensorLastValButton1) {
        if (sensorValButton1 == HIGH) {buttonDisplayPressed();}
      }  
      
  switch (LedDisplayMode){
  case 1: // Sound activated, Pont 1 - Sound sensitivity, Pont 2 - Color
   sensorValPont1 = analogRead(sensorPont1);
    if (sensorValPont1 != 0) {sensorValPont1 = (sensorValPont1*sensorSensitivity)/1023;}
    volumeThreshold = volumeAverage+sensorValPont1; 
    LedReadPont();
          Serial.println(ledColor);    
    volume = analogRead(A0); // Reads the value from the Analog PIN A0
    if (volume>volumeThreshold) {
  LedsOn();
    ledStatus = 1;
  }  
  else {
  LedsOff();
      ledStatus = 0;
    }
  break;
  case 2: // Candle  
  rgbLedRainbow(numRegisters*8/3, 10, 3, maxBrightness, numRegisters*8); //slower, wider than the number of LED's
  break;
  case 3:
  LedsCandle2();
  break;
  case 4:
  LedsCandleColors()
  delay(LedDisplayTime*8);
  break;
  case 5:
  LedsOnColorPicker();
  break;
  case 6:
  LedsRandom();
  break;
  case 7:
  LedsStrobe();
  break;
  LedsFadeInOut();
}
 
 
  /*
  if (ledStatus != lastLedStatus) {ShiftPWM.SetAll(0);}
   else if (ledStatus == 1){LedsOn();}
         */
           lastLedStatus = ledStatus;
           sensorLastValButton1 = sensorValButton1;
           ShiftPWM.SetAll(0);
           colorCounter += 1;
           
           delay(1);
}


/////////////////////////// Functions ////////////////////

void buttonDisplayPressed() {    
  Serial.println("Button Pressed");    
  Serial.print("Sequence: ");    
  Serial.print(LedDisplayMode);    

//  delay(300);             // pause to control the program. ensuring single mode change
  LedDisplayMode += 1;        // increment Mode
  if (LedDisplayMode > LedDisplayModeMax) {             // if mode is greater than last case, restart from mode 1
    LedDisplayMode = 1;
    Serial.println("Resetting Display Sequence");
    LedsOff();
    for (int x = 0; x<=250; x += 50) {
      ShiftPWM.SetAll(x);
    }
  }
  Serial.println("");
  LedsOff();
}

///////////////// LED READ PONT

void LedReadPont(){
    sensorValPont2 = analogRead(sensorPont2); 
    ledColor = ((long)sensorValPont2*360)/1023; 
}



/////////////////////////// LED ANIMATIONS ////////////////////


////////////////////LEDS ON
 void LedsOn(){
        hue = ledColor;
        sat = 255;
        val = 255;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        for(int led=0;led<(numRegisters*8/3);led++){ // loop over all LED's
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        ShiftPWM.SetGroupOf3(led, red, green, blue); // write the rgb values
      }
 }
 //////////////////// LEDS OFF
  void LedsOff(){
   ShiftPWM.SetAll(0);
 }
//////////////////// LedCandle

void LedsCandle2(){
  int CandFlicker_random_low_start = 0;
int CandFlicker_random_low_end = 0;
int CandFlicker_random_high = 0;
int CandFlicker_random_speed_start = 0;
int CandFlicker_random_speed_end = 0;

  int hue = 19;
  sensorValPont2 = analogRead(sensorPont2); 
int sat = ((sensorValPont2*15)/1023)+240;
int val;  
   // random time for low
  CandFlicker_random_low_start = random(CandFlicker_low_min, CandFlicker_low_max);
  CandFlicker_random_low_end = random(CandFlicker_low_min, CandFlicker_low_max);
   
  // random time for high
  CandFlicker_random_high = random(CandFlicker_high_min, CandFlicker_high_max);
   
  // random time for speed
  CandFlicker_random_speed_start = random(CandFlicker_speed_min, CandFlicker_speed_max);
  CandFlicker_random_speed_end = random(CandFlicker_speed_min, CandFlicker_speed_max);
   
  // low -> high
  
  for (int i = CandFlicker_random_low_start; i<CandFlicker_random_high; i++) {
    val=i;
    hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
       for(int led=0;led<(numRegisters*8/3);led++){ // loop over all LED's
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        ShiftPWM.SetGroupOf3(led, red, green, blue); // write the rgb values
      }
    delayMicroseconds(CandFlicker_random_speed_start);
  }
   
  // hold
  delay(random(CandFlicker_hold_min, CandFlicker_hold_max));
   
  // high -> low
  for (int i = CandFlicker_random_high; i>=CandFlicker_random_low_end; i--) {
    val=i;
    hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
       for(int led=0;led<(numRegisters*8/3);led++){ // loop over all LED's
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        ShiftPWM.SetGroupOf3(led, red, green, blue); // write the rgb values
      }
    delayMicroseconds(CandFlicker_random_speed_end);
  }
   
  // pause
  delay(random(CandFlicker_pause_min, CandFlicker_pause_max));
  
}

///////////// LED CANDLE RAINBOW

void LedsCandleColors(){
  int CandFlicker_random_low_start = 0;
int CandFlicker_random_low_end = 0;
int CandFlicker_random_high = 0;
int CandFlicker_random_speed_start = 0;
int CandFlicker_random_speed_end = 0;


   // random time for low
  CandFlicker_random_low_start = random(CandFlicker_low_min, CandFlicker_low_max);
  CandFlicker_random_low_end = random(CandFlicker_low_min, CandFlicker_low_max);
   
  // random time for high
  CandFlicker_random_high = random(CandFlicker_high_min, CandFlicker_high_max);
   
  // random time for speed
  CandFlicker_random_speed_start = random(CandFlicker_speed_min, CandFlicker_speed_max);
  CandFlicker_random_speed_end = random(CandFlicker_speed_min, CandFlicker_speed_max);
   
  // low -> high

  
  
  for (int i = CandFlicker_random_low_start; i<CandFlicker_random_high; i++) {
    rgbLedRainbow(numRegisters*8/3, 10, 3, i, numRegisters*8); 
    delayMicroseconds(CandFlicker_random_speed_start);
  }
   
  // hold
  delay(random(CandFlicker_hold_min, CandFlicker_hold_max));
   
  // high -> low
  for (int i = CandFlicker_random_high; i>=CandFlicker_random_low_end; i--) {
    rgbLedRainbow(numRegisters*8/3, 10, 3, i, numRegisters*8); 
    delayMicroseconds(CandFlicker_random_speed_end);
  }
   
  // pause
  delay(random(CandFlicker_pause_min, CandFlicker_pause_max));
  
}

///////////// LED CANDLE

void LedsCandle(){
int hue = 19;
int sat;
int val;  
int theRandom;

theRandom = random(120,256);
for (val = theRandom-70; val <=theRandom; val++){
     for(int led=0;led<(numRegisters*8/3);led++){ // loop over all LED's
        //sat = random(100,121)+135;
        sat = 255;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        ShiftPWM.SetGroupOf3(led, red, green, blue); // write the rgb values
      }
}
if (random(21)>10) { delay(random(300)+160);  }
else { delay(random(600)+260); }
for (val = theRandom-20; val >=0; val--){
     for(int led=0;led<(numRegisters*8/3);led++){ // loop over all LED's
        //sat = random(100,121)+135;
        sat = 255;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        ShiftPWM.SetGroupOf3(led, red, green, blue); // write the rgb values
      }
}

}
//////////////////// Random 2 colors

void LedsRandom(){
int hue;
int sat = 255;
int val = 255;  
     for(int led=0;led<(numRegisters*8/3);led++){ // loop over all LED's
          hue = random(37)*10;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        ShiftPWM.SetGroupOf3(led, red, green, blue); // write the rgb values
      }
  delay(800);     

}

//////////////////// RAINBOW

 void rgbLedRainbow(int numRGBLeds, int delayVal, int numCycles, int maxBrightness, int rainbowWidth){
  // Displays a rainbow spread over a few LED's (numRGBLeds), which shifts in hue. The rainbow can be wider then the real number of LED's.
  int hue, sat, val; 
  unsigned char red, green, blue;

  ShiftPWM.SetAll(0);
  if (colorShift > 360) { colorShift = 0; }

  for(int led=0;led<numRGBLeds;led++){ // loop over all LED's
        hue = ((led)*360/(rainbowWidth-1)+colorShift)%360; // Set hue from 0 to 360 from first to last led and shift the hue
        sat = 255;
        val = 255;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        ShiftPWM.SetGroupOf3(led, red, green, blue); // write the rgb values
      }
      delay(delayVal); // this delay value determines the speed of hue shift
  colorShift += 1;
    } 

/////////////////// STROBE

void LedsStrobe(){
    for (int i = 700, strobe=2; i>0; i -= 48+strobe,strobe += 2){
    delay(i);
    ShiftPWM.SetAll(maxBrightness);  
    delay(15);
    ShiftPWM.SetAll(0);  
  }
}

/////////////////// ALL ON COLOR PICKER PONT

void LedsOnColorPicker(){
  //ShiftPWM.OneByOneFast();
    LedReadPont();
    for(int led=0;led<numRegisters*8/3;led++){ // loop over all LED's
        hue = ledColor; // Set hue from 0 to 360 from first to last led and shift the hue
        sat = 255;
        val = 255;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        ShiftPWM.SetGroupOf3(led, red, green, blue); // write the rgb values
      }
}

//////////////////// FADE IN/OUT ALL LEDS

void LedsFadeInOut(){
      // Fade in all outputs
  for(int j=0;j<maxBrightness;j++){
    ShiftPWM.SetAll(j);  
    delay(5);
  }
  delay(20);
  // Fade out all outputs
  for(int j=maxBrightness;j>=0;j--){
    ShiftPWM.SetAll(j);  
  delay(5);
  }
  delay(20);
}
 
