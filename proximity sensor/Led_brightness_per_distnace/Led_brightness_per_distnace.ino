const int trigPin = 12;
const int echoPin = 11;
int ledPin1 = 9; // select the pin for the LED
int ledPin2 = 6; // select the pin for the LED
int ledPin3 = 3; // select the pin for the LED
int test;
int lengthModifier =  40; // in cm
int lengthParameter = 255 / lengthModifier;

void setup() {
  // initialize serial communication:
  pinMode(ledPin1, OUTPUT); 
pinMode(ledPin2, OUTPUT); 
pinMode(ledPin3, OUTPUT); 
  pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  Serial.begin(9600);
}
 
void loop()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;
 
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.

  duration = pulseIn(echoPin, HIGH);
 
  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  test = cm*lengthParameter;
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  Serial.print(lengthParameter);

  if (cm < lengthModifier) {
  analogWrite(ledPin1,test);
  analogWrite(ledPin2,test);
  analogWrite(ledPin3,test);  
  }
  else if (cm > 150 && cm < 1000) {
    analogWrite(ledPin1,0);
  analogWrite(ledPin2,0);
  analogWrite(ledPin3,0);  
}
delay(13);
}
 
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
 
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
