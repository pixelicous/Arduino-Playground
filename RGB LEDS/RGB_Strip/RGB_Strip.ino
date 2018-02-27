
/******************************************************************************
 * This example shows how to use the ShiftPWM library to PWM many outputs.
 * All shift registers are chained, so they can be driven with 3 pins from the arduino.
 * A timer interrupt updates all PWM outputs according to their duty cycle setting.
 * The outputs can be inverted by making ShiftPWM_invertOutputs true.
 * 
 * How the library works:
 * The ShiftPWM class keeps a setting for the duty cycle for each output pin, which
 * can be set using the provided functions. It also keeps a counter which it compares 
 * to these duty cycles. This timer continuously runs from 0 to the maximum duty cycle.
 * 
 * A timer interrupt is configured by ShiftPWM.Start(pwmFrequency,maxBrightness).
 * The interrupt frequency is set to pwmFrequency * (maxBrightness+1).
 * Each interrupt all duty cycles are compared to the counter and the corresponding pin
 * is written 1 or 0 based on the result. Then the counter is increased by one.
 * 
 * The duration of the interrupt depends on the number of shift registers (N).
 * T = 112 + 43*N 
 * 
 * The load of the interrupt function on your program can be calculated:
 * L = Interrupt frequency * interrupt duration / clock frequency
 * L = F*(Bmax+1)*(112+43*N)/F_CPU
 * Quick reference for load:
 * 3 registers  255 maxBrightness 75Hz  load = 0.29
 * 6 registers  255 maxBrightness 75Hz  load = 0.45
 * 24 registers 100 maxBrightness 75Hz  load = 0.54
 * 48 registers  64 maxBrightness 75Hz  load = 0.66
 * 96 registers  32 maxBrightness 75Hz  load = 0.66
 * 
 * A higher interrupt load will mean less computional power for your main program,
 * so try to keep it as low as possible and at least below 0.9.
 * 
 * The following functions are used:
 * 
 * ShiftPWM.Start(int ledFrequency, int max_Brightness)		Enable ShiftPWM with desired frequency and brightness levels
 * ShiftPWM.SetAmountOfRegisters(int newAmount)			Set or change the amount of output registers. Can be changed at runtime.
 * ShiftPWM.PrintInterruptLoad()				Print information on timer usage, frequencies and interrupt load
 * ShiftPWM.OneByOneSlow()  				        Fade in and fade out all outputs slowly
 * ShiftPWM.OneByOneFast()					Fade in and fade out all outputs fast
 * ShiftPWM.SetOne(int pin, unsigned char value)		Set the duty cycle of one output
 * ShiftPWM.SetAll(unsigned char value)				Set all outputs to the same duty cycle
 * 
 * ShiftPWM.SetGroupOf2(int group, unsigned char v0, unsigned char v1);
 * ShiftPWM.SetGroupOf3(int group, unsigned char v0, unsigned char v1, unsigned char v2);
 * ShiftPWM.SetGroupOf4(int group, unsigned char v0, unsigned char v1, unsigned char v2, unsigned char v3);
 * ShiftPWM.SetGroupOf5(int group, unsigned char v0, unsigned char v1, unsigned char v2, unsigned char v3, unsigned char v4);
 * 		--> Set a group of outputs to the given values. SetGroupOf3 is useful for RGB LED's. Each LED will be a group.
 * 
 * Debug information for wrong input to functions is also send to the serial port,
 * so check the serial port when you run into problems.
 * 
 * ShiftPWM v1.04, (c) Elco Jacobs, April 2012.
 * 
 *****************************************************************************/
//#include <Servo.h>
#include <math.h>

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
int tempera;

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
}



void loop()
{    
  // Turn all LED's off.
//  ShiftPWM.SetAll(0);
     delay(1200); // this delay value determines the speed of hue shift
         tempera = analogRead(A0);
Serial.print(Thermister(tempera));
Serial.println("c");
  int hue, sat, val; 
  unsigned char red, green, blue;

      for(int led=0;led<(numRegisters*8/3);led++){ // loop over all LED's
        hue = tempera*10; // Set hue from 0 to 360 from first to last led and shift the hue
        sat = 255;
        val = 255;
        hsv2rgb(hue, sat, val, &red, &green, &blue, maxBrightness); // convert hsv to rgb values
        ShiftPWM.SetGroupOf3(led, red, green, blue); // write the rgb values
      }
    }
    

double Thermister(int RawADC) {
double Temp;
Temp = log(((10240000/RawADC) - 10000));
Temp = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * Temp * Temp ))* Temp );
Temp = Temp - 273.15;// Convert Kelvin to Celcius
return Temp;
}
