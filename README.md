Rotary Encoders for FGFS
==================

This uses (up to) 3 Rotary Encoders (RE) and a Raspberry Pi Pico (Pico) to connect the REs to FlightGear Flight Simulator as a Pitch Trimwheel and HDG and CRS adjusters.

A configuration file is provided for FGFS, and no additional configuration is required.

The software will work fine with any or all the REs connected.


The file `trimwheel.ino` is for standard 4-bit gray code encoders, and the file `CTS288.ino` for CTS 288 encoders with 2-bit binary as provided by Leo Bodnar and others
