Author: Eric Chen

Date: 11/7/23

Summary: I wrote a program that initializes the lidar configuration settings and follows the paradigm given by the manufacturer. It repeatedly gets the distance measurement from the lidar every 100 ms.

1) Write to register 0x00 with value 0x04 (initiates and begins acquisition)
2) Read register 0x01
3) Repeat read of 0x01 until bit 0 (LSB) goes low
4) Read two bytes from 0x10 (low byte 0x10 then high byte 0x11) to get 16-bit distance in cm

Video:
[Video Demonstration](https://drive.google.com/file/d/1VqrHLcX8-OCw_5m-2_KncuCZNWrj7bAS/view?usp=drive_link)
