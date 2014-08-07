// Example of grabbing image frames from the ADNS-26xx optical mouse sensor and outputting them to the serial port.
// Of course, you need a suitable script/sketch/program running on the PC to catch them (wait for 324 bytes, then send one character to acknowledge)
//Hacked by Andrew Quitmeyer to print out values
//and we can send these values to a processing sketch to see them!
#include "Adns.h"

byte frameBuf[324];  // how much memory does this thing have, anyway?

Mousecam cam( 10, 11, 12, 8); // sclk, sdio, pwr, gnd

void setup()
{
  Serial.begin(115200);

  pinMode(6, OUTPUT);  // optional: run an LED
  digitalWrite(6, LOW);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);  
  
  if(cam.adns_init() == 0)
  {
    Serial.println("Cam not found\r\n");
  }
  
}

void loop()
{

    cam.adns_get_frame(frameBuf); // get entire image frame (18*18 = 324 bytes) to frameBuf[]

//!-framestart-!
 /*   for(int i=0; i<324; i++)  // write the raw data to the serial port
    {
      Serial.print(frameBuf[i]); //For some reason (note, i don't have an oscillator) the first pixel is bright
      Serial.print(","); //Separate values by commas
    
    }
    
    

   Serial.println();//end frame
   */
   Serial.write(frameBuf,sizeof(frameBuf)/sizeof(frameBuf[0]));
   Serial.write(10);
   Serial.flush();
}
