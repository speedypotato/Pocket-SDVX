#include "iivx_leo.h"
#include "Encoder.h"
iivxReport_t report;
#include "Keyboard.h"
#include "Mouse.h"
#include <EEPROM.h>

#define REPORT_DELAY 2000
// Number of microseconds between HID reports
// 2000 = 500hz

// Options, change only these values
int old_macroButtonState = HIGH;
int old_startButtonState = HIGH;
int old_btnMash = 0;
int old_Button1State = HIGH;
int old_Button2State = HIGH;
int currMode = EEPROM.read(0); // Read eeprom address 0. Value stored indicated mode. Mode1=joystick, Mode2=keyboard.
uint8_t buttonCount = 7;
uint8_t macropin = 16;

#define BT_A 5
#define BT_B 6
#define BT_C 7
#define BT_D 8
#define FX_L 9
#define FX_R 10
#define BT_ST 4

uint8_t lightMode = 0;
// 0 = reactive lighting, 1 = HID lighting
uint8_t buttonPins[] = { 4, 5, 6, 7, 8, 9, 10};
uint8_t ledPins[] = {15, 15, 15, 15, 15, 15, 15, 15, 15};


#define ENCODER_SENSITIVITY (double) 0.375
// encoder sensitivity = number of positions per rotation times 4 (24*4) / number of positions for HID report (256)
Encoder encL(3, 2), encR(1, 0);
float knob1 = 0;
float knob2 = 0;
float old_knob1 = 0;
float old_knob2 = 0;


