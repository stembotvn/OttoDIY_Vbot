# OttoDIY_Vbot
Mix from OttoDIY opensource Project.  
http://ottodiy.com 
You can do it yours 
## About this Library
- I mixed with some changes, by Arrange all C++ files in 1 library so it is easy to install in Arduino. 
- Modify to use with Bluetooth HC06 in SoftwareSerial, so you can upload code without removing the HC06 Module 
- Can Programming the Otto in Scratch Language using mBlock software (Both Arduino mode and Scratch Mode)

## Installation
Just download Zip File and add Library manually by: Arduino/Sketch/Include Library/Add Zip Library 
After installing Library, go to Files/Examples/otto and can upload the demo programms. 

## Hardware (Basically same OttoDIY Plus Matrix project)
<img src="https://github.com/stembotvn/OttoDIY_mBlock_Extension/blob/master/media/connection.png" width="500" align="center">

 |  Component             | Arduino Pin |
 | --- | --- |
 |  Servo Hip left        | D2          |
 |  Servo Foot Left       | D4          |
 |  Servo Hip right       | D3          |
 |  Servo Foot right| D5|
 |  Buzzer          | D13|
 |Bluetooth HC06 Rx | D7|
 |Bluetooth HC06 Tx | D6|
 |SRF04 Trigger     | D8|
 |SRF04 Echo        | D9|
 |Max7219 LedMatrix Data | D10|
 |Max7219 LedMatrix CS   | D11|
 |Max7219 LedMatrix Clk  | D12|
 |Analog Sound Sensor Module|A6|
 |Button 1                  |A2|
 |Button 2                  |A3|
 |Light sensor Module (RIGHT) Analog output Pin|A0|
 |Light Sensor Module (LEFT) Analog output Pin|A1|
## Scratch Programming in mBlock 
 - Download mBlock software: http://www.mblock.cc/
 - Install mBlock Extension for OttoDIY: https://github.com/stembotvn/OttoDIY_mBlock_Extension
 - See demo video for installation: https://www.youtube.com/watch?v=lPDUmW9uHto&t=59s
## Play with Zowi App (Android only) via Bluetooth
- Zowi App is a great app for Otto learning especially suitable for +5 years old kids
- Zowi App is develop for Zowi Robot (an perfect mixing with Otto) from BQ: https://www.bq.com/en/zowi
- Install App on Android Phone (Search Zowi app in CHPlay): https://play.google.com/store/apps/details?id=com.bq.zowi&hl=en
- Upload firmware on Arduino IDE: "Examples/otto/HC06_BT_config" to configure the Bluetooth HC06 module for compatible with Zowi App https://github.com/stembotvn/OttoDIY_Vbot/tree/master/examples/HC06_BT_config
- Upload firmware on Arduino IDE: "Examples/otto/Otto_BT_ZowiAPP.ino  for enjoy Zowi App with your Otto. 
- For experience with Zowi App, refer here: http://zowi.bq.com/en/parents-teachers/
- Pairing the Bluetooth Module with Android device, the name of module will be "Zowi", Passcode is 1234
- in Zowi App, the App will Search, detect and connect the Robot.  










