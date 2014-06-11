// Simple Capacitive Sense Example

//Uses one input wire hooked up through a 330kOhm resistor to the Vin line
//Usually cap sensing requires two lines

//Adapted from: UdK DRL EIT  Jussi Mikonen
//

//Output Pin
int ledPin = 0;      // select the pin for the LED
int fadePin = 1;
//Pin hooked up to capactive load
int sensPin = PB3; // Corresponds to A3
int sensPin2 = PB4; //Corresponds to A2

int calibration = 0;
int calibration2 = 0;

int previous;

float reading;
float reading2;

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
  // calibrate pin3:
  delay(100);
  for (int i = 0; i < 8; i++) {
    calibration += chargeTime(sensPin);
    delay(20);
  }
  calibration = (calibration + 4) / 8;
  
  //cal pin 2
  delay(100);
  for (int i = 0; i < 8; i++) {
    calibration2 += chargeTime(sensPin2);
    delay(20);
  }
  calibration2 = (calibration2 + 4) / 8;


}



void loop() {
  capSensing();
    


analogWrite(ledPin, reading2*5);

analogWrite(fadePin, reading*5);

  
}



void capSensing(){
  // sensing on pin 3:
  int ctime = chargeTime(sensPin);
  reading = ctime - calibration;
  
   int dtime = chargeTime(sensPin2);
  reading2 = dtime - calibration2;

  
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



