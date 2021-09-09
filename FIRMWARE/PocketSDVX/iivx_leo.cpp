/*
  Keyboard.cpp

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

#include "iivx_leo.h"

#if defined(_USING_HID)

//================================================================================
//================================================================================
//	iivx descriptor

static const uint8_t _hidReportDescriptor[] PROGMEM = {

  	0x05, 0x01,                    /* USAGE_PAGE (Generic Desktop) */ 
    0x09, 0x04,                    /* USAGE (Joystick) */ 
    0xa1, 0x01,                    /* COLLECTION (Application) */ 
    0x85, 0x04,				 	   /*   REPORT_ID set to 4 */
    /*Buttons */ 
    0x05, 0x09,                    /*     USAGE_PAGE (Button) */ 
    0x19, 0x01,                    /*     USAGE_MINIMUM (Button 1) */ 
    0x29, 0x0f,                    /*     USAGE_MAXIMUM (Button 16) */ 
    0x15, 0x00,                    /*     LOGICAL_MINIMUM (0) */ 
    0x25, 0x01,                    /*     LOGICAL_MAXIMUM (1) */ 
    0x95, 0x0f,                    /*     REPORT_COUNT (16) */ 
    0x75, 0x01,                    /*     REPORT_SIZE (1) */ 
    0x81, 0x02,                    /*     INPUT (Data,Var,Abs) */ 
    /* Reserved byte */ 
    0x95, 0x01,                      /*   REPORT_COUNT (1) */ 
    0x75, 0x01,                      /*   REPORT_SIZE (8) */ 
    0x81, 0x03,                      /*   INPUT (Cnst,Var,Abs) */ 
    /*Axis */ 
    0x05, 0x01,                    /*     USAGE_PAGE (Generic Desktop) */ 
    0x09, 0x30,                    /*     USAGE (X) */ 
    0x09, 0x31,                    /*     USAGE (Y) */ 
    0x15, 0x81,                    /*     LOGICAL_MINIMUM (0) */ 
    0x25, 0x7f,                    /*     LOGICAL_MAXIMUM (127) */ 
    0x95, 0x02,                    /*     REPORT_COUNT (2) */ 
    0x75, 0x08,                    /*     REPORT_SIZE (8) */ 
    0x81, 0x02,                    /*     INPUT (Data,Var,Abs) */ 

    //iivx led

    /*Lights config*/
    0x85, 0x05,				 /*   REPORT_ID set to 5 */ 
    0x15, 0x00,                    /*     LOGICAL_MINIMUM (0) */ 
    0x25, 0x01,                    /*     LOGICAL_MAXIMUM (1) */ 
    /*Led 1 */ 
    0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x01,                    /*     USAGE (Instance 1) */ 
    0xa1, 0x02,                    /*     COLLECTION (Logical) */ 
    0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */ 
    0x79, 0x04,                    /*       STRING_INDEX (04) */
    0x75, 0x01,                    /*       REPORT_SIZE (1) */ 
    0x95, 0x01,                    /*       REPORT_COUNT (1) */ 
    0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,                          /*     END_COLLECTION */ 
    /*Led 2 */ 
    0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x02,                    /*     USAGE (Instance 2) */ 
    0xa1, 0x02,                    /*     COLLECTION (Logical) */ 
    0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */ 
    0x79, 0x05,                    /*       STRING_INDEX (05) */
    0x75, 0x01,                    /*       REPORT_SIZE (1) */ 
    0x95, 0x01,                    /*       REPORT_COUNT (1) */ 
    0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,                          /*     END_COLLECTION */ 
    /*Led 3 */ 
    0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x03,                    /*     USAGE (Instance 3) */ 
    0xa1, 0x02,                    /*     COLLECTION (Logical) */ 
    0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */ 
    0x79, 0x06,                    /*       STRING_INDEX (06) */
    0x75, 0x01,                    /*       REPORT_SIZE (1) */ 
    0x95, 0x01,                    /*       REPORT_COUNT (1) */ 
    0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,                          /*     END_COLLECTION */ 
    /*Led 4 */ 
    0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x04,                    /*     USAGE (Instance 4) */ 
    0xa1, 0x02,                    /*     COLLECTION (Logical) */ 
    0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */ 
    0x79, 0x07,                    /*       STRING_INDEX (07) */
    0x75, 0x01,                    /*       REPORT_SIZE (1) */ 
    0x95, 0x01,                    /*       REPORT_COUNT (1) */ 
    0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,                          /*     END_COLLECTION */ 
    /*Led 5 */ 
    0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x05,                    /*     USAGE (Instance 5) */ 
    0xa1, 0x02,                    /*     COLLECTION (Logical) */ 
    0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */ 
    0x79, 0x08,                    /*       STRING_INDEX (08) */
    0x75, 0x01,                    /*       REPORT_SIZE (1) */ 
    0x95, 0x01,                    /*       REPORT_COUNT (1) */ 
    0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,                          /*     END_COLLECTION */ 
    /*Led 6 */ 
    0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x06,                    /*     USAGE (Instance 6) */ 
    0xa1, 0x02,                    /*     COLLECTION (Logical) */ 
    0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */ 
    0x79, 0x09,                    /*       STRING_INDEX (09) */
    0x75, 0x01,                    /*       REPORT_SIZE (1) */ 
    0x95, 0x01,                    /*       REPORT_COUNT (1) */ 
    0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,                          /*     END_COLLECTION */ 
    /*Led 7 */ 
    0x05, 0x0a,                    /*     USAGE_PAGE (Ordinals) */ 
    0x09, 0x07,                    /*     USAGE (Instance 7) */ 
    0xa1, 0x02,                    /*     COLLECTION (Logical) */ 
    0x05, 0x08,                    /*       USAGE_PAGE (LEDs) */ 
    0x09, 0x4b,                    /*       USAGE (Generic Indicator 1) */ 
    0x79, 0x0a,                    /*       STRING_INDEX (10) */
    0x75, 0x01,                    /*       REPORT_SIZE (1) */ 
    0x95, 0x01,                    /*       REPORT_COUNT (1) */ 
    0x91, 0x02,                    /*       OUTPUT (Data,Var,Abs) */ 
    0xc0,                          /*     END_COLLECTION */ 
  	/*  Reserved 1 bit */ 
  	0x95, 0x01,						 /*   REPORT_COUNT (1) */ 
  	0x75, 0x01,						 /*   REPORT_SIZE (1) */ 
  	0x91, 0x03,						 /*   OUTPUT (Cnst,Var,Abs) */ 
    /*Footer */ 
    0xc0                          /* END_COLLECTION */ 
};

iivx_::iivx_(void) 
{
	static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
	HID().AppendDescriptor(&node);
}

#define iivxByte 5 		// should be equivalent to sizeof(JoyState_t)

void iivx_::setState(iivxReport_t *report)
{
	uint8_t data[iivxByte];
	uint16_t buttonTmp;   
	buttonTmp = report->buttons;

	data[0] = buttonTmp & 0xFF;		// Break 32 bit button-state out into 4 bytes, to send over USB
	buttonTmp >>= 8;
	data[1] = buttonTmp & 0xFF;

	data[2] = report->xAxis;		// X axis
	data[3] = report->yAxis;		// Y axis

	//HID_SendReport(Report number, array of values in same order as HID descriptor, length)
	HID().SendReport(4, data, iivxByte);
}

iivx_ iivx;

#endif
