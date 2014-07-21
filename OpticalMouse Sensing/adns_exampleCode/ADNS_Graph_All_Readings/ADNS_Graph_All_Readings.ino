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


//For Graphing in P5
char separator[] = " | ";

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
    Serial.print("X");
            Serial.print(":");
    Serial.print(pos_x, DEC);
    
    Serial.print(separator);
    
    Serial.print("Y");
            Serial.print(":");
    Serial.print(pos_y, DEC);
    Serial.print(separator);
    //Get Pixel Sum
    pixsum = cam.adns_get_pixel_sum();
    Serial.print("PIXSUM");
            Serial.print(":");
    Serial.print(pixsum, DEC);
Serial.print(separator);
   //Get Min and Max Pix
    maxpix = cam.adns_get_max_pixel();
    Serial.print("MaxPix");
            Serial.print(":");
    Serial.print(maxpix, DEC);
Serial.print(separator);
    minpix = cam.adns_get_min_pixel();
    Serial.print("MinPix");
            Serial.print(":");
    Serial.print(minpix, DEC);
Serial.print(separator);

  //Get Surface Quality (number of features)
      surfacequal = cam.adns_get_surface_quality();
    Serial.print("SurfaceQual");
        Serial.print(":");
    Serial.print(surfacequal, DEC);


  
   Serial.println();

}
