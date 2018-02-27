#include <Charliplexing.h>
#include <Figure.h>
#include <Font.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

int carRepeats = 1;
int carStartSpeed = 50;
int carPosition = 5;
int carVelocity = carStartSpeed;
int gameState = 0;
int gameScore = 0;

int laserCounter = 0;
int laser = random(0,9);
int laserPos = 13;
String gameString;
int inputIphone = 0;


void setup() {  
  Serial.begin(115200);  //set serial to 9600 baud ratef
  LedSign::Init(SINGLE_BUFFER | GRAYSCALE);
     gameString = "3 2 1 GO ";
     writeLED(gameString);
}

void loop(){
    LedSign::Clear(0);
    LedSign::Set(laserPos,laser,5);
    if (carVelocity <= 0) {
      laserPos -= 1; 
      carVelocity = (carStartSpeed-carRepeats);
    }
    LedSign::Set(0,carPosition,7);
    LedSign::Set(1,carPosition,7);
    if (laser == carPosition && (laserPos == 1 || laserPos == 0)) {gameState =3;}
    LedSign::Flip(true);
    
    if (Serial.available() > 0) { //  Check if there is a new message
      inputIphone = Serial.read();    //  Put the serial input into the message
      Serial.write(carPosition);
    }

   if (inputIphone == 1){
     if (carPosition<8) {carPosition += 1;}
   }
   else if (inputIphone == 2){
     if (carPosition>0) {carPosition -= 1;}
   }

    if (gameState == 3) {
      gameString = "GAME OVER! ";
     writeLED(gameString);
      String gameScoreChar = String(gameScore);
      gameScoreChar = gameScoreChar + " ";
      writeLED(gameScoreChar);
      resetGame();
    }
    

    gameScore = gameScore + 1;
    carVelocity = carVelocity - 10;
    inputIphone = 0;
    if (laserPos < 0) {
     laser =random(0,9);
     laserPos = 13;
     carRepeats += 1;
     carVelocity = (carStartSpeed-carRepeats);
    }
}

void resetGame(){
 carRepeats = 1;
 gameScore = 0;
 gameState = 0;
 carPosition = 5;
 carVelocity = carStartSpeed;
 laser = random(0,9);
 laserPos = 13;
 gameString = "3 2 1 GO ";
 writeLED(gameString);
}


void writeLED(String writeLEDInput)  {
    for (int8_t x=DISPLAY_COLS, i=0,count=0; count<writeLEDInput.length()*5; x--) {
  	LedSign::Clear();
        for (int8_t x2=x, i2=i; x2<DISPLAY_COLS;) {
         int8_t w = Font::Draw(writeLEDInput[i2], x2, 0);
	 x2 += w, i2 = (i2+1)%(writeLEDInput.length());
	  if (x2 <= 0){	// off the display completely?
	   x = x2, i = i2;
           }
        }
        count += 1;
        delay(70);
      }
  
}
