// Simple Capacitive Sense Example

//Uses one input wire hooked up through a 330kOhm resistor to the Vin line
//Usually cap sensing requires two lines

//Adapted from: UdK DRL EIT  Jussi Mikonen
//

//Output Pin
int ledPin = 0;      // select the pin for the LED
int fadePin = 1;
//Pin hooked up to capactive load
int sensPin = PB3;

int calibration = 0;
int previous;

float reading;

float fadeValue=255/2;
float fadeIncrease=0.25;


int absthresh = 2;


void setup() {
  // declare the ledPin as an OUTPUT:
  
  //Delay 
  
  delay(2000);
  pinMode(ledPin, OUTPUT); 
  pinMode(fadePin, OUTPUT);
  // CAPACITIVE CALLIBRATION
  // calibrate pin:
  delay(100);
  for (int i = 0; i < 8; i++) {
    calibration += chargeTime(sensPin);
    delay(20);
  }
  calibration = (calibration + 4) / 8;
  if(fadeValue > 255) fadeValue=255;


}



void loop() {
  capSensing();
//  fadeValue-=fadeIncrease/2;
    if(fadeValue < 0) fadeValue=0;

//  if(reading>0) fadeValue+=fadeIncrease;

//Binary Switch
//if(abs(reading)>0) fadeValue=255;
if(abs(reading)>0) fadeValue=255;

else fadeValue=0;

  analogWrite(ledPin, fadeValue);
analogWrite(fadePin, reading);
  
}



void capSensing(){
  // sensing on pin 3:
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