void lights(uint8_t lightDesc) {
  for (int i = 0; i < buttonCount; i++) {
    if ((lightDesc >> i) & 1) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void setup() {
//Serial.begin(9600);
 //while (!Serial) ;

  
  delay(1000);
  // Setup I/O for pins
  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
    pinMode(macropin, INPUT_PULLUP);
  //  pinMode(sysPin,INPUT_PULLUP);
  //setup interrupts


  // startup mode
  int Button1State = digitalRead(5); //Read Btn-A
  int Button2State = digitalRead(6); //Read Btn-B
    // button 1 is held down
    if (Button1State == LOW && Button2State == HIGH) {
      // do things
      if (currMode != 1) { //if eeprom=2
      //  Serial.print("Switching to mode 1");
        EEPROM.update(0, 1); 
        delay(200);
      }
    }

    if (Button2State == LOW && Button1State == HIGH) {
      //if button 2 is held down
      if (currMode != 2) { //if eeprom=1
        EEPROM.update(0, 2);
        delay(200);
      }
    }
}


void loop() {


  if (EEPROM.read(0) == 1) {
    joy_mode();
  }
  if (EEPROM.read(0) == 2) {
    keyboard_mode();
  }
}

void keyboard_mode() {
  
  // read encoders
  knob1 =  (float)(encL.read());
  knob2 = (float)encR.read();
  
  if(knob1 != old_knob1)
  {
    // if there's a difference in encoder movement from last pass, move the mouse
    if(knob1 < old_knob1)
    {
      Mouse.move(-15, 0);
    }
    else
    {
      Mouse.move(15, 0);
    }
    
  // we count the difference in the encoders, but we must not go over arduino's int limit
    if(knob1 < -255)
    {
      encL.write(0);
      old_knob1 = 0;
    }
    else if (knob1 > 255)
    {
      encL.write(0);
      old_knob1 = 0;
    }
    else
    {
      old_knob1 = knob1;
    }
  }
  
  if(knob2 != old_knob2)
  {
    if(knob2 > old_knob2)
    {
      Mouse.move(0, 15);
    }
    else
    {
      Mouse.move(0, -15);
    }
    
    if(knob2 < -255)
    {
      encR.write(0);
      old_knob2 = 0;
    }
    else if(knob2 > 255)
    {
      encR.write(0);
      old_knob2 = 0;
    }
    else
    {
      old_knob2 = knob2;
    }
  }
  
  // read the buttons for low, if it's low, output a keyboard press  
  if(digitalRead(BT_A) == LOW)
  {
    Keyboard.press('a');
  }
  else
  {
    Keyboard.release('a');
  }
  
  if(digitalRead(BT_B) == LOW)
  {
    Keyboard.press('s');
  }
  else
  {
    Keyboard.release('s');
  }
  
  if(digitalRead(BT_C) == LOW)
  {
    Keyboard.press('d');
  }
  else
  {
    Keyboard.release('d');
  }
  
  if(digitalRead(BT_D) == LOW)
  {
    Keyboard.press('f');
  }
  else
  {
    Keyboard.release('f');
  }
  
  if(digitalRead(FX_L) == LOW)
  {
    Keyboard.press('z');
  }
  else
  {
    Keyboard.release('z');
  }
  
  if(digitalRead(FX_R) == LOW)
  {
    Keyboard.press('x');
  }
  else
  {
    Keyboard.release('x');
  }
  
  if(digitalRead(BT_ST) == LOW)
  {
    Keyboard.press(KEY_RETURN);
  }
  else
  {
    Keyboard.release(KEY_RETURN);
  }
  if(digitalRead(macropin) == LOW)
  {
    Keyboard.press(KEY_ESC);
  }
  else
  {
    Keyboard.release(KEY_ESC);
  }
}

void joy_mode() {
  // Read buttons
  for (int i = 0; i < buttonCount; i++) {
    if (digitalRead(buttonPins[i]) != HIGH) {
      report.buttons |= (uint16_t)1 << i;
    } else {
      report.buttons &= ~((uint16_t)1 << i);
    }
  }
  // Read Encoders
  report.xAxis = (uint8_t)((int32_t)(encL.read() / ENCODER_SENSITIVITY) % 256);
  report.yAxis = (uint8_t)((int32_t)(encR.read() / ENCODER_SENSITIVITY) % 256);
  // Light LEDs
  if (lightMode == 0) {
    lights(report.buttons);
  } else {
    lights(iivx_led);
  }

/* 
   ------- KEYBOARD ASCII CODES -------
  Top row keys: (use these if you don't have a numpad on your pc)
  48 - 0
  49 - 1
  50 - 2
  51 - 3
  52 - 4
  53 - 5
  54 - 6
  55 - 7
  56 - 8
  57 - 9
  45 - - (minus)(default service button)
  61 - = (equals)(default test button)
  8 - backspace
  
  
  220 NumPad /
  221 NumPad *
  222 NumPad -
  223 NumPad +
  224 NumPad ENTER
  225 NumPad 1 and End
  226 NumPad 2 and Down Arrow
  227 NumPad 3 and PageDn
  228 NumPad 4 and Left Arrow
  229 NumPad 5
  230 NumPad 6 and Right Arrow
  231 NumPad 7 and Home
  232 NumPad 8 and Up Arrow
  233 NumPad 9 and PageUp
  234 NumPad 0 and Insert
  235 NumPad . and Delete
  */

int macroButtonState = digitalRead(macropin);
int startButtonState = digitalRead(4);
int Button1State = digitalRead(5);
int Button2State = digitalRead(6);

// Press Start + Macro to send numpad_6
if (macroButtonState != old_macroButtonState) { //compare the button states (from StateChangeDetection Example)
  if (digitalRead(macropin) == LOW) { //if macro button is pressed
      if (digitalRead(4) == LOW){ // if start button is pressed
      Keyboard.begin();
      Keyboard.press(54); //top row 6
      delay(30);
      Keyboard.release(54);
      Keyboard.end();
       } else {
        Keyboard.end();
       } 
    }
  }

//Commented out some shortcuts, can't tell if causing input lag or not... maybe it's just my terrible sdvx skills
/*  // Press Start + Btn1 to send Numpad 9
if (Button1State != old_Button1State) { //compare the button states (from StateChangeDetection Example)
  if (digitalRead(5) == LOW) { //if Btn1 is pressed
      if (digitalRead(4) == LOW){ // if start button is pressed
      Keyboard.begin();
      Keyboard.press(233); //numpad_9
      delay(30);
      Keyboard.release(233);
      Keyboard.end();
       } else {
        Keyboard.end();
       } 
    }
  }

    // Press Start + Btn2 to send Numpad 3
if (Button2State != old_Button2State) { //compare the button states (from StateChangeDetection Example)
  if (digitalRead(6) == LOW) { //if Btn6 is pressed
      if (digitalRead(4) == LOW){ // if start button is pressed
      Keyboard.begin();
      Keyboard.press(227); //numpad_3
      delay(30);
      Keyboard.release(227);
      Keyboard.end();
       } else {
        Keyboard.end();
       } 
    }
  }
*/
// ****************Change your PIN here using the ascii table above**************************

if (digitalRead(4) == HIGH) {
 if (macroButtonState != old_macroButtonState) {  //compare the button states (from StateChangeDetection Example)
   if (digitalRead(macropin) == LOW) {  //if macro button is pressed
   Keyboard.begin();
   Keyboard.press(8); //backspace (triggers card scan)
   delay(300);
   Keyboard.release(8);
   delay(30);
   Keyboard.press(49);//toprow 1
   delay(30);
   Keyboard.release(49);
   delay(30);
   Keyboard.press(50); //toprow 2
   delay(30);
   Keyboard.release(50);
   delay(30);
   Keyboard.press(51); //toprow 3
   delay(30);
   Keyboard.release(51);
   delay(30);
   Keyboard.press(52); //toprow 4
   delay(30);
   Keyboard.release(52);
   Keyboard.end();// end keyboard
   } else {
  Keyboard.end();
      }
  }
}

//Press button 1-7 at the same time to send Numpad_Divide, which can be mapped to the Test button. 
int btnMash=0;

 if (digitalRead(4) == LOW) {
 btnMash = btnMash + 1;
 }
 if (digitalRead(5) == LOW) {
 btnMash = btnMash + 1;
 }
 if (digitalRead(6) == LOW) {
 btnMash = btnMash + 1;
 }
 if (digitalRead(7) == LOW) {
 btnMash = btnMash + 1;
 }
 if (digitalRead(8) == LOW) {
 btnMash = btnMash + 1;
 }
 if (digitalRead(9) == LOW) {
 btnMash = btnMash + 1;
 }
 if (digitalRead(10) == LOW) {
 btnMash = btnMash + 1;
 }
if (btnMash != old_btnMash) {
 if (btnMash == 7) {
  Keyboard.begin();
  Keyboard.press(61); //Top row equals
  delay(40);
  Keyboard.release(61);
  Keyboard.end();
 } else {
  Keyboard.end();
 }
}


old_macroButtonState = macroButtonState; // save the current state as the last state, for next time through the loop
old_startButtonState = startButtonState; // save the current state as the last state, for next time through the loop
old_btnMash = btnMash;
old_Button1State = Button1State;
old_Button2State = Button2State;

    
  
    
  // Detect lighting changes
  // if(digitalRead(sysPin)!=HIGH){
  //   if(report.buttons == 1){
  //     lightMode = 0;
  //  } else if (report.buttons == 4){
  //    lightMode = 1;
  //  } else if (report.buttons == 16){
  //    report.buttons = (uint16_t)1 << (buttonCount);
  //   } else if (report.buttons == 64){
  //     report.buttons = (uint16_t)1 << (buttonCount+1);
  //   }
  //  }
  // Send report and delay
  iivx.setState(&report);
  delayMicroseconds(REPORT_DELAY);
}
