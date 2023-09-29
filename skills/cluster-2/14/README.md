Author: Eric Chen

Date: 9/28/23

Summary:
I wrote a program that will begin counting when the button is pressed. Any additional presses of the button will result in the timer restarting at 0. The timer is displayed on the i2c screen. The counting up is controlled by an interrupt-driven timer function to ensure that it counts up at an accurate rate rather than delay 1000ms. I connected the i2c to the SDA and SCL pins on the board and connected a 10kohm resistor to the button. The program will initialize everything and begin counting when the button is pressed the first time. 

Circuit:
![IMG_0080](https://github.com/BU-EC444/Chen-Eric/assets/98416392/02c81f0f-0ae3-4ff7-b5e6-0b515219bd79)

Video:
[Video Demonstration](https://drive.google.com/file/d/1eM2tfhc-1hsJYRXY6g7TtiITpPdC9nQn/view?usp=drive_link)
