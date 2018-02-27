//
//
//
int mic=A0;
float sensorvalue=0,lastsensorvalue=0,lastminsensorvalue=1024;
int i;
int ledpin1 = 9;
float val;
//
//
//
void setup()
{
//sg90.attach(9);
Serial.begin(9600);
  pinMode(ledpin1, OUTPUT);      

//
// line 1 and 2 UP
//
digitalWrite(ledpin1, HIGH);
//
//
}
//
//
//
void loop()
{
// val = sensibility
// this variable can be changed to potentiometer input
//
val=130;
sensorvalue=analogRead(mic);
sensorvalue=sensorvalue/val;
//
// Get MAX
//
if (sensorvalue > lastsensorvalue)
{
  lastsensorvalue=sensorvalue;
}
//
// Get MIN
//
if( sensorvalue < lastminsensorvalue)
{
  lastminsensorvalue=sensorvalue;
}
//
//
//
Serial.print("MAX: ");
Serial.print(lastsensorvalue);
Serial.print("  ");
Serial.print("LAST: ");
Serial.print(sensorvalue);
Serial.print("  ");
Serial.print("MIN: ");
Serial.print("  ");
Serial.print(lastminsensorvalue);
Serial.print("Sensibility: ");
Serial.println(val);
delay(1);
//
// Clear led matrix
//
if (sensorvalue == lastminsensorvalue)
{
    digitalWrite(ledpin1, HIGH);
    delay(130);
}
//
//
//
else
{
    digitalWrite(ledpin1, LOW);
      delay(10);
    digitalWrite(ledpin1, HIGH);
}
}
