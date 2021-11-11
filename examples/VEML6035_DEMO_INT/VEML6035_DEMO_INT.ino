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

/*
 * This demo requests to cnnect the INT pin of VEML6035 to Digital Pin 2. 
 */
 
#include <Vishay_VEML6035.h>

const byte interruptPin = 2;
volatile boolean isr_triggered = false;
float thd_percent = 1.0f;               // threshold 1%

void isr()
{
  isr_triggered = true;  
}

void setup()
{
  Serial.begin(9600);
  while (!Serial);
  Serial.println();

  if (!veml6035.begin())
  {
    Serial.println("Failed to initialize veml6035");
    while(1);
  }

  // Initial interrupt
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), isr, FALLING);
  veml6035.INT_EN_with_threshold(thd_percent);
  Serial.println("ALS\tLux");
}

void loop()
{
  if (isr_triggered)
  {
    isr_triggered = false;
    uint16_t int_flag;
    if (veml6035.read_INT_FLAG(&int_flag) &&
        (int_flag & (veml6035.INT_FLAG_H | veml6035.INT_FLAG_L)))
    {
      uint16_t als;
      if (veml6035.read_ALS(&als))
      {
        Serial.print(als);
        Serial.print('\t');    
        Serial.println(veml6035.get_lux());
      }
      veml6035.INT_EN_with_threshold(thd_percent);
    }
  }
}
