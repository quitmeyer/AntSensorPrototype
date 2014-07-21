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

#ifndef _ADNS_3000_H_
#define _ADNS_3000_H_

#include "adns.h"

using namespace adns;

namespace adns3000
{
	// ADNS-3000 Registers
	const byte PROD_ID          = 0x00; // Product ID
	const byte REV_ID           = 0x01; // Revision ID
	const byte MOTION_ST        = 0x02; // Motion Status
	const byte DELTA_X          = 0x03; // Lower byte of Delta Y
	const byte DELTA_Y          = 0x04; // Lower byte of Delta X
	const byte SQUAL            = 0x05; // Surface quality
	const byte SHUT_HI          = 0x06; // Shutter open time upper byte
	const byte SHUT_LO          = 0x07; // Shutter open time lower byte

	const byte PIX_MAX          = 0x08; // Maximum pixel value
	const byte PIX_ACCUM        = 0x09; // Average pixel value
	const byte PIX_MIN          = 0x0a; // Minimum pixel value
	const byte PIX_GRAB         = 0x0b; // Pixel grabber
	const byte DELTA_XY_HIGH    = 0x0c; // Upper 4 bits of Delta x and y 
	const byte MOUSE_CTRL       = 0x0d; // Mouse control
	const byte RUN_DOWNSHIFT    = 0x0e; // Run to rest1 time
	const byte REST1_PERIOD     = 0x0f; // Rest 1 period

	const byte REST1_DOWNSHIFT  = 0x10; // Rest 1 to rest 2 time
	const byte REST2_PERIOD     = 0x11; // Rest 2 period
	const byte REST2_DOWNSHIFT  = 0x12; // Rest 2 to rest 3 time
	const byte REST3_PERIOD     = 0x13; // Rest 3 period
	const byte PERFORMANCE      = 0x22; // Performance
	const byte RESET            = 0x3A; // Reset
	const byte NOT_REV_ID       = 0x3F; // Inverted revision ID
	const byte LED_CTRL         = 0x40; // LED control

	const byte MOTION_CTRL      = 0x41; // Motion control
	const byte BURST_READ_FIRST = 0x42; // Burst read starting register
	const byte REST_MODE_CONFIG = 0x45; // Rest mode configuration
	const byte MOTION_BURST     = 0x63; // Burst read

	enum { DPI_1000 = 0, DPI_250, DPI_500, DPI_1250, DPI_1500, DPI_1750, DPI_2000 };

	class ADNS3000 : public ADNS
	{	
		bool using12Bits; 

		void readMotion8bit(int * x, int * y);
		void readMotion12bit(int * x, int * y);

	public:

		byte xl;
		byte yl;
		byte xyh;
		byte shut_lo;
		byte shut_hi;

		void	init(int boardType = adns::ARDUINO_UNO,
					 int resolution = DPI_2000, bool use12Bits = true);
		
		//specific write operations
		void	writeResolution(int resolution = DPI_2000, bool use12Bits = true);
		
		//specific read operations
		bool	readHasMoved(); //detected frame motion?
		void	readMotion(int * x, int * y); //8- or 12-bit motion
		void	readShutter(int * shutter); //shutter speed
		int		readShutter(); //shutter speed
		bool	readBurst(int * x, int * y, byte * squal, int * shut, byte * pix_max, byte * pix_accum); //read a burst

		bool	isUsing12Bits();
		
		ADNS3000();
	};
}

#endif
