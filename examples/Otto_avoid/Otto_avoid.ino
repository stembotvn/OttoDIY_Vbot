//----------------------------------------------------------------
//-- Otto Demo using Ultrasonic Sensor for avoid obstacle.
//-- Mixed from OttoDIY project
//-- Example: Avoid obstacles, using SRF04 Sonar Sensor for measure Range in front, if there is obstacle with distance < 15 cm, stop and turn Right 2 step then keep going Forwalk

//-----------------------------------------------------------------
//Chương trình mẫu thực hiện yêu cầu tránh vật cản, khi robot đang đi tiến về trước và phát hiện vật cản ở khoảng cách nhỏ hơn 15 Cm thì dừng lại và rẽ hướng khác. 
//-----------------------------------------------------------------


#include "Otto.h"
//-- First step: Make sure the pins for servos are in the right position
/*
             --------------- 
            |     O   O     |
            |---------------|
HIP_R D3==> |               | <== HIP_L -> D2
             --------------- 
               ||     ||
FOOT_R D5==> -----   ------  <== FOOT_L -> D4
            |-----   ------|
*/

Otto Vbot;  //my name is Vbot! Hello World!
//---------------------------------------------------------
/*
#define HIP_L   2        // connect Servo Hip left to D2
#define FOOT_L  4        // Connect Servo Foot Left to D4
#define HIP_R   3        // Connect Servo Hip right to D3
#define FOOT_R  5        // COnnect Servo Foot Right to D5
*/
///////////////////////////////////////////////////////////////////
//-- Global Variables -------------------------------------------//
///////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------
///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){
  //Set the servo pins
  Vbot.init(HIP_L, HIP_R, FOOT_L, FOOT_R, true, PIN_NoiseSensor, PIN_Buzzer,PIN_Trigger, PIN_Echo);  
  // [No calibrate home position] [ SRF04 Echo to D9, Trigger to D8 ] [ Buzzer to D10(High level active)]

  Vbot.sing(S_connection); //Vbot wake up!
  Vbot.home();
  delay(50);
  Vbot.sing(S_happy); // a happy Vbot :)
}
///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {
  if (Vbot.getDistance()<15) { 
               Vbot.sing(S_surprise); 
               Vbot.playGesture(OttoFretful); 
               Vbot.sing(S_fart3); 
               Vbot.walk(2,1300,-1); 
               Vbot.turn(2,1000,-1);                
             delay(50); 
           
             }        
         else{ 
            Vbot.walk(1,1000,1); 
           
        }           
  }  
///////////////////////////////////////////////////////////////////
