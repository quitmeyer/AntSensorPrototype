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

#include "adns.h"

using namespace adns;

// Initializes chip and SPI communication based on the Arduino board type
void 
ADNS::init(int _boardType)
{
	boardType = _boardType;

	// Set SPI pin numbers based on board type
	switch (boardType)
	{
		case ARDUINO_UNO:
			SCK_PIN = SCK_PIN_ARDUINO_UNO;
			MISO_PIN = MISO_PIN_ARDUINO_UNO;
			MOSI_PIN = MOSI_PIN_ARDUINO_UNO;
			CS_PIN = CS_PIN_ARDUINO_UNO;
			break;

		case ARDUINO_MEGA_2560:
			SCK_PIN = SCK_PIN_ARDUINO_MEGA_2560;
			MISO_PIN = MISO_PIN_ARDUINO_MEGA_2560;
			MOSI_PIN = MOSI_PIN_ARDUINO_MEGA_2560;
			CS_PIN = CS_PIN_ARDUINO_MEGA_2560;
			break;
	}

	// Initialize SPI
	SPI.begin();
	SPI.setDataMode(SPI_MODE3);
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV32); // can run up to DIV16 (1MHz)

//	writeResolution(resolution, use12Bits);
}

// Write value to a register
void 
ADNS::write(byte reg, byte value)
{
	// To write to a register, the ADNS expects two bytes:
	// the address byte and the data byte. 
	// The first byte has 1 as its MSB to indicate a write sequence
	reg = reg | 0x80;

	digitalWrite(CS_PIN, LOW); 	// Select chip
	SPI.transfer(reg);	// Specify register
	SPI.transfer(value); 	// Write value

	digitalWrite(CS_PIN, HIGH); // Deselect chip
}

// Basic read operation, specifying register and assuming that chip is selected.
// Optional delay in tsrad ms.
// Useful for executing multiple reads in fast sequence after chip has been selected.
byte
ADNS::readOperation(byte reg, bool delay)
{
	if (delay)
		delayMicroseconds(tsrad);

	SPI.transfer(reg);

	return SPI.transfer(0x00);
}

// Operation to read out result from register, assuming that register has been specified and that chip is selected. Optional delay in tsrad ms. Useful for burst reads. 
byte
ADNS::readResult(bool delay)
{
	if (delay)
		delayMicroseconds(tsrad);

	return SPI.transfer(0x00);
}

// Start of a read operation from a register. Typically used in the beginning of a burst read, where this command would be folowed by multiple read operations. 
void
ADNS::readBegin(byte reg)
{
	// To read from a register, the ADNS expects a register address reg,
	// the first byte has a 0 as its MSB to indicate a read sequence

	digitalWrite(CS_PIN, LOW);	// Select chip
	SPI.transfer(reg); 	// Specify register

	delayMicroseconds(tsrad);
}

// A complete read operation to read out a single value from a register.
// Includes overhead for selecting/deselecting the chip, so don't use if you expect
// to do multiple reads in sequence.
byte 
ADNS::read(byte reg)
{
	readBegin(reg); // Select chip, specify register
	byte result = SPI.transfer(0x00); // Read a byte
	readEnd(); // Deselect chip
	
	return(result); // Return the result
}

// End of a read operation, which deselects the chip.
void
ADNS::readEnd()
{
	digitalWrite(CS_PIN, HIGH); // Deselect the chip
}

// Invert one bit in byte
void
ADNS::flip(byte * b, int bit)
{
	bitWrite( *b, bit, bitRead(*b, bit) == 1 ? 0 : 1 );
}

// Convert a byte in 2's complement to decimal
int 
ADNS::convert2sComplementToDecimal(byte b)
{
/*	if (bitRead(b, 7) == 1) //negative
	{
		b = ~b;
		b += 1;
		return -b;
	}
	else
		return b;
*/
	return bitRead(b, 7) ? -( (byte)~b + 1 ) : b;
}

// Convert 2's complement integer to decimal
int
ADNS::convert2sComplementToDecimal(int b)
{
	return bitRead(b, 15) ? -( (int)~b + 1 ) : b;
}

// Convert two 12-bit values in 2's complement to two integers.
//
// TODO: Should be written more efficiently, now verbose to make it easy to follow.
// Addresses a special case when 2 values * 12 bits = 24 bits are distributed across
// 3 bytes. 
// x = xyh[7:4] + xl[7:0]
// y = xyh[3:0] + yl[7:0]
void
ADNS::convert2sComplementToDecimal(byte xl, byte yl, byte xyh, int * x, int * y)
{
	if ( bitRead(xyh, 7) ) // Sign bit set
	{
		*x = 0;

		for (int i = 7; i >= 4; i--) // Invert top bits
			bitWrite( xyh, i, !bitRead(xyh, i) );
		
		xl = ~xl; // Invert lower bits
		*x = ( (xyh & 0xF0) << 4 ) | xl; // Fuse them
		*x += 1; 
		*x = *x & 0x0FFF; // In case of overflow after addition -- needed?
		*x = -*x;
	}
	else
		*x = ( (xyh & 0xF0) << 4 ) | xl; // Fuse them

	if ( bitRead(xyh, 3) ) // Sign bit set
	{
		*y = 0;

		for (int i = 3; i >= 0; i--) // Invert top bits
			bitWrite( xyh, i, !bitRead(xyh, i) );
		
		yl = ~yl; // Invert lower bits
		*y = ( (xyh & 0x0F) << 8 ) | yl; // Fuse them
		*y += 1; 
		*y = *y & 0x0FFF; // In case of overflow after addition -- needed? 
		*y = -*y;
	}
	else
		*y = ( (xyh & 0x0F) << 8 ) | yl; // Fuse them
}

// Convert two 16-bit values in 2's complement to two integers.
void
ADNS::convert2sComplementToDecimal(byte xl, byte xh, byte yl, byte yh, int * x, int * y)
{
	*x = ( xh << 8 ) | xl; // Fuse them
	*y = ( yh << 8 ) | yl; // Fuse them

	*x = convert2sComplementToDecimal(*x);
	*y = convert2sComplementToDecimal(*y);
}

// Constructor
ADNS::ADNS() : tsrad(10)
{

}
