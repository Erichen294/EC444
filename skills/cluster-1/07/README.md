# Code Readme

Author: Eric Chen

Date: 9/12/23

Summary:
I built a voltage divider to bring down the input voltage from 3.3V to below 2V. I chose two 1k resistors and set the ADC pin as GPIO 34 
which means that the ADC pin should read roughly 1.6 V or 1600 mV. The default attenuation factor is 1, but the pin was reading 1300 mV so I scaled it up to 2 and got about 1625 mV. 

Circuit:
![ADC](https://github.com/BU-EC444/Chen-Eric/assets/98416392/e5f5763c-21cc-4e19-8e64-180d7c313f23)

Screenshot of ADC pin reading voltage displaying on terminal:
<img width="150" alt="Screenshot 2023-09-12 at 3 18 32 PM" src="https://github.com/BU-EC444/Chen-Eric/assets/98416392/20f14912-3989-4806-b94b-dd0e919471de">


