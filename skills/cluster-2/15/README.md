Author: Eric Chen

Date: 9/29/23

Summary: I created a circuit to read acceleration values from an accelerometer and also converts the values into tilt data. I finished functions that would write one byte to a register in the ADXL343 accelerometer, read one byte from a register, and read two bytes from a register. I also wrote a function that took the x, y, and z values and converted them to tilt data using equations. The esp32 and accelerometer communicate via the SCL and SDA lines. The ADXL343 is powered by the 3.3V output of the esp32 and grounded. 

Circuit:
![IMG_0082](https://github.com/BU-EC444/Chen-Eric/assets/98416392/7b414b11-393d-4896-a8cb-3c713cf2f7b4)

Video:
[Video Demonstration](https://drive.google.com/file/d/1tJP1HlMDBt2OdDj8loL2qz4bYGAakCuA/view?usp=drive_link)
