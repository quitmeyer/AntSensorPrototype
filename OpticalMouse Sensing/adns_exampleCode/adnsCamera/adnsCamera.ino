// Example of grabbing image frames from the ADNS-26xx optical mouse sensor and outputting them to the serial port.
// Of course, you need a suitable script/sketch/program running on the PC to catch them (wait for 324 bytes, then send one character to acknowledge)

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

    for(int i=0; i<324; i++)  // write the raw data to the serial port
    {
      Serial.write( byte(frameBuf[i]) );
      /*
      Serial.print(frameBuf[i], BYTE);
      A quick search led me a link with the solution (https://decibel.ni.com/content/thread/11196?tstart=0), that is to replace
-  Serial.print(0, BYTE) by Serial.write( byte(0) )
-  Serial.print(1, BYTE) by Serial.write( byte(1) )
...
- and all Serial.print(xxxx, BYTE) by Serial.write(xxxx).
      */
      
    }
    
        while (Serial.available() == 0)  // wait for PC to acknowledge it's ready for the next frame by sending a character
    {
    }
    Serial.flush();
}
