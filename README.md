# PMW3360 Module Library for Arduino

This library allows an Arduino board to easily communicate with PMW3360 Module.
This library is designed for https://www.tindie.com/products/jkicklighter/pmw3360-motion-sensor/
However, other setup will also work (e.g., https://easyeda.com/Justice/New_Project-cc5450b338fd4d55bef91ec37025ab6a).

For the most basic example, please take look at [basic_polling] example.
[basic_interrupt] example is using movement interrupt pin (MT) on the module. It does SPI transmission only if any movement is detects.
[HID_mouse] example will work as a regular mouse with left/right buttons.

# PMW3360 class
* void begin(unsigned int ss_pin, unsigned int CPI = 800)
  * Initialize the sensor. ss_pin is Slave Select pin on the module. Optionally CPI value can be set.
* void setCPI(unsigned int newCPI); / unsigned int getCPI();
  * Set/get CPI (Count per Inch).
* PMW3360_DATA readBurst();
  * Read sensor motion data using burst mode operation.
  * PMW3360_DATA is a struct that contains various information about a motion.
	  - PMW3360_DATA.isMotion      : bool, True if a motion is detected. 
	  - PMW3360_DATA.isOnSurface   : bool, True when a chip is on a surface 
	  - PMW3360_DATA.dx, data.dy   : integer, displacement on x/y directions.
	  - PMW3360_DATA.SQUAL         : byte, Surface Quality register, max 0x80
	                               * Number of features on the surface = SQUAL * 8
	  - PMW3360_DATA.rawDataSum    : byte, It reports the upper byte of an 18‐bit counter 
	                               which sums all 1296 raw data in the current frame;
	                               * Avg value = Raw_Data_Sum * 1024 / 1296
	  - PMW3360_DATA.maxRawData    : byte, Max/Min raw data value in current frame, max=127
	    PMW3360_DATA.minRawData
	  - PMW3360_DATA.shutter       : unsigned int, shutter is adjusted to keep the average
	                               raw data values within normal operating ranges.
* byte readReg(byte reg_addr);
  * Read register value from the module.
* void writeReg(byte reg_addr, byte data);
  * Write register value to the module.


Notice: some part of the code is based on https://github.com/mrjohnk/PMW3360DM-T2QU
Disclaimer: This is not a PixArt official library. USE AT YOUR OWN RISK.

# License

Copyright (c) Sunjun Kim. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA

# Update log
* v1.0.0
  * Initial release

* v1.0.1
  * Bug fix on register reading (timing stabilized)
  * Sensor initialization check routine added. (begin() will return false if failed)

* v1.1.0
  * Raw frame capture functions are updated (prepareImage, readImagePixel, endImage).
  * 'Camera' example is added

