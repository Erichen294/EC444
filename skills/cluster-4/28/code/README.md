Author: Eric Chen

Date: 11/7/23

Summary: The program sets up the configuration of the lidar. Then, it writes the value 0x04 to register 0, continuously reads from register 1 until the LSB is low, and then reads 2 bytes from register 10. It prints the distance and repeats every 100 ms.
