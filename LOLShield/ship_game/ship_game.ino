#include <Charliplexing.h>
#include <Figure.h>
#include <Font.h>

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

int shipFireRow = 0;
int shipFire = 0;
int shipRepeats = 1;
int shipStartSpeed = 300;
int shipPosition = 5;
int shipVelocity = shipStartSpeed;
int gameState = 0;
int gameScore = 0;
int laserCounter = 0;
int laser = random(1,8);
int laserPos = 13;
String gameString;
int inputIphone = 0;


void setup() {  
  Serial.begin(115200);  //set serial to 9600 baud ratef
  LedSign::Init(SINGLE_BUFFER | GRAYSCALE);
     Serial.write(0);
     writeLED("123 GO!");
}

void loop(){
    LedSign::Clear(0);
        if (Serial.available() > 0) { //  Checks serial buffer
      inputIphone = Serial.read();    //  write back to processing received message      
      if (inputIphone == 8) {Serial.write(3);}
    }
    
//draw ship
    LedSign::Set(0,shipPosition,7);
    LedSign::Set(1,shipPosition,7);
    LedSign::Set(0,shipPosition+1,7);
    LedSign::Set(0,shipPosition-1,7);

    if (shipFire != 0) {
      LedSign::Set(shipFire,shipFireRow,5);
      shipFire += 1;
    }
    
//draw laser
    LedSign::Set(laserPos,laser,5);
    if (shipVelocity <= 0) {
      laserPos -= 1; 
      shipVelocity = 300;
    }


//ship blown
    if (laser == shipPosition && (laserPos == 1 || laserPos == 0)) {gameState =3;}
//laser blown
    if (laser == shipFireRow && laserPos == shipFire) {
    gameScore = gameScore + 1;
    for (int blowUp = 1; blowUp<50; blowUp++){
      LedSign::Set(laserPos-1,laser,random(1,8));
      LedSign::Set(laserPos+1,laser,random(1,8));
      LedSign::Set(laserPos,laser-1,random(1,8));
      LedSign::Set(laserPos,laser+1,random(1,8));
      LedSign::Set(laserPos,laser,random(2,4));
      delay(3);
    }
    Serial.write(2);
    shipFire = 0;
    shipFireRow = 13;
    laser = random(1,8);
    laserPos = 13;}
    
    LedSign::Flip(true);
    


   if (inputIphone == 0) { }
   else if (inputIphone != 8) {shipPosition = inputIphone;}
   else
   if (inputIphone == 8 && shipFire == 0) {
     shipFire = 1;
     shipFireRow = shipPosition;
    }

   
    if (shipFire == 13) {shipFire = 0;}
     shipVelocity = shipVelocity - 10;
    inputIphone = 0;
    if (laserPos < 0) {
     gameState = 3;
//   laser =random(1,8);
//   laserPos = 13;
     shipRepeats += 1;
//     shipVelocity = (shipStartSpeed-shipRepeats);
    }
    
        if (gameState == 3) {
     Serial.write(1);
     gameString = "GAME OVER! ";
     writeLED(gameString);
      String gameScoreChar = String(gameScore);
      gameScoreChar = gameScoreChar + " ";
      writeLED(gameScoreChar);
      resetGame();
    }
}

void resetGame(){
 shipFire = 0;
 shipRepeats = 1;
 gameScore = 0;
 gameState = 0;
 shipPosition = 5;
 shipVelocity = 300;
 laser = random(1,8);
 laserPos = 13;
 Serial.write(0);
 gameString = "123 GO!";
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
