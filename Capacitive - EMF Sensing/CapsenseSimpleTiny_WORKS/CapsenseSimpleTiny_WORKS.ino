// Simple Capacitive Sense Example with ATTINY 85
// Quitmeyer 2014 Public Domain
//Uses one input wire (Pin 3, second down from Top Left Corner)
//hooked up through a 1MOhm resistor to the Vin line
//Usually cap sensing requires two controllable lines, 
//this is kinda weird cheating that is super simple

//Use a 100pf capacitor between the Sensor Pin (pin 3) and the Ground, 
//for greater sensitivity
//Adapted from: UdK DRL EIT  Jussi Mikonen, via Kobakant
//

//Output Pin
int binaryTouchPin = 0;      // This pin displays Touched or NOT
int analogTouchPin = 1;     // This pin displays higher granularity like how much was it touched
//Pin hooked up to capactive load
int sensPin = PB3; //This pin works as the sensor

int calibration = 0;
int previous;

float reading; // This is the reading for how long it took to charge
//(which shows how much capacitance there is)


float binaryValue=255/2;
float fadeIncrease=0.25;


int absthresh = 1; //Threshold to determine if touched or not


void setup() {
  // declare the binaryTouchPin as an OUTPUT:
  
  //Delay 
  
  delay(2000);
  pinMode(binaryTouchPin, OUTPUT); 
  pinMode(analogTouchPin, OUTPUT);
  // CAPACITIVE CALLIBRATION
  // calibrate pin:
  delay(100);
  for (int i = 0; i < 8; i++) {
    calibration += chargeTime(sensPin);
    delay(20);
  }
  calibration = (calibration + 4) / 8;
  if(binaryValue > 255) binaryValue=255;


}



void loop() {
  
  //Do the capacitive Sensing algorithm... Determine how much capacitance
  capSensing();
  
//Binary Reading
if(abs(reading)>0) binaryValue=255;
else binaryValue=0;

//Display Touched or Not
  analogWrite(binaryTouchPin, binaryValue);
  
//Display How much it is squeezed or how close it is touched
analogWrite(analogTouchPin, reading);
  
}



void capSensing(){
  // sensing on pin 3:
  //Read how long it takes to charge up the pin
  //More Time == More Capacitance
  int ctime = chargeTime(sensPin);
  reading = ctime - calibration;
  

  previous = ctime;
  delayMicroseconds(500);

}


byte chargeTime(byte pin)
{
  byte mask = (1 << pin);
  byte i;

  DDRB &= ~mask; // input
  PORTB |= mask; // pull-up on

  for (i = 0; i < 160000; i++) {
    if (PINB & mask) break;
  }

  PORTB &= ~mask; // pull-up off
  DDRB |= mask; // discharge

  return i;
}



