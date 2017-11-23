# OttoDIY_Vbot
Mix from OttoDIY opensource Project.  
http://ottodiy.com 
You can do it yours 
## About this Library
- I mixed some changes, by Arrange C++ files in 1 library so it is easy to install in Arduino. 
- Modify to use with Bluetooth HC06 in SoftwareSerial, so you can upload code without removing the HC06 Module
- Can Programming the Otto in Scratch Language using mBlock software (Both Arduino mode and Scratch Mode)

## Installation
Just download Zip File and add manually by: Arduino/Sketch/Include Library/Add Zip Library 
After installing Library, go to Files/Examples/otto and can upload the demo programms. 

## Hardware (Basically same OttoDIY Plus Matrix project)
- connect Servo Hip left to D2
- Connect Servo Foot Left to D4
- Connect Servo Hip right to D3
- COnnect Servo Foot Right to D5
- Buzzer to D13 (Active High)
- HC06Rx to Arduino Tx(D7)
- HO06Tx to Arduino Rx(D6)
- SRF04 Trigger to D8
- SRF04 Echo    To D9
- Max7219   DATA    12   //for LED matrix Mouth
- Max7219   CS    10
- Max7219  CLK    11
- Sound Sensor  to analog A6, 
- Light sensor Left to analog A1
- Light Sensor Right to analog A0
- Button 1    to A2 in pull up input (low active: connect A2 to Gnd) 
- Button 2    to A3 in pull up input(low active: connect A3 to Gnd)
## Scratch Programming in mBlock 
Install mBlock Extension for OttoDIY: https://github.com/stembotvn/OttoDIY_mBlock_Extension
## Play with Zowi App (Android only) via Bluetooth
- Zowi App is a great app for Otto learning especially suitable for +5 years old kids
- Zowi App is develop for Zowi Robot (an perfect mixing with Otto) from BQ: https://www.bq.com/en/zowi
- Install App on Android Phone (Search Zowi app in CHPlay): https://play.google.com/store/apps/details?id=com.bq.zowi&hl=en
- Upload firmware on Arduino IDE: "Examples/otto/HC06_BT_config" to configure the Bluetooth HC06 module for compatible with Zowi App https://github.com/stembotvn/OttoDIY_Vbot/tree/master/examples/HC06_BT_config
- Upload firmware on Arduino IDE: "Examples/otto/Otto_BT_ZowiAPP.ino  for enjoy Zowi App with your Otto. 
- For experience with Zowi App, refer here: http://zowi.bq.com/en/parents-teachers/
- Pairing the Bluetooth Module with Android device, the name of module will be "Zowi", Passcode is 1234
- in Zowi App, the App will Search, detect and connect the Robot.  










