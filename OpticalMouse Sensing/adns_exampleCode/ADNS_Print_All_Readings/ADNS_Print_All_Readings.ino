/*
Sketch that exports pretty much all the readings i can get from
the ADNS 2620 optical mouse sensor (except visual camera data, 
other example will do that)
This is for an Arduino Uno type microcontroller right now 
(I wanna do an ATtiny85 Later)

Ƹ|Ʒ 2014 Hacked together by A. Quitmeyer
*/
#include "Adns.h"

int dx;
int dy;

int pos_x=0;
int pos_y=0;

byte pixsum;

byte maxpix;
byte minpix;

byte surfacequal;



Mousecam cam( 10, 11, 12, 8); // sclk, sdio, pwr, gnd 
//Not totally sure why they bother setting powers and grounds. but whatever.

void setup()
{
  Serial.begin(115200);

  pinMode(6, OUTPUT);  // optional: run an LED
  digitalWrite(6, LOW); //LED anode
  
  pinMode(7, OUTPUT); 
  digitalWrite(7, HIGH);  //LED cathode
  
  if(cam.adns_init() == 0)
  {
    Serial.println("ADNS Mouse Cam not found\r\n");
  }
  
}

void loop()
{
  
    
    //Get change in X and change in Y
    //Note: The mouse camera returns signed 8-bit values (char) for the DELTA_X and DELTA_Y; the rest return (byte)
    dx = cam.adns_get_delta_x();
    dy = cam.adns_get_delta_y();
    //Get positions
    pos_x = pos_x + dx;
    pos_y = pos_y + dy;  
  //Initial view in just Serial Monitor
    Serial.print("X=");
    Serial.print(pos_x, DEC);
    
    Serial.print("  Y=");
    Serial.print(pos_y, DEC);
    
    //Get Pixel Sum
    pixsum = cam.adns_get_pixel_sum();
    Serial.print("  PIXSUM= ");
    Serial.print(pixsum, DEC);

   //Get Min and Max Pix
    maxpix = cam.adns_get_max_pixel();
    Serial.print("  MaxPix= ");
    Serial.print(maxpix, DEC);

    minpix = cam.adns_get_min_pixel();
    Serial.print("  MinPix= ");
    Serial.print(minpix, DEC);


  //Get Surface Quality (number of features)
      surfacequal = cam.adns_get_surface_quality();
    Serial.print("  SurfaceQual= ");
    Serial.print(surfacequal, DEC);


//Later view all this data in Processing, package it up to send it
/*
    Serial.print(frameBuf[i]); //For some reason (note, i don't have an oscillator) the first pixel is bright, and the rest have weird issues scanning
    Serial.print(","); //Separate values by commas
  */
  
  
   Serial.println();

}
