
int sensorPin = A0; // select the input pin for the potentiometer
int ledPin1 = 9; // select the pin for the LED
int ledPin2 = 6; // select the pin for the LED
int ledPin3 = 3; // select the pin for the LED
int sensorValue = 0; // variable to store the value coming from the sensor

void setup() {
pinMode(ledPin1, OUTPUT); 
pinMode(ledPin2, OUTPUT); 
pinMode(ledPin3, OUTPUT); 
Serial.begin(9600); 
}

void loop() {

sensorValue = analogRead(sensorPin); 
digitalWrite(ledPin1, HIGH); 
digitalWrite(ledPin2, HIGH); 
digitalWrite(ledPin3, HIGH); 
delay(1); 
digitalWrite(ledPin1, LOW); 
digitalWrite(ledPin2, LOW); 
digitalWrite(ledPin3, LOW); 
delay(1);
Serial.println(sensorValue, DEC); 
}
