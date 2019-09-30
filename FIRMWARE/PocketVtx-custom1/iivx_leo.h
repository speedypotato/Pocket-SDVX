/*
  Keyboard.h

  Copyright (c) 2015, Arduino LLC
  Original code (pre-library): Copyright (c) 2011, Peter Barrett

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

#ifndef iivx_h
#define iivx_h

#include "HID.h"

#if !defined(_USING_HID)

#warning "USING OLD HID CORE, UPGRADE TO ARDUINO 1.6.8"

#else

//  Low level key report: up to 6 keys and shift, ctrl etc at once
extern volatile uint16_t iivx_led;

typedef struct iivxReport     // Pretty self explanitory. Simple state to store all the joystick parameters
{
  uint16_t  buttons;
  uint8_t   xAxis;
  uint8_t   yAxis;
  uint8_t   zAxis;
} iivxReport_t;

class iivx_
{
public:
  iivx_();
  void setState(iivxReport_t *report);
  inline uint16_t readState(){ return iivx_led; };
};

extern iivx_ iivx;

#endif
#endif
