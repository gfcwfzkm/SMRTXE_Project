# SMRTXE_Project
Here is my main project folder where I work on new functions and test existing code. Contains graphics and ST7586 library (the graphics library modified to be used with the ST7586 and it's lack to read the display memory back).
Also, I've just added functions to use the keyboard and memory IC.

Files:
--- ST7586.c/.h
The hardware library to access and control the display (currently only the 4-wire serial mode supported)

--- glcd_library.c/.h
My basic graphics library I use for simple, monochrome/grayscale displays. Got updated with functions for "buffered" drawing.

--- hwSPI.c/.h
The SPI library I use to communicate with the display and the memory chip

--- mx25L1005.c/.h
The hardware library to access the memory chip.

--- sxe_driver.c/.h
Contains the init function to initialize all required pins and libraries, also includes the functions to read the keyboard.

--- sxe_pinout.h
All required pins, ports and direction registers are defined here.

--- main.c
Demo-program that shows a few functions. 

