// Simple Capacitive Sense Example

//Uses one input wire hooked up through a 330kOhm resistor to the Vin line
//Usually cap sensing requires two lines
//Adapted from: UdK DRL EIT

//Output Pin
int valPin = 1;      // select the pin for the LED
int binaryPin = 0;      // on or off if there is anything

//Pin hooked up to capactive load
int sensPin = A3;

int calibration = 0;
int previous;

float reading;

float fadeValue=255/2;
float fadeIncrease=0.25;


void setup() {
  // declare the valPin as an OUTPUT:
  pinMode(valPin, OUTPUT); 
    pinMode(binaryPin, OUTPUT); 

        digitalWrite(binaryPin, HIGH);

  // CAPACITIVE CALLIBRATION
  // calibrate pin:
  delay(100);
  for (int i = 0; i < 8; i++) {
    calibration += chargeTime(sensPin);
    delay(20);
  }
  calibration = (calibration + 4) / 8;
 // calibration = (calibration) / 8;
      
          digitalWrite(binaryPin, LOW);


}



void loop() {
  capSensing();
  
  if (reading>0){
      digitalWrite(binaryPin, HIGH);

   fadeValue= reading; 
  }
  else{
          digitalWrite(binaryPin, LOW);

  fadeValue = 0;
  }
  
    analogWrite(valPin, fadeValue);

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

  for (i = 0; i < 16; i++) {
    if (PINB & mask) break;
  }

  PORTB &= ~mask; // pull-up off
  DDRB |= mask; // discharge

  return i;
}



int chargeTimeArduSpeak(byte pin)
{
 
  //Testing out doing it in Ardu-speak 
  //Works, i wonder if it is just slower or something?
 
/**/ 
  int i;
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
  
   for (i = 0; i < 160; i++) {
    if (digitalRead(pin)) break;
  }
  pinMode(pin, OUTPUT);
digitalWrite(pin, LOW);//added an extra?

  return i;
/**/
  /**
  byte mask = (1 << pin);
  int i;

  DDRB &= ~mask; // input
  PORTB |= mask; // pull-up on


  for (i = 0; i < 1555; i++) {
    if (PINB & mask) break;
  }

  PORTB &= ~mask; // pull-up off
  DDRB |= mask; // discharge

  return i;
  
  /**/
}



