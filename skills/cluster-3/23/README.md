Author: Eric Chen
Date: 10/23/23
Summary: I wired up my thermistor and photocell and sent the data into the serial port in a [temperature photocell] format every line and every second. The node.js program reads the data from the serial port and uses a socket to communicate with the index.html and receive data. Everytime the socket receives the message, it'll push the new data point to the appropriate data array and update the plot on the canvasJS chart. The x axis is time in seconds starting from 0 and increments of 1 second. The y axis is temperature or photocell reading depending on which graph you're looking at.

Video Demonstration:
[Video](https://drive.google.com/file/d/1xDoJtMyUL5HFB2SLJ8ZBvpOdnPYazywi/view?usp=drive_link)
