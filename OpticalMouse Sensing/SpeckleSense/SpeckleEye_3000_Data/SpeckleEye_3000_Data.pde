/* -------------------------------------------------------------------------
 *
 *	The SpeckleEye Library: SpeckleSense Libraries and Sample Code for Arduino
 *	Fast, Precise, Low-cost and Compact Motion Sensing using Laser Speckle
 * 
 *	Copyright (C) 2012, http://www.specklesense.org/
 *
 *	The SpeckleEye Library is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	The SpeckleEye Library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with the SpeckleEye Library. If not, see <http://www.gnu.org/licenses/>.
 *
 * -------------------------------------------------------------------------
 */

#include <adns.h>
#include <adns3000.h>
#include <SPI.h>

using namespace adns3000;
ADNS3000 sensor;

int x;
int y;
byte pix_max;
byte squal;
byte pix_accum;
int shut;

int ledPin = 13;

boolean burstMode = false;

void loop()
{
  digitalWrite(ledPin, HIGH);

  if (sensor.readHasMoved())
  {
    digitalWrite(ledPin, LOW);

    if (burstMode)
      readValuesBurst();
    else
      readValues();  

    Serial.print(x, DEC);
    Serial.print('\t');

    Serial.print(y, DEC);
    Serial.print('\t');

    Serial.print(pix_max, DEC);
    Serial.print('\t');

    Serial.print(pix_accum, DEC);
    Serial.print('\t');

    Serial.print(squal, DEC);
    Serial.print('\t');

    Serial.print(shut, DEC);
    Serial.print('\t');

    Serial.print('\n');
    Serial.print('\0');
  }
}

void setup()
{
  Serial.begin(115200);

  sensor.init(ARDUINO_UNO, DPI_250, false);

  pinMode(ledPin, OUTPUT);
}

void readValues()
{
  sensor.readMotion(&x, &y);
  pix_max = sensor.read(PIX_MAX);
  pix_accum = sensor.read(PIX_ACCUM);
  squal = sensor.read(SQUAL);
  shut = sensor.readShutter();  
}

void readValuesBurst()
{
  sensor.readBurst(&x, &y, &squal, &shut, &pix_max, &pix_accum);         
}





