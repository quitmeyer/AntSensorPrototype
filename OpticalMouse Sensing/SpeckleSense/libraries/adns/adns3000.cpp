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

#include "adns3000.h"

using namespace adns3000;

// Initialize with:
// board type = ADNS::{ ARDUINO_UNO, ARDUINO_MEGA_2560 }
// resolution = { DPI_1000 = 0, DPI_250, DPI_500, DPI_1250, DPI_1500, DPI_1750, DPI_2000 }
// use12bits = 8- or 12-bit motion reporting
void 
ADNS3000::init(int _boardType, int resolution, bool use12Bits)
{
	ADNS::init(_boardType);
	writeResolution(resolution, use12Bits);
}

// Burst read of relative x/y, surface quality, shutter time, pixel max, and pixel accum
bool
ADNS3000::readBurst(int * x, int * y, 
					byte * squal, int * shut, 
					byte * pix_max, byte * pix_accum)
{
	readBegin(MOTION_BURST); // Prepare for burst read, then read out all values

		xl = SPI.transfer(0);
		yl = SPI.transfer(0);

		if (using12Bits) // Additional read out of high bits if 12-bit
			xyh = SPI.transfer(0);

		*squal = SPI.transfer(0);
		
		shut_hi = SPI.transfer(0);
		shut_lo = SPI.transfer(0);

		*pix_max = SPI.transfer(0);
		*pix_accum = SPI.transfer(0);

	readEnd();

	// Convert relative motion data from 2's complement to decimal 
	if (using12Bits)
		convert2sComplementToDecimal(xl, yl, xyh, x, y);
	else
	{
		*x = convert2sComplementToDecimal(xl);
		*y = convert2sComplementToDecimal(yl);
	}

	*shut = (shut_hi << 8) | shut_lo; // Fuse low and high bit for shutter

	return true;
}

// Look into this -- is this correct?!
// Set tracking resolution
void
ADNS3000::writeResolution(int resolution, bool use12Bits)
{
//	MOUSE_CTRL register
//	BIT_REPORTING | RSVD | RES_EN | RES2 | RES1 | RES0 | PD | RES_D
//	7               6      5        4      3      2      1    0

	byte control = 0;

	if (use12Bits)
		bitSet(control, 7); // 0/1 = 8-bit/12-bit motion reporting - BIT_REPORTING

	bitSet(control, 5); // 0/1 disable/enable resolution settings - RES_EN (overrides RES_D)

/*	byte res = 0;

	switch (resolution)
	{
		case DPI_1000:	res = 0;	break;
		case DPI_250:	res = 1;	break;
		case DPI_500:	res = 2;	break;
		case DPI_1250:	res = 3;	break;
		case DPI_1500:	res = 4;	break;
		case DPI_1750:	res = 5;	break;
		case DPI_2000:	res = 6;	break;
	}
*/
	// only set if in the range DPI_1000 = 0, DPI_2000 = 6, see enums
	if (resolution >= DPI_1000 && resolution <= DPI_2000)
		control |= ( resolution << 2 ); // RES2-RES1-RES0

	write(MOUSE_CTRL, control);

	using12Bits = use12Bits;
}

// Returns true if motion has been detected, which means that chip is ready for motion read
bool
ADNS3000::readHasMoved()
{
	return (read(MOTION_ST) & 0x80) == 0x80;
}

// Read motion data
void
ADNS3000::readMotion(int * x, int * y)
{
	if (using12Bits)
		readMotion12bit(x, y);
	else
		readMotion8bit(x, y);
}

// Read 8-bit motion data (chip must be set to 8-bit)
void
ADNS3000::readMotion8bit(int * x, int * y)
{
//	xl = read(DELTA_X);
//	yl = read(DELTA_Y);
//	avoid redundant calls in read(...) methods, through explicit operations

	digitalWrite(CS_PIN, LOW); //select chip

		SPI.transfer(DELTA_X);		// write the register
		delayMicroseconds(tsrad);	// wait
		xl = SPI.transfer(0);	// read X

		delayMicroseconds(tsrad);	// Wait

		SPI.transfer(DELTA_Y);		// write the register
		delayMicroseconds(tsrad);	// wait
		yl = SPI.transfer(0);	// read y

	digitalWrite(CS_PIN, HIGH);

	*x = convert2sComplementToDecimal(xl);
	*y = convert2sComplementToDecimal(yl);
}

// Read 12-bit motion data (chip must be set to 12-bit)
void
ADNS3000::readMotion12bit(int * x, int * y)
{
	digitalWrite(CS_PIN, LOW); //select chip

		SPI.transfer(DELTA_X);		// write the register
		delayMicroseconds(tsrad);	// wait
		xl = SPI.transfer(0);	// read X

		delayMicroseconds(tsrad);	// Wait

		SPI.transfer(DELTA_Y);		// write the register
		delayMicroseconds(tsrad);	// wait
		yl = SPI.transfer(0);	// read y

		SPI.transfer(DELTA_XY_HIGH);// write the register
		delayMicroseconds(tsrad);	// wait
		xyh = SPI.transfer(0);	// read y

	digitalWrite(CS_PIN, HIGH); //deselect chip

	convert2sComplementToDecimal(xl, yl, xyh, x, y);
}

// Read shutter time
void 
ADNS3000::readShutter(int * shutter)
{
	shut_hi = read(SHUT_HI);
	shut_lo = read(SHUT_LO);

	*shutter = (shut_hi << 8) | shut_lo;
}

// Read shutter time
int
ADNS3000::readShutter()
{
	return ( read(SHUT_HI) << 8 ) | read(SHUT_LO);
}

// 8- or 12-bit motion data mode
bool
ADNS3000::isUsing12Bits()
{
	return using12Bits;
}

// Constructor
ADNS3000::ADNS3000() : ADNS(), using12Bits(false)
{
	tsrad = 4;
}
