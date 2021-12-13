/* Encoders for FGFS Trimwqheel, HDG and CRS Version 1.1 */
/* This version for Leo Bodnar Rotary Encoders type CTS288
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

  ****************************************************************
    Now modified for two extra REs for HSI HDG and CRS control
    on pins GP6 & GP7, (HDG) and pins GP10 & GP11 (CRS)

    Software runs fine with any or all of the REs attached
  ****************************************************************

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
  lines). In that location, there will be a file 'CTS288.ino.elf.uf2'. Copy this
  to the (newly mounted) 'RPI-RP2' folder.

  The program will start running after the copy.

  Normal Running
  ==============


  Just plug the Pico into the USB (don't press the BOOTSEL button).

  Have fun.

  ****************************************************************/


#include <PicoGamepad.h>

/*

  Code table for 2 bit binary

  V   0 1 3 2 0
  B   0 0 1 1 0
  A   0 1 1 0 0
  Pos 1 2 3 4 5 ...

*/
PicoGamepad gamepad;

#define BUTTON_HOLD_TIME 40

const uint8_t CW = 1;
const uint8_t CCW = 2;

struct CTS288 {
  uint8_t state;
  uint8_t pinUp,  pinDown;
  uint8_t butUp,  butDown;
};

CTS288 encoders[] = {
  {.state = 0, .pinUp = 2, .pinDown = 3, .butUp = 0, .butDown = 1},
  {.state = 0, .pinUp = 6, .pinDown = 7, .butUp = 2, .butDown = 3},
  {.state = 0, .pinUp = 10, .pinDown = 11, .butUp = 4, .butDown = 5}
};

const uint8_t ENCODERS = sizeof(encoders) / sizeof(CTS288);

static uint8_t codeTable[][4] = {
  {0, CW, CCW, 0},
  {CCW, 0, 0, CW},
  {CW, 0, 0, CCW},
  {0, CCW, CW, 0},
};



uint8_t process(uint8_t *state, uint8_t value) {

  uint8_t nextValue;

  nextValue = codeTable[*state][value];
  *state = value;
  return nextValue;

}

void setup() {
  uint8_t temp;

  for (uint8_t i = 0; i < ENCODERS; i++) {
    pinMode(encoders[i].pinUp, INPUT_PULLUP);
    pinMode(encoders[i].pinDown, INPUT_PULLUP);
    encoders[i].state = (digitalRead(encoders[i].pinDown) << 1) | digitalRead(encoders[i].pinUp);
  }
}

void loop() {
  for (uint8_t i = 0; i < ENCODERS; i++) {
    uint8_t value = (digitalRead(encoders[i].pinDown) << 1) | digitalRead(encoders[i].pinUp);
    if (value != encoders[i].state) {
      uint8_t res = process(&encoders[i].state, value);
      if (res == CW) {
        pressAButton(encoders[i].butUp);
      }
      else if (res = CCW) {
        pressAButton(encoders[i].butDown);
      }
    }
  }
}

void pressAButton(uint8_t num) {
  gamepad.SetButton(num, true);
  gamepad.send_update();        // set button ON
  delay(BUTTON_HOLD_TIME);      // give FGFS time to see it
  gamepad.SetButton(num, false);
  gamepad.send_update();        // set button OFF
  delay(BUTTON_HOLD_TIME);      // give FGFS time to see it
}
