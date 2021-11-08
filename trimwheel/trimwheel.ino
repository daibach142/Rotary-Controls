/* Trimwheel Version 1.0 */

/*
Copyright (C) 2021 Dave Attwood

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <https://www.gnu.org/licenses/>.

 */
 
 /****************************************************************
   FOR THE RASPBERRY PI PICO BOARD
   ===============================
   
   This program connects a rotary encoder (RE) on pins D2 and D3 to a
   Gamepad Joystick emulation, suitable for use in Flight Gear Flight
   Simulator. To complete the connection, a tailored configuration file
   'Arduino-RaspberryPi-Pico.xml' configures buttons 0 & 1 for
   elevator trim down and elevator trim up. This needs to be copied to
   the user's Input/Joysticks folder. It is important NOT TO use the FGFS
   built-in 'elevator trim' setting on these buttons, as it doesn't work
   properly (2020.3.8).
   
   Connect pins 4 (GP2) and 5 (GP3) to the outside legs of the RE.
   Connect one of the grounds (say pin 3) to the centre leg of the RE.
   Connect 100nF ceramic capacitors from the centre to each outer leg for
   debounce.
   
   Setup Arduino IDE
   =================
   
   In 'Tools->Board->Board Manager' search for 'pico', and install 
   'Arduino Mbed OS RP2040 Boards' and then select the board as 
   'Raspberry Pi Pico'.
   
   Plug the Pico into the USB connector holding the BOOTSEL button down.
   Release the button a few seconds after plugging in to USB.
   
   Compile and download the sketch. There is a BUG in thew download process, 
   and it will (probably) fail. To complete the load, find the location where
   the compiled files are (see last lines output prior to the orange load failed
   lines). In that location, there will be a file 'trimwheel.ino.elf.uf2'. Copy this
   to the (newly mounted) 'RPI-RP2' folder.
   
   The program will start running after the copy.
   
   Normal Running
   ==============
   
   
   Just plug the Pico into the USB (don't press the BOOTSEL button).
   
   Have fun.
   
   ****************************************************************/
 
//#include "MyPicoGamepad.h"
#include <PicoGamepad.h>
#include <Rotary.h>


//// Digital pins
//const uint8_t PINS[][2] = 	{
//								{2, 3}, 		// Elevator trim
//								{6, 7}, 		// HDG
//								{10, 11}		// CRS
//							};
//
//
//// Button numbers
//const uint8_t BUTTONS[][2] = 	{
//									{0, 1}, 	// Elevator trim
//									{2, 3}, 	// HDG
//									{4, 5}	// CRS
//								};
//	 
//
//
//Rotary encoders[] = {
//					Rotary(PINS[0][0], PINS[0][1]),		// Elevator trim
//					Rotary(PINS[1][0], PINS[1][1]),		// HDG 
//					Rotary(PINS[2][0], PINS[2][1])		// CRS
//					};

const uint8_t UP = 0;
const uint8_t DOWN = 1;

// Button Hold Time - milliseconds
// This seems to be a suitable time, 20 millis is not long enough
// Value chosen by experimentation
const unsigned long BUTTON_HOLD_TIME = 40;

struct Re {
  Rotary encoder;
  uint8_t pin_up, pin_down;
  uint8_t button_up, button_down;
};

Re encoders[3] = {
  { Rotary(2, 3),    2,  3, 0, 1 },     // Elevator Trim
  { Rotary(6, 7),    6,  7, 2, 3 },     // HDG
  { Rotary(10, 11), 10, 11, 4, 5 }      // CRS
};

const uint8_t ENTRIES = sizeof(encoders) / sizeof(Re);

PicoGamepad gamepad;

void setup() {
//  Serial.begin(115200);
//  while (!Serial) ;
  
//  Serial.println("Three Rotary Encoders in a row");
  
	for (int i = 0; i < ENTRIES; i++) {
    pinMode(encoders[i].pin_up, INPUT_PULLUP);
    pinMode(encoders[i].pin_down, INPUT_PULLUP);
	}
}

void loop() {
  uint8_t tick;

  // the 'official' arduino pi pico software does not handle interrupts
  // so the code simply polls the pins continuously
  // this doesn't really matter as the RE doesn't move that fast (more than
  // 25 ticks per second to lose a tick)
  // The processing rate is controlled by BUTTON_HOLD_TIME, which sets the time 
  // the RE pin appears pressed
  
  for (int i = 0; i < ENTRIES; i++) {
//    Serial.println(ENTRIES, DEC);
  	tick = encoders[i].encoder.process();
    if (tick == DIR_CW) {
//      Serial.println("DOWN");
      pressAButton(encoders[i].button_down);
    } else if (tick == DIR_CCW) {
//      Serial.println("UP");
      pressAButton(encoders[i].button_up);
    }
  }
}

void pressAButton(uint8_t num) {
  gamepad.SetButton(num, true);
  gamepad.send_update();        // set button ON
  delay(BUTTON_HOLD_TIME);      // give FGFS time to see it
  gamepad.SetButton(num, false);
  gamepad.send_update();        // set button OFF
}
