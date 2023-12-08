Author: Eric Chen
Date: 12/8/23

Summary: The FSM represents the possible states that a seat could be in. When starting in the not occupied and not reserved state, reserving the seat will cause the state to transition to not occupied and reserved. From there if 5 minutes have elapsed (reservation expired), then the state will transition back to not occupied and not reserved state. From the unoccupied and reserved state, if heat is detected then it will transition to the occupied state. From the occupied state, if there is no heat then it will transition to unoccupied and unreserved. Also, from the unoccupied and unreserved state if heat is detected then it will transition to occupied.

<img width="927" alt="FSM" src="https://github.com/BU-EC444/Chen-Eric/assets/98416392/e60eefde-0ead-417e-aff3-bf23ef4a062a">

For the block diagram, we have 3 ESP-32s each equipped with an accelerometer, a temperature sensor, and a multicolor LED to display the status of the seat. Green means that the seat (ESP-32) is not taken and RED means that the seat is reserved / taken. These ESPs will subscribe and publish to a message broker that is hosted on a Pi. It will publish information about the seat occupancy based on the temperature data and accelerometer data. This information is then published to an HTML for viewing on a website. The HTML is also used to publish seat reservations, which can be made on the website. This information is published to the broker, which then publishes the information to the ESPs to display the LEDs correctly. This HTML is connected to the router using DDNS so that it can be accessed from outside networks.
<img width="927" alt="Diagram" src="https://github.com/BU-EC444/Chen-Eric/assets/98416392/ae34b0c8-cd2f-465e-bc30-3aafc8b4a3fe">

The reservation information is sent to the ESP32 seats. The seats can be reserved by the user by using the website to publish the information. The ESP-32 receives this information and updates the LEDs to reflect the change. Green is available, and Red is occupied/reserved. If there is a thermister reading, then it must check if the seat is reserved, then the seat is now occupied. If the seat is unreserved and detects temperature readings, then the seat is now occupied. The website will reflect if the seat is occupied.
<img width="624" alt="Flowchart" src="https://github.com/BU-EC444/Chen-Eric/assets/98416392/9ae44354-2465-4c00-a20c-4c23a91f6d22">


