#include <Charliplexing.h>
#include <Figure.h>
#include <Font.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

char inputIphone = 0;     //  This will hold one byte of the serial message
char messageStringC = 0;     //  This will hold one byte of the serial message
String messageString;

void setup() {  
  Serial.begin(115200);  //set serial to 9600 baud ratef
  LedSign::Init(SINGLE_BUFFER | GRAYSCALE);
}

void loop(){
    if (Serial.available() > 0) { //  Check if there is a new message
      inputIphone = Serial.read();    //  Put the serial input into the message
      }

   if (inputIphone < -69){
     int stam = (inputIphone/70);
     LedSign::Set(abs(stam),0,7);
     LedSign::Flip(false);
            delay(30);
   }
   else {   
   if (inputIphone != '1'){           
     Serial.write(inputIphone);
    }
   else {
       LedSign::Set(13,8,7);
       LedSign::Flip(false);
       delay(30);
       if (Serial.available() > 0) { //  Check if there is a new message
               LedSign::Set(13,5,7);
               LedSign::Flip(false);
               delay(250);
      messageString = Serial.readString();    //  Put the serial input into the message
               LedSign::Set(13,6,7);
               LedSign::Flip(false);
                              delay(250);

      delay(5);
      }  

               LedSign::Set(13,7,7);
               LedSign::Flip(false);
      for (int8_t x=DISPLAY_COLS, i=0;; x--) {
  	LedSign::Clear();
        for (int8_t x2=x, i2=i; x2<DISPLAY_COLS;) {
        Serial.flush();
        if (Serial.available() > 0) {inputIphone = Serial.read();}
        if (inputIphone = '0'){
        Serial.flush();
        break;
        }  
  	    int8_t w = Font::Draw(messageString[i2], x2, 0);
	      x2 += w, i2 = (i2+1)%(messageString.length());
	      if (x2 <= 0){	// off the display completely?
	        x = x2, i = i2;
              }
          delay(90);
           if (Serial.available() > 0) {inputIphone = Serial.read();}
           if (inputIphone != '0'){
             Serial.flush();
             break;}  
          }        
      }
    }
   }
}

