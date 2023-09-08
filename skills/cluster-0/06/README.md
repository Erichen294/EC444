# Code Readme

Please describe in this readme what is in your code folder and
subfolders. Make it easy for us to navigate this space.

Also
- Please provide your name and date in the comment header for any
code you submit
- Indicate within comment lines attributrion for any code you
have adopted from elsewhere
- Indicate, in code comments any use of AI for producing your code

#  Use GPIO to Drive LEDs

Author: Eric Chen

Date: 2023-09-08


### Summary
This circuit has three LEDs each of whom is driven by a GPIO pin on the ESP32 board. The red LED represents the least significant digit in my binary number and the yellow LED represents the most significant digit. Since I am using three LEDs, I will be counting from 0 to 7 then back down from 7 to 0. Each LED has a 330 ohm resistor connected to it to prevent the LEDs from damage. Each of the LEDs are grounded. The code simply initializes each LED and I created a function that lights up the correct LEDs to display the current binary number. I have a global count variable that keeps track of the current number being displayed as well as a global boolean to represent whether I'm counting up or down. The while loop will ensure that the circuit keeps on going forever as long as there is a power source. 

### Sketches/Diagrams
[Circuit Diagram](../../../../../../Downloads/IMG_0925.HEIC)
Here is a photo of my circuit diagram.

### Modules, Tools, Source Used Including Attribution


### Supporting Artifacts
[Counting in Binary](../../../../../../Downloads/IMG_0924.MOV)
Here is a working video of my binary counter.