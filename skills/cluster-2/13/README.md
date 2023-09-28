Author: Eric Chen

Date: 9/28/23

Summary:
I created a module that receives a string (up to 8 characters) and display it on the i2c. If the string is longer than 4 characters, then the screen will scroll. I implemented it by using a sliding window if it was more than 4 characters. The screen will only display the string for a set period of time before turning off all the 14-segment displays and waiting for the next input.

Circuit:
![IMG_0041](https://github.com/BU-EC444/Chen-Eric/assets/98416392/25964b0b-a50f-47e6-bd14-f7a94acf69df)

[Video Demonstration](https://drive.google.com/file/d/1YMktzW2lTQ-Xqvssah-YZ8S8kE96TO5N/view?usp=drive_link)
