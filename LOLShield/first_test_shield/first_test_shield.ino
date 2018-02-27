
#include "Charliplexing.h"
#include "Font.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

const unsigned int inhale_time_ms  = 500;
const unsigned int hold_breath_ms  = 600;
const unsigned int exhale_time_ms  = 800;
const unsigned int pause_breath_ms = 2000;
static const char test[]="OFF!   ";

void setup()                    // run once, when the sketch starts
{
    LedSign::Init(DOUBLE_BUFFER | GRAYSCALE);
}

void loop()                     // run over and over again
{ 
    int MouseX;
  int MouseY;
  if (Serial.available() >= 2) {
    MouseX = Serial.read();
    MouseY = Serial.read();
    if (MouseX>127 && MouseY<127){
        // inhale
    for (int8_t i=0; i <= SHADES-1; i++) {
        uint8_t sleep =   inhale_time_ms / SHADES 
                        + ( SHADES / 2 - i ) * ( inhale_time_ms / (SHADES * 6) );
        LedSign::Clear(i);
        LedSign::Flip(true);
        delay( sleep );
    }
    delay( hold_breath_ms );
      }f
      else if (MouseX<127 && MouseY<127){
          for (int8_t x=DISPLAY_COLS, i=0;; x--) {
	LedSign::Clear();
        for (int8_t x2=x, i2=i; x2<DISPLAY_COLS;) {
	    int8_t w = Font::Draw(test[i2], x2, 0);
	    x2 += w, i2 = (i2+1)%strlen(test);
	    if (x2 <= 0)	// off the display completely?
		x = x2, i = i2;
	}
        delay(80);
    }
      
      }
        else if (MouseX>127 && MouseY>127){
            for (int8_t x=DISPLAY_COLS, i=0;; x--) {
	LedSign::Clear();
        for (int8_t x2=x, i2=i; x2<DISPLAY_COLS;) {
	    int8_t w = Font::Draw(test[i2], x2, 0);
	    x2 += w, i2 = (i2+1)%strlen(test);
	    if (x2 <= 0)	// off the display completely?
		x = x2, i = i2;
	}
        delay(80);
    }
  }
          else if (MouseX<127 && MouseY>127){
              // exhale
    for (int8_t i=SHADES-1; i >= 0; i--) {
        uint8_t sleep =   exhale_time_ms / SHADES 
                        + ( SHADES / 2 - i ) * ( inhale_time_ms / (SHADES * 6) );
        LedSign::Clear(i);
        LedSign::Flip(true);
        delay( sleep );
    }

    // pause
    delay( pause_breath_ms );
  }
   }




}








