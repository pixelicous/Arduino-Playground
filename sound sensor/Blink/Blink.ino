   boolean negative = false;
   
// HARDWARE SET UP
int sensor = A0;
int sensorSensitivity = 0;
int volumeThreshold;
int volumeDivider = 0;

int ledPin1 = 9; // select the pin for the LED
int ledStatus;
int lastLedStatus = 1;

///// Silence Average checker
const int volumeReadingsAmount = 30;     // How many readings to check for average
int volumeReadings[volumeReadingsAmount];      // Readings array for the analog input
int index = 0;                  // Index in the array
int volumeTotal = 0;                  // Total readings
int volumeAverage = 0;          // the average
float volume=0,volumeLast=0,volumeLastMin=1024;


//////////// MAIN SETUP

void setup() {                
  Serial.begin(9600); // For debugging
  pinMode(ledPin1, OUTPUT);     

 // Primary Silence Average check
  for (int x =0; x<volumeReadingsAmount; x++){
     volume = analogRead(A0); // Reads the value from the Analog PIN A0
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
 
 
//////////// MAIN LOOP
 
 
void loop() {

  while(Serial.available() > 2)
  {
   volumeDivider = Serial.read();    //  Put the serial input into the message
  }
//  if (volumeDivider > 3) { volumeDivider = -volumeDivider;}    
    //   Serial.write(volumeDivider);
    volumeThreshold = volumeAverage+volumeDivider;
    
  volume = analogRead(A0); // Reads the value from the Analog PIN A0
  // Get MAX
  if (volume > volumeLast){  volumeLast=volume;}
  // Get MIN
  if( volume < volumeLast){  volumeLastMin=volume;}

/* DEBUG MODE
Serial.print("average: ");
Serial.print(volumeAverage);
Serial.print("  ");
Serial.print("MAX: ");
Serial.print(volumeLast);
Serial.print("  ");
Serial.print("LAST: ");
Serial.print(volume);
Serial.print("  ");
Serial.print("MIN: ");
Serial.print("  ");
Serial.print(volumeLastMin);
Serial.println("  ");
Serial.print("Sensibility: ");
Serial.println(volumeThreshold);
*/


//delay(1);
  if (volume>volumeThreshold) {
  LedsOn();

  }  
  else {
  LedsOff();

  }
  
  if (ledStatus != lastLedStatus) {Serial.write(ledStatus);
}
   else if (ledStatus == 1){Serial.write(ledStatus);}
  lastLedStatus = ledStatus;
 }

    
 void LedsOn(){
   digitalWrite(ledPin1, HIGH); 
  ledStatus = 1;
 }
 
  void LedsOff(){
   digitalWrite(ledPin1, LOW); 
  ledStatus = 0;
 }
