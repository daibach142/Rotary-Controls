
Installation
============

Materials
---------

 1. Your Trimwheel with its Rotary Encoder (RE)
 2. (Optional) 2 off 100nF ceramic capacitors.
 3. Rapsberry Pi Pico.
 4. Hook-up wire.
 
Load Software into Pico
-----------------------

 1. Open the Pico in Bootloader mode (hold the BOOTSEL button down when plugging into the USB cable, then release button).
 2. The Pico appears as a memory device, `RPI-RP2`.
 3. Drag and drop the firmware file `trimwheel.ino.elf.uf2` onto `RPI-RP2`.
 4. The Pico can now be disconnected from USB.

Electrical
----------
 
 1. Connect the RE being used as a trimwheel to locations GP2 and GP3 on the Pico. Depending on the RE being used, GP2 connects to the input for 'pitch down' (the direction can be easily swapped in the FGFS configuration file). 
 2. Connect the Common or Ground pin on the RE to any GND location on the Pico.
 3. Connect a 100nF ceramic capacitor between each of the RE data pins to the Common or Ground pin. (optional)
 
Configure FGFS
--------------

**On Linux** Copy `Arduino-RaspberryPi-Pico.xml` to `~/.fgfs/Input/Joysticks`.  
**On Windows** Copy `RaspberryPi-Pico.xml` to `%UserProfile%\Appdata\Roaming\flightgear.org\Input\Joysticks`.  

Both these files have identical contents.

Preferably make the copied file Read-Only to avoid inadvertent overwriting.




  
