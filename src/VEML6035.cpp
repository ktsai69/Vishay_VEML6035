/*
  This file is part of the Vishay_VEML6035 library.
  Copyright (c) 2021 Vishay Capella.  All rights reserved.

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
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "VEML6035.h"

#define VEML6035_ADDRESS            0x29
#define VEML6035_WHO_AM_I           0x83

#define VEML6035_REG_ALS_CONF       0x00
#define VEML6035_REG_WH             0x01
#define VEML6035_REG_WL             0x02
#define VEML6035_REG_PSM            0x03
#define VEML6035_REG_ALS            0x04
#define VEML6035_REG_WHITE          0x05
#define VEML6035_REG_ID             0x07

#define VEML6035_ALS_IT_SHIFT       6
#define VEML6035_ALS_IT_MASK        (0xF << VEML6035_ALS_IT_SHIFT)
#define VEML6035_ALS_IT_25MS        (0xC << VEML6035_ALS_IT_SHIFT)
#define VEML6035_ALS_IT_50MS        (0x8 << VEML6035_ALS_IT_SHIFT)
#define VEML6035_ALS_IT_100MS       (0x0 << VEML6035_ALS_IT_SHIFT)
#define VEML6035_ALS_IT_200MS       (0x1 << VEML6035_ALS_IT_SHIFT)
#define VEML6035_ALS_IT_400MS       (0x2 << VEML6035_ALS_IT_SHIFT)
#define VEML6035_ALS_IT_800MS       (0x3 << VEML6035_ALS_IT_SHIFT)

#define VEML6035_GAIN               (1 << 10)
#define VEML6035_DG                 (1 << 11)
#define VEML6035_SENS               (1 << 12)

#define VEML6035_DEFAULT_ALS_CONF   (VEML6035_ALS_IT_100MS | VEML6035_SENS)
#define VEML6035_DEFAULT_WH         0xFFFF
#define VEML6035_DEFAULT_WL         0x0000
#define VEML6035_DEFAULT_PSM        0x0000

VEML6035Class::VEML6035Class(TwoWire& wire) :
  _wire(&wire)
{
}

VEML6035Class::~VEML6035Class()
{
}

VEML6035Class::begin()
{
  _wire->begin();
  slaveAddress = VEML6035_ADDRESS;
  
  long id = read(VEML6035_REG_ID);
  if (id < 0 || (id & 0xFF) != VEML6035_WHO_AM_I)
	  return 0;
  
  // Initialization
  write(VEML6035_REG_ALS_CONF, VEML6035_DEFAULT_ALS_CONF);
  write(VEML6035_REG_WH, VEML6035_DEFAULT_WH);
  write(VEML6035_REG_WL, VEML6035_DEFAULT_WL);
  write(VEML6035_REG_PSM, VEML6035_DEFAULT_PSM);
  lens_factor = 1.0;
  
  return 1;
}

long VEML6035Class::read(uint8_t reg)
{
  uint8_t   wd;
  uint16_t  data;

  _wire->beginTransmission(slaveAddress);
  if (_wire->write (reg) != 1)
    goto read_error;
  _wire->endTransmission(false);
  if (_wire->requestFrom(slaveAddress, (uint8_t)2) != 2)
    goto read_error;
  
  wd = 10;
  while (_wire->available()<2 && wd)
    wd--;
  if (!wd)
    goto read_error;
  
  data = _wire->read();
  data |= _wire->read() << 8;
  return data;

read_error:
  _wire->endTransmission(true);
  return -1;
}

int VEML6035Class::write(uint8_t reg, uint16_t data)
{
  int status = 1;
  
  _wire->beginTransmission(slaveAddress);
  if ((_wire->write(reg) == 1) &&
      (_wire->write((uint8_t)(data & 0xFF)) == 1) &&
      (_wire->write((uint8_t)((data >> 8) & 0xFF)) == 1))
      status = -1;
  _wire->endTransmission(true);

  return status;
}

long VEML6035Class::read_ALS(void)
{
  return read(VEML6035_REG_ALS);
}

float VEML6035Class::get_lux(void)
{
  float resolution[] = {0.0128f, 0.0064f, 0.0032f, 0.0016f, 0.0008f, 0.0004f};
  int als_conf = read(VEML6035_REG_ALS_CONF);
  long als = read_ALS();
  uint16_t als_it = 0x0000;
  boolean gain, dg, sens;
  if (als_conf >= 0 && als >= 0)
  {
    als_it = (uint16_t)als_conf;
    als_it &= VEML6035_ALS_IT_MASK;
    als_it >>= VEML6035_ALS_IT_SHIFT;
    // map als_it to match the array as {25ms, 50ms, 100ms, 200ms, 400ms, 800ms} order
    switch (als_it)
    {
      case 0:
      case 1:
      case 2:
      case 3:
        als_it += 2;
        break;
      default:
        als_it &= 0xC;
        als_it = (als_it == 0xC) ? 0: 1;
        break;
    }
    
    dg = (als_conf & VEML6035_DG) ? 1 : 0;
    gain = (als_conf & VEML6035_GAIN) ? 1 : 0;
    sens = (als_conf & VEML6035_SENS) ? 1 : 0;

    float lux = (float)als;    
    lux *= resolution[als_it];
    if (!dg)
      lux *= 2.0f;
    if (!gain && !sens)
      lux *= 2.0f;
    else if (gain && sens)
      lux *= 8.0f;
    else if (!gain && sens)
      lux *= 16.0f;
      
    lux *= lens_factor;
    return lux;
  }
  return (float)als;
}

VEML6035Class veml6035(Wire);