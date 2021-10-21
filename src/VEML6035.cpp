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
#define VEML6035_REG_IF             0x06
#define VEML6035_REG_ID             0x07

// ALS_CONF
#define VEML6035_SD                 (1 << 0)
#define VEML6035_INT_EN             (1 << 1)
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
// IF
#define VEML6035_IF_H               (1 << 14)
#define VEML6035_IF_L               (1 << 15)

// Default values
#define VEML6035_DEFAULT_ALS_CONF   (VEML6035_ALS_IT_100MS | VEML6035_SENS)
#define VEML6035_DEFAULT_WH         0xFFFF
#define VEML6035_DEFAULT_WL         0x0000
#define VEML6035_DEFAULT_PSM        0x0000

VEML6035Class::VEML6035Class(TwoWire& wire) : _wire(&wire)
{
}

VEML6035Class::~VEML6035Class()
{
  INT_EN(false);
}

VEML6035Class::begin()
{
  _wire->begin();
  slaveAddress = VEML6035_ADDRESS;

  // Prevent I2C bus lockup
  write(VEML6035_REG_WL, VEML6035_DEFAULT_WL);
  write(VEML6035_REG_WL, VEML6035_DEFAULT_WL);

  uint16_t id;
  if (!read(VEML6035_REG_ID, &id) || (id & 0xFF) != VEML6035_WHO_AM_I)
    return 0;
  
  // Initialization
  write(VEML6035_REG_ALS_CONF, VEML6035_DEFAULT_ALS_CONF);
  write(VEML6035_REG_WH, VEML6035_DEFAULT_WH);
  write(VEML6035_REG_WL, VEML6035_DEFAULT_WL);
  write(VEML6035_REG_PSM, VEML6035_DEFAULT_PSM);
  lens_factor = 1.0;
  
  return 1;
}

boolean VEML6035Class::read(uint8_t reg, uint16_t *data)
{
  uint8_t   wd;

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
  
  *data = _wire->read();
  *data |= _wire->read() << 8;
  return true;

read_error:
  _wire->endTransmission(true);
  return false;
}

boolean VEML6035Class::write(uint8_t reg, uint16_t data)
{
  boolean status = false;
  
  _wire->beginTransmission(slaveAddress);
  if (_wire->write(reg) &&
      _wire->write((uint8_t)(data & 0xFF)) &&
      _wire->write((uint8_t)((data >> 8) & 0xFF)))
    status = true;
  _wire->endTransmission(true);

  return status;
}

boolean VEML6035Class::bitsUpdate(uint8_t reg, uint16_t mask, uint16_t update)
{
  uint16_t value;
  
  if (!read(reg, &value))
    return false;
  value &= mask;
  value |= update;
  return write(reg, value);
}

boolean VEML6035Class::read_ALS(uint16_t *als)
{
  return read(VEML6035_REG_ALS, als);
}

float VEML6035Class::get_lux(void)
{
  uint16_t als_conf;
  uint16_t als;
  
  if (!read(VEML6035_REG_ALS_CONF, &als_conf) || !read_ALS(&als))
    return -1.0f;

  int als_it = (als_conf & VEML6035_ALS_IT_MASK) >> VEML6035_ALS_IT_SHIFT;
  
  // map als_it to match the array as {25ms, 50ms, 100ms, 200ms, 400ms, 800ms} order
  float resolution[] = {0.0128f, 0.0064f, 0.0032f, 0.0016f, 0.0008f, 0.0004f};
  als_it = (als_it <= 3) ? als_it+2 : (((als_it & 0xC) == 0xC) ? 0: 1);
    
  float lux = (float)als;    
  lux *= resolution[als_it];
  if (!(als_conf & VEML6035_DG))
    lux *= 2.0f;
  float gain_factor[] = {2.0f, 16.0f, 1.0f, 8.0f};
  int gain_id = ((als_conf & VEML6035_GAIN) ? 0b10 : 0b00) | \
                ((als_conf & VEML6035_SENS) ? 0b01 : 0b00);
  lux *= gain_factor[gain_id];
  lux *= lens_factor;
  return lux;
}

boolean VEML6035Class::INT_EN(boolean enable)
{
  return bitsUpdate(
    VEML6035_REG_ALS_CONF,
    ~VEML6035_INT_EN,
    enable ? VEML6035_INT_EN : 0);
}

boolean VEML6035Class::enableINT_with_threshold(float percent)
{
  uint16_t als;
  
  if (!read(VEML6035_REG_ALS, &als) || percent <= 0)
    return false;
 
  float thdh = (float)als * (100.0 + percent) / 100.0;
  float thdl = (float)als * (100.0 - percent) / 100.0;
  if (thdh - thdl < 1.0)
    thdh += 1.0;
  uint16_t wh = (thdh > 65535.0f) ? 65535 : (uint16_t)thdh;   
  uint16_t wl = (thdl < 0.0f) ? 0 : (uint16_t)thdl;

  if (INT_EN(false) &&
      write(VEML6035_REG_WH, wh) &&
      write(VEML6035_REG_WL, wl) &&
      INT_EN(true))
    return true;
    
  return false;
}

boolean VEML6035Class::cleanINT(void)
{
  uint16_t int_flag;

  if (read(VEML6035_REG_IF, &int_flag))
  {
    return (int_flag & (VEML6035_IF_H | VEML6035_IF_L)) ? true : false;
  }
  return false;
}

VEML6035Class veml6035(Wire);
