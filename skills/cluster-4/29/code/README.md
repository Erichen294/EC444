Author: Eric Chen

Date: 11/7/23

Summary: The program reads the distance value measured by the ultrasonic sensor and then it takes the setpoint(50 cm) and it subtracts the measured distance. Then, it passes the difference to a function that turns on the corresponding LED depending on the value of the difference. If the difference is negative then the red led will turn on, if the difference is within 0 < x < 0.5 then the green led turns on, and if the difference is positive then the blue led will turn on. It repeats every 100 ms.
