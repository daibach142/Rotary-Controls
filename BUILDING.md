
BUILDING
========

Setup Arduino IDE
-----------------
   
In `Tools->Board->Board Manager` search for `pico`, and install  
`Arduino Mbed OS RP2040 Boards`  
and then select the board as  
`Raspberry Pi Pico`.
   
Plug the Pico into a USB connector.
   
The software is presented as an Arduino sketch, point your Arduino Sketchbook to the `trimwheel` or `CTS288` folder (the folder also contains a library for the RE and one for the PicoGamepad).  
 
Building
----------
 
Compile and download the sketch.

There is a BUG in the download process, 
and it will (probably) fail.

To complete the load, find the location where
the compiled files are (see last lines output prior to the orange load failed
lines). In that location, there will be a file `trimwheel.ino.elf.uf2` or `CTS288.ino.elf.uf2`. Copy this
to the (newly mounted) `RPI-RP2` folder.
   
   
---

License
=======

The Arduino code (such as it is!) is under GPL V3.  
The Libraries are under their individual licenses, as given in their respective _xxx_.cpp files.

