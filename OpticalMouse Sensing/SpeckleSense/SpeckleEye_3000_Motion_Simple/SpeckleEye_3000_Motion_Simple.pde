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

using namespace adns3000;

#include <SPI.h>

ADNS3000 sensor;

int x;
int y;

int ledPin = 13;

void loop()
{
  digitalWrite(ledPin, HIGH);

  if (sensor.readHasMoved())
  {
    digitalWrite(ledPin, LOW);

    sensor.readMotion(&x, &y);

    Serial.print(x, DEC);
    Serial.print('\t');
    Serial.print(y, DEC);
    Serial.print("\t\n\0");
  }
}

void setup()
{
  Serial.begin(115200);

  sensor.init(ARDUINO_UNO, DPI_2000, true);

  pinMode(ledPin, OUTPUT);
}
