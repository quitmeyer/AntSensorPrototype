// Code by Arvid Jense 22/10/2012 at the Music As Material
// Workshop by Eric Rosenbaum and Micheal Smith-Welch in Arnhem
 
 
// CapacitiveSensor tutorial from arduino http://www.arduino.cc/playground/Code/CapacitiveSensor
//
// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//  how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher


#include "pins_arduino.h" // Arduino pre-1.0 needs this
uint8_t readCapacitivePin(int pinToMeasure) {
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;
 
  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
       if (*pin & bitmask) { cycles =  0;}
  else if (*pin & bitmask) { cycles =  1;}
  else if (*pin & bitmask) { cycles =  2;}
  else if (*pin & bitmask) { cycles =  3;}
  else if (*pin & bitmask) { cycles =  4;}
  else if (*pin & bitmask) { cycles =  5;}
  else if (*pin & bitmask) { cycles =  6;}
  else if (*pin & bitmask) { cycles =  7;}
  else if (*pin & bitmask) { cycles =  8;}
  else if (*pin & bitmask) { cycles =  9;}
  else if (*pin & bitmask) { cycles = 10;}
  else if (*pin & bitmask) { cycles = 11;}
  else if (*pin & bitmask) { cycles = 12;}
  else if (*pin & bitmask) { cycles = 13;}
  else if (*pin & bitmask) { cycles = 14;}
  else if (*pin & bitmask) { cycles = 15;}
  else if (*pin & bitmask) { cycles = 16;}
 
  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;
 
  return cycles;
}
// End of capacitive sensing code
 
 
void setup(){
Serial.begin(9600);
}
 
int bounceTimes = 6; // setting this higher, will allow you to make the MM less susseptible for false triggers and make it less bouncy (but also less responsive)
 
int touchZero=0;
int touchOne=0;
int touchTwo=0;
int touchThree=0;
int touchFour=0;
int touchFive=0;
int touchSix=0;
int touchSeven=0;
int touchEight=0;
int touchNine=0;
int touchTen=0;
int touchEleven=0;
 
void loop() {
 
  if (readCapacitivePin(0)>1){     //the rest state of the pin is 1, so if its any higher, its touched
      if(touchZero> bounceTimes){  //we're counting the number of misfires
        Serial.print("g");       //using pressed/released will work better, but would make the code a bit more complex
        touchZero=0;
     }
  }
  else{
    touchZero++;    
  }
 
   if (readCapacitivePin(1)>1){
      if(touchOne> bounceTimes){
      Serial.print("f");
      touchOne=0;
     }
  }
  else{
    touchOne++;    
  }
 
   if (readCapacitivePin(2)>1){
      if(touchTwo> bounceTimes){
      Serial.print("d");
      touchTwo=0;
     }
  }
  else{
    touchTwo++;    
  }
 
   if (readCapacitivePin(3)>1){
      if(touchThree> bounceTimes){
      Serial.print("s");
      touchThree=0;
     }
  }
  else{
    touchThree++;    
  }
 
   if (readCapacitivePin(4)>1){
      if(touchFour> bounceTimes){
      Serial.print("a");
      touchFour=0;
     }
  }
  else{
    touchFour++;    
  }
 
   if (readCapacitivePin(5)>1){
      if(touchFive> bounceTimes){
      Serial.print("w");
      touchFive=0;
     }
  }
  else{
    touchFive++;    
  }
 
    if (readCapacitivePin(6)>1){
      if(touchSix> bounceTimes){
      Serial.print("z");
      touchSix=0;
     }
  }
  else{
    touchSix++;    
  }
    if (readCapacitivePin(7)>1){
      if(touchSeven> bounceTimes){
      Serial.print("x");
      touchSeven=0;
     }
  }
  else{
    touchSeven++;    
  }
    if (readCapacitivePin(8)>1){
      if(touchEight> bounceTimes){
      Serial.print("c");//down
      touchEight=0;
     }
  }
  else{
    touchEight++;    
  }
    if (readCapacitivePin(12)>1){
      if(touchNine> bounceTimes){
      Serial.print("v"); //up
      touchNine=0;
     }
  }
  else{
    touchNine++;    
  }
    if (readCapacitivePin(13)>1){
      if(touchTen> bounceTimes){
      Serial.print("b");//left
      touchTen=0;
     }
  }
  else{
    touchTen++;    
  }
    if (readCapacitivePin(15)>1){
      if(touchEleven> bounceTimes){
      Serial.print("n"); //rightff
      touchEleven=0;
     }
  }
  else{
    touchEleven++;    
  }
 
}
