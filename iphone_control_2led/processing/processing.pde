import oscP5.*;        //  Load OSC P5 library
import netP5.*;        //  Load net P5 library
import processing.serial.*;    //  Load serial library

Serial arduinoPort;        //  Set arduinoPort as serial connection
OscP5 oscP5;            //  Set oscP5 as OSC connefction


//int[] button = new int [26];    //  Array allows us to add more toggle buttons in TouchOSC
int button = 0;
int dimmer = 0;    //  Array allows us to add more toggle buttons in TouchOSC
int buttonPressed = 0;
int buttonFirst = 0;

String arduinoMessage = " ";
void setup() {
  size(100,100);        // Processing screen size
  noStroke();            //  We don’t want an outline or Stroke on our graphics
    oscP5 = new OscP5(this,8000);  // Start oscP5, listening for incoming messages at port 8000
   arduinoPort = new Serial(this, "COM3", 115200);    // Set arduino to 9600 baud
}

void oscEvent(OscMessage theOscMessage) {   //  This runs whenever there is a new OSC message
    String addr = theOscMessage.addrPattern();  //  Creates a string out of the OSC message
    if(addr.indexOf("/1/toggle") !=-1){   
      //int i = int((addr.charAt(9) )) - 0x30;   // returns the ASCII number so convert into a real number by subtracting 0x30
      button  = int(theOscMessage.get(0).floatValue());     //  Puts button value into      
    }        
    if(addr.indexOf("/1/dimmer") !=-1){
      dimmer  = int(theOscMessage.get(0).floatValue());     //  Puts button value into    
    }    
}

void draw() {
 while (arduinoPort.available() > 0) {
   int arduinoRead = arduinoPort.read();
 //  println(arduinoRead);
 }
 if (button != 1) {if (button != 0) {buttonPressed = 0;}}
 
 background(50);        // Sets the background to a dark grey, can be 0-255
  if (dimmer > 69) {
    arduinoPort.write(-dimmer);
    //println(-dimmer);
    dimmer = 0;
  }
  
  
  switch(button)   {
  case 0:   
    if (buttonPressed == 1) {
      println("**MARQUEEE STOPPED**");
      arduinoMessage = " ";
    }
    buttonPressed = 0;
    arduinoPort.write("0");
    break;
  case 1:
   
    if (buttonPressed == 0) {
      arduinoPort.write("1");
      delay(2);
      arduinoPort.write(arduinoMessage);
      println(arduinoMessage);          
      buttonPressed = 1;
    }    
    break;
    
case 3:
arduinoMessage = arduinoMessage + "Q";
break;
case 5:
arduinoMessage = arduinoMessage + "W";
break;
case 7:
arduinoMessage = arduinoMessage + "E";
break;
case 9:
arduinoMessage = arduinoMessage + "R";
break;
case 11:
arduinoMessage = arduinoMessage + "T";
break;
case 13:
arduinoMessage = arduinoMessage + "Y";
break;
case 15:
arduinoMessage = arduinoMessage + "U";
break;
case 17:
arduinoMessage = arduinoMessage + "I";
break;
case 19:
arduinoMessage = arduinoMessage + "O";
break;
case 21:
arduinoMessage = arduinoMessage + "P";
break;
case 23:
arduinoMessage = arduinoMessage + "A";
break;
case 25:
arduinoMessage = arduinoMessage + "S";
break;
case 27:
arduinoMessage = arduinoMessage + "D";
break;
case 29:
arduinoMessage = arduinoMessage + "F";
break;
case 31:
arduinoMessage = arduinoMessage + "G";
break;
case 33:
arduinoMessage = arduinoMessage + "H";
break;
case 35:
arduinoMessage = arduinoMessage + "J";
break;
case 37:
arduinoMessage = arduinoMessage + "K";
break;
case 39:
arduinoMessage = arduinoMessage + "L";
break;
case 41:
arduinoMessage = arduinoMessage + "Z";
break;
case 43:
arduinoMessage = arduinoMessage + "X";
break;
case 45:
arduinoMessage = arduinoMessage + "C";
break;
case 47:
arduinoMessage = arduinoMessage + "V";
break;
case 49:
arduinoMessage = arduinoMessage + "B";
break;
case 51:
arduinoMessage = arduinoMessage + "N";
break;
case 53:
arduinoMessage = arduinoMessage + "M";
break;
case 55:
arduinoMessage = arduinoMessage + "S";
break;
  }   
}
  


