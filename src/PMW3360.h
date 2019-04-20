/*
  PMW3366.h - Library for interfacing PMW3360 motion sensor module

  Copyright (c) 2019, Sunjun Kim
  
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef PMW3360_LIB
#define PMW3360_LIB

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <SPI.h>

// Registers
#define REG_Product_ID  0x00
#define REG_Revision_ID 0x01
#define REG_Motion  0x02
#define REG_Delta_X_L 0x03
#define REG_Delta_X_H 0x04
#define REG_Delta_Y_L 0x05
#define REG_Delta_Y_H 0x06
#define REG_SQUAL 0x07
#define REG_Raw_Data_Sum  0x08
#define REG_Maximum_Raw_data  0x09
#define REG_Minimum_Raw_data  0x0A
#define REG_Shutter_Lower 0x0B
#define REG_Shutter_Upper 0x0C
#define REG_Control 0x0D
#define REG_Config1 0x0F
#define REG_Config2 0x10
#define REG_Angle_Tune  0x11
#define REG_Frame_Capture 0x12
#define REG_SROM_Enable 0x13
#define REG_Run_Downshift 0x14
#define REG_Rest1_Rate_Lower  0x15
#define REG_Rest1_Rate_Upper  0x16
#define REG_Rest1_Downshift 0x17
#define REG_Rest2_Rate_Lower  0x18
#define REG_Rest2_Rate_Upper  0x19
#define REG_Rest2_Downshift 0x1A
#define REG_Rest3_Rate_Lower  0x1B
#define REG_Rest3_Rate_Upper  0x1C
#define REG_Observation 0x24
#define REG_Data_Out_Lower  0x25
#define REG_Data_Out_Upper  0x26
#define REG_Raw_Data_Dump 0x29
#define REG_SROM_ID 0x2A
#define REG_Min_SQ_Run  0x2B
#define REG_Raw_Data_Threshold  0x2C
#define REG_Config5 0x2F
#define REG_Power_Up_Reset  0x3A
#define REG_Shutdown  0x3B
#define REG_Inverse_Product_ID  0x3F
#define REG_LiftCutoff_Tune3  0x41
#define REG_Angle_Snap  0x42
#define REG_LiftCutoff_Tune1  0x4A
#define REG_Motion_Burst  0x50
#define REG_LiftCutoff_Tune_Timeout 0x58
#define REG_LiftCutoff_Tune_Min_Length  0x5A
#define REG_SROM_Load_Burst 0x62
#define REG_Lift_Config 0x63
#define REG_Raw_Data_Burst  0x64
#define REG_LiftCutoff_Tune2  0x65

/*
Raw burst data structure: 
  BYTE[00] = Motion    = if the 7th bit is 1, a motion is detected.
         ==> 7 bit: MOT (1 when motion is detected)
         ==> 3 bit: 0 when chip is on surface / 1 when off surface
  BYTE[01] = Observation  
  BYTE[02] = Delta_X_L = dx (LSB)
  BYTE[03] = Delta_X_H = dx (MSB) 
  BYTE[04] = Delta_Y_L = dy (LSB)
  BYTE[05] = Delta_Y_H = dy (MSB)
  BYTE[06] = SQUAL     = Surface Quality register, max 0x80
                       - Number of features on the surface = SQUAL * 8
  BYTE[07] = Raw_Data_Sum   = It reports the upper byte of an 18‐bit counter which sums all 1296 raw data in the current frame;
                             * Avg value = Raw_Data_Sum * 1024 / 1296
  BYTE[08] = Maximum_Raw_Data  = Max raw data value in current frame, max=127
  BYTE[09] = Minimum_Raw_Data  = Min raw data value in current frame, max=127
  BYTE[10] = Shutter_Upper     = Shutter LSB
  BYTE[11] = Shutter_Lower     = Shutter MSB, Shutter = shutter is adjusted to keep the average raw data values within normal operating ranges

Struct description
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
*/
struct PMW3360_DATA
{
 bool isMotion;        // True if a motion is detected. 
 bool isOnSurface;     // True when a chip is on a surface 
 int dx;               // displacement on x directions. Unit: Count. (CPI * Count = Inch value)
 int dy;               // displacement on y directions.
 byte SQUAL;           // Surface Quality register, max 0x80. Number of features on the surface = SQUAL * 8
 byte rawDataSum;      // It reports the upper byte of an 18‐bit counter which sums all 1296 raw data in the current frame; * Avg value = Raw_Data_Sum * 1024 / 1296
 byte maxRawData;      // Max raw data value in current frame, max=127
 byte minRawData;      // Min raw data value in current frame, max=127
 unsigned int shutter; // unit: clock cycles of the internal oscillator. shutter is adjusted to keep the average raw data values within normal operating ranges.
}; 

class PMW3360
{
public:
  PMW3360();  // set CPI to 800 by default.
  // begin: initialize the module, ss_pin: slave select pin, CPI: initial Count Per Inch
  bool begin(unsigned int ss_pin, unsigned int CPI = 800);    
  // setCPI: set Count Per Inch value
  void setCPI(unsigned int newCPI);
  // setCPI: get CPI value (it does read CPI register from the module)
  unsigned int getCPI();
  PMW3360_DATA readBurst();
  byte readReg(byte reg_addr);
  void writeReg(byte reg_addr, byte data);
  void prepareImage();
  byte readImagePixel();
  void endImage();

private:
  unsigned int _ss;
  bool _inBurst = false;
  unsigned long _lastBurst = 0;
  byte adns_read_reg(byte reg_addr);
  void adns_write_reg(byte reg_addr, byte data);
  void adns_upload_firmware();
  bool check_signature();  
};
//extern AdvMouse_ AdvMouse;

#endif