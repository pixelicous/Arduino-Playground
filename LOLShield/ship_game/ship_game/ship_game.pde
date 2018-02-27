import oscP5.*;        //  Load OSC P5 library
import netP5.*;        //  Load net P5 library
import processing.serial.*;    //  Load serial library
import ddf.minim.*;

Minim minim = new Minim(this);
AudioSample blowup;
AudioSample shoot;
AudioSample gameOver;
AudioSample gameStart;
Serial arduinoPort;        //  Set arduinoPort as serial connection
OscP5 oscP5;            //  Set oscP5 as OSC connefction


//int[] button = new int [26];    //  Array allows us to add more toggle buttons in TouchOSC
int button = 0;

String arduinoMessage = " ";
void setup() {
  //size(100,100);        // Processing screen size
  //noStroke();            //  We don’t want an outline or Stroke on our graphics
    oscP5 = new OscP5(this,8000);  // Start oscP5, listening for incoming messages at port 8000
   arduinoPort = new Serial(this, "COM3", 115200);    // Set arduino to 9600 baud
   blowup = minim.loadSample("blowup.wav");
   gameStart = minim.loadSample("gamestart.wav");
   gameOver = minim.loadSample("gameover.wav");
   shoot = minim.loadSample("shoot.wav");
}

void oscEvent(OscMessage theOscMessage) {   //  This runs whenever there is a new OSC message
    String addr = theOscMessage.addrPattern();  //  Creates a string out of the OSC message
    if(addr.indexOf("/1/toggle") !=-1){   
      //int i = int((addr.charAt(9) )) - 0x30;   // returns the ASCII number so convert into a real number by subtracting 0x30
      button  = int(theOscMessage.get(0).floatValue());     //  Puts button value into      
    }
    
}

void draw() {
 while (arduinoPort.available() > 0) {
   int arduinoRead = arduinoPort.read();
   if (arduinoRead == 0) {gameStart.trigger();}
   if (arduinoRead == 1) {gameOver.trigger();}
   if (arduinoRead == 2) {blowup.trigger();}
   if (arduinoRead == 3) {shoot.trigger();}
   
 }
 
 if (button < 8) {
   arduinoPort.write(button);
 }
 else {
   arduinoPort.write(8);
 }
 button = 0;
//println(button);  

}
  
  
  void stop()
{
  // always close Minim audio classes when you are done with them
  blowup.close();
  shoot.close();
  minim.stop();
  super.stop();
}

