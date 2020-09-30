#include "iivx_leo.h"
#include "Encoder.h"
iivxReport_t report;
#include "Keyboard.h"
#include "Mouse.h"
#include <EEPROM.h>

// Number of microseconds between HID reports
// 2000 = 500hz
#define REPORT_DELAY 1000

// Read eeprom address 0. Value stored indicated mode. Mode1=joystick, Mode2=keyboard.
int currMode = EEPROM.read(0);

// Mouse Sens Multiplier
#define MOUSE_MULT  3

// Button Keybinds
#define BIND_A    'a'
#define BIND_B    's'
#define BIND_C    'd'
#define BIND_D    'f'
#define BIND_FX_L 'z'
#define BIND_FX_R 'x'
#define BIND_ST   KEY_RETURN

// Button Pinout
#define BT_A  5
#define BT_B  6
#define BT_C  7
#define BT_D  8
#define FX_L  9
#define FX_R  10
#define BT_ST 4

// LED Pinout
#define BT_A_LED  14
#define BT_B_LED  15
#define BT_C_LED  18
#define BT_D_LED  19
#define FX_L_LED  20
#define FX_R_LED  21
#define BT_ST_LED 16

// Number of cycles before HID falls back to reactive
unsigned long reactiveTimeoutMax = 1000;
unsigned long reactiveTimeoutCount = reactiveTimeoutMax;

uint8_t bindKeys[] = { BIND_A, BIND_B, BIND_C, BIND_D, BIND_FX_L, BIND_FX_R, BIND_ST };
uint8_t buttonPins[] = { BT_A, BT_B, BT_C, BT_D, FX_L, FX_R, BT_ST };
uint8_t ledPins[] = { BT_A_LED, BT_B_LED, BT_C_LED, BT_D_LED, FX_L_LED, FX_R_LED, BT_ST_LED };
uint8_t buttonCount = sizeof(buttonPins) / sizeof(buttonPins[0]);

// Encoder sensitivity = number of positions per rotation times 4 (24*4) / number of positions for HID report (256)
#define ENCODER_SENSITIVITY (double) 0.375
Encoder encL(3, 2), encR(1, 0);
float knob1 = 0;
float knob2 = 0;
float old_knob1 = 0;
float old_knob2 = 0;

/**
 * Light Logic for Controller & HID Mode
 * @param lightDesc Bit representation of activated lights
 */
void lights(uint16_t lightDesc) {
  for (int i = 0; i < buttonCount - 1; i++) {
    if ((lightDesc >> i) & 1) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
  // Start button light: 8 for controller reactive, 6 for HID
  if ((lightDesc >> 8) & 1 || (lightDesc >> 6) & 1) {
    digitalWrite(BT_ST_LED, HIGH);
  } else {
    digitalWrite(BT_ST_LED, LOW);
  }
}

/**
 * Arduino Setup
 */
void setup() {
  // Setup I/O for pins
  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }

  // Startup mode
  int Button1State = digitalRead(BT_A); //Read Btn-A
  int Button2State = digitalRead(BT_B); //Read Btn-B
  // Button 1 is held down: Joystick Mode
  if (Button1State == LOW && Button2State == HIGH) {
    if (currMode != 1) {
      EEPROM.update(0, 1); 
      delay(200);
    }
  } else if (Button2State == LOW && Button1State == HIGH) {
    // Button 2 is held down: Keyboard Mode
    if (currMode != 2) {
      EEPROM.update(0, 2);
      delay(200);
    }
  }
}

/**
 * Arduino Loop
 */
void loop() {
  if (EEPROM.read(0) == 1) {
    joy_mode();
  } else if (EEPROM.read(0) == 2) {
    keyboard_mode();
  }
}

/**
 * Keyboard Implementation
 */
void keyboard_mode() {
  // Read encoders
  knob1 =  (float)(encL.read());
  knob2 = (float)encR.read();

  if(knob1 != old_knob1) {
    Mouse.move((knob1 - old_knob1) * MOUSE_MULT, 0);
    
    // We count the difference in the encoders, but we must not go over arduino's int limit
    if(knob1 < -255 || knob1 > 255) {
      encL.write(0);
      old_knob1 = 0;
    } else {
      old_knob1 = knob1;
    }
  }
  
  if(knob2 != old_knob2) {
    Mouse.move(0, (knob2 - old_knob2) * MOUSE_MULT);
    
    if(knob2 < -255 || knob2 > 255) {
      encR.write(0);
      old_knob2 = 0;
    } else {
      old_knob2 = knob2;
    }
  }
  
  // Read the buttons, if it's low, output a keyboard press  
  for (int i = 0; i < buttonCount; i++) {
    if (digitalRead(buttonPins[i]) == LOW) {
      Keyboard.press(bindKeys[i]);
      digitalWrite(ledPins[i], HIGH);
    } else {
      Keyboard.release(bindKeys[i]);
      digitalWrite(ledPins[i], LOW);
    }
  }
}

/**
 * Controller Implementation
 */
void joy_mode() {
  // Read buttons except BT_ST
  for (int i = 0; i < buttonCount - 1; i++) {
    if (digitalRead(buttonPins[i]) != HIGH) {
      report.buttons |= (uint16_t)1 << i;
    } else {
      report.buttons &= ~((uint16_t)1 << i);
    }
  }
  // Read BT_ST
  if (digitalRead(BT_ST) != HIGH) {
    report.buttons |= (uint16_t)1 << 8;
  } else {
    report.buttons &= ~((uint16_t)1 << 8);
  }
  
  // Read Encoders
  report.xAxis = (uint8_t)((int32_t)(encL.read() / ENCODER_SENSITIVITY) % 256);
  report.yAxis = (uint8_t)((int32_t)(encR.read() / ENCODER_SENSITIVITY) % 256);
  
  // Reactive or HID LED switching
  if (reactiveTimeoutCount >= reactiveTimeoutMax) {
    lights(report.buttons);
  } else {
    reactiveTimeoutCount++;
    lights(iivx_led);
  }
  
  iivx.setState(&report);
  delayMicroseconds(REPORT_DELAY);
}
