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

#ifndef _ADNS_H_
#define _ADNS_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SPI.h>
#include <stdlib.h>

namespace adns
{
	// SPI pins for Arduino Uno
	// http://arduino.cc/en/Hacking/PinMappingUno
	const int SCK_PIN_ARDUINO_UNO  = 13;
	const int MISO_PIN_ARDUINO_UNO = 12;
	const int MOSI_PIN_ARDUINO_UNO = 11; 
	const int CS_PIN_ARDUINO_UNO   = 10;

	// SPI pins for Arduino Mega 2560
	// http://arduino.cc/en/Hacking/PinMapping2560
	const int SCK_PIN_ARDUINO_MEGA_2560  = 52;
	const int MISO_PIN_ARDUINO_MEGA_2560 = 50;
	const int MOSI_PIN_ARDUINO_MEGA_2560 = 51; 
	const int CS_PIN_ARDUINO_MEGA_2560   = 53;

	enum { ARDUINO_UNO, ARDUINO_MEGA_2560 };

	class ADNS
	{

	public:

		// SPI pins
		int SCK_PIN;
		int MISO_PIN;
		int MOSI_PIN;
		int CS_PIN;

		// Board type, e.g., ADNS::ARDUINO_UNO
		int boardType;
		
		virtual void	init(int boardType = adns::ARDUINO_UNO);
		
		// General write operation
		virtual void	write(byte reg, byte val);
		
		// Single read operation
		virtual byte	read(byte reg); // Complete read operation for single reads (incl. activating/deactivating the chip)
		
		// Methods for multiple reads
		virtual void	readBegin(byte reg); // Preparation to read from registry
		virtual void	readEnd(); // Completes a sequence of reads

		virtual byte	readResult(bool delay = false); // Reads out single result

		virtual byte	readOperation(byte reg, bool delay = false); // Read from one registery

		
		// Static utility methods for conversions from 2's complement
		static int convert2sComplementToDecimal(byte b);
		static int convert2sComplementToDecimal(int b);
		static void convert2sComplementToDecimal(byte xl, byte yl, byte xyh, int * x, int * y);
		static void convert2sComplementToDecimal(byte xl, byte xh, byte yl, byte yh, int * x, int * y);

		// tsrad constant for needed delay between certain operations, see ADNS data sheet
		unsigned int tsrad;

		static void flip(byte * b, int bit);

		ADNS();
	};
}

#endif
