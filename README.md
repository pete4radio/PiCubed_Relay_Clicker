# PiCubed_Relay_Clicker
Clicks the relay (GPIO 44) on the PiCubed board.

Prerequisites:  
https://github.com/pete4radio/boards
RPi Extension

Build instructions
1.  If necessary, Import into the RPi Extension
2.  Connect the PiCubed board via USB
3.  In the lower right of the VSCode window, select the correct Board (probably SAMWISE)
4.  Click "Run Program (USB)"
5.  If it reports not finding ninja.build file, rm -rf build; cmake -B build -G Ninja
