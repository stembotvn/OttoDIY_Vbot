//----------------------------------------------------------------
//Firmware using SoftwareSerial for Bluetooth Interface 
//Otto firmware for compatible with Bluetooth Zowi App (Android Only) and Scratch via Bluetooth
//Mixed from OttoDIY and Zowi 
//Comatible with Zowi APP, a very fun Smartphone app for programming Zowi Ottobot
//Link Android APP: https://play.google.com/store/apps/details?id=com.bq.
// This Firmware also compatible with Mblock SoftWare in Scratch Mode via Bluetooth(using online programming in Scratch Language, must always connect to PC to programming and run)
//Suitable for kids from 5-15 years old - Enjoy 
//Hardware: Arduino Nano 
//Buzzer: D13
//SRF04: Echo D9
//       Trigger D8
//Servo: D2,D3,D4,D5 
//HC06 (BT_Tx)   Tx  ----> Rx (D6) Arduino 
//HC06  (BT_Rx)  Rx  ----> Tx (D7) Arduino
//Sound  A6
//
//-----------------------------------------------------------------

//-- Otto Library
#include "Otto.h"
#include "SoftSerialCommand.h"


SoftwareSerial BT = SoftwareSerial(BT_Tx,BT_Rx); // Set Up Bluetooth connection on SoftwareSerial (Tx is D7, Rx is D6 in Arduino Side)  
SoftSerialCommand SCmd(BT); //The SerialCommand object

//---------------------------------------------------------
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

Otto Ottobot;  //my name is Ottobot! Hello World!
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
const char programID[]="ZOWI_BASE_v2";

const char name_fac='$'; //Factory name
const char name_fir='#'; 

//-- Movement parameters
int T=1000;              //Initial duration of movement -> show the speed of movement
int moveId=0;            //type of movement
int moveSize=30;         //Size of movement (amplitude of movement)

unsigned long previousMillis=0;

int randomDance=0;
int randomSteps=0;

bool obstacleDetected = false;


///////////////////////////////////////////////////////////////////
//-- Setup ------------------------------------------------------//
///////////////////////////////////////////////////////////////////
void setup(){

  //Serial communication initialization
  BT.begin(9600);  //init for Bluetooth HC-06 interface via Software Serial 
  Serial.begin(9600); //init for Serial interface for Debug data in PC 
 // Ottobot.setTrims(0,0,0,0);
 // Ottobot.saveTrimsOnEEPROM(); // NOTE: comment out this line if you are using calibration function from Zowi App
  Ottobot.init(HIP_L, HIP_R, FOOT_L, FOOT_R, true, PIN_NoiseSensor, PIN_Buzzer,PIN_Trigger, PIN_Echo);  
  

  randomSeed(analogRead(A6));

  //Setup callbacks for SerialCommand commands 
  SCmd.addCommand("S", receiveStop);      //  sendAck & sendFinalAck
  SCmd.addCommand("L", receiveLED);       //  sendAck & sendFinalAck
  SCmd.addCommand("T", recieveBuzzer);    //  sendAck & sendFinalAck
  SCmd.addCommand("M", receiveMovement);  //  sendAck & sendFinalAck
  SCmd.addCommand("H", receiveGesture);   //  sendAck & sendFinalAck
  SCmd.addCommand("K", receiveSing);      //  sendAck & sendFinalAck
  SCmd.addCommand("C", receiveTrims);     //  sendAck & sendFinalAck
  SCmd.addCommand("G", receiveServo);     //  sendAck & sendFinalAck
  SCmd.addCommand("R", receiveName);      //  sendAck & sendFinalAck
  SCmd.addCommand("E", requestName);
  SCmd.addCommand("D", requestDistance);
  SCmd.addCommand("N", requestNoise);
  SCmd.addCommand("B", requestBattery);
  SCmd.addCommand("I", requestProgramId);
  SCmd.addDefaultHandler(receiveStop);

  //Ottobot wake up!
  Ottobot.sing(S_connection);
  Ottobot.home();
  delay(50);
  Serial.println("Zowi Started");
  
  //Send Ottobot name, programID & battery level.
  requestName();
  delay(50);
  requestProgramId();
  delay(50);
  requestBattery();
  Serial.println("Sent Name");
  
  //Checking battery
  LowBatteryAlarm();
  Ottobot.playGesture(OttoHappy);
   Ottobot.sing(S_happy);
    delay(200);
 

 previousMillis = millis();

}

///////////////////////////////////////////////////////////////////
//-- Principal Loop ---------------------------------------------//
///////////////////////////////////////////////////////////////////
void loop() {

  if (BT.available() > 0) {
   // Ottobot.putMouth(happyOpen);
  //  Serial.print(BT.read());
    SCmd.readSerial();
  
    //If Ottobot is moving yet
    if (Ottobot.getRestState()==false) {
      move(moveId);
    }
  } 
}

///////////////////////////////////////////////////////////////////
//-- Functions --------------------------------------------------//
///////////////////////////////////////////////////////////////////

//-- Function to read distance sensor & to actualize obstacleDetected variable
void obstacleDetector(){

   int distance = Ottobot.getDistance();

        if(distance<15){
          obstacleDetected = true;
        }else{
          obstacleDetected = false;
        }
}


//-- Function to receive Stop command.
void receiveStop(){

    sendAck();
    Ottobot.home();
    sendFinalAck();

}




//-- Function to receive buzzer commands
void recieveBuzzer(){
  
    //sendAck & stop if necessary
    sendAck();
    Ottobot.home(); 

    bool error = false; 
    int frec;
    int duration; 
    char *arg; 
    
    arg = SCmd.next(); 
    if (arg != NULL) { frec=atoi(arg); }    // Converts a char string to an integer   
    else {error=true;}
    
    arg = SCmd.next(); 
    if (arg != NULL) { duration=atoi(arg); } // Converts a char string to an integer  
    else {error=true;}

    if(error==true){


      delay(2000);
 

    }else{ 

      Ottobot._tone(frec, duration, 1);   
    }

    sendFinalAck();

}


//-- Function to receive TRims commands
void receiveTrims(){  

    //sendAck & stop if necessary
    sendAck();
    Ottobot.home(); 

    int trim_YL,trim_YR,trim_RL,trim_RR;

    //Definition of Servo Bluetooth command
    //C trim_YL trim_YR trim_RL trim_RR
    //Examples of receiveTrims Bluetooth commands
    //C 20 0 -8 3
    bool error = false;
    char *arg;
    arg=SCmd.next();
    if (arg != NULL) { trim_YL=atoi(arg); }    // Converts a char string to an integer   
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { trim_YR=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { trim_RL=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { trim_RR=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}
    
    if(error==true){

      delay(2000);

    }else{ //Save it on EEPROM
      Ottobot.setTrims(trim_YL, trim_YR, trim_RL, trim_RR);
      Ottobot.saveTrimsOnEEPROM(); 
      Serial.println("Save offset to EEROM");
    } 

    sendFinalAck();   

}
 

//-- Function to receive Servo commands
void receiveServo(){  

    sendAck(); 
    moveId = 30;

    //Definition of Servo Bluetooth command
    //G  servo_YL servo_YR servo_RL servo_RR 
    //Example of receiveServo Bluetooth commands
    //G 90 85 96 78 
    bool error = false;
    char *arg;
    int servo_YL,servo_YR,servo_RL,servo_RR;

    arg=SCmd.next();
    if (arg != NULL) { servo_YL=atoi(arg); }    // Converts a char string to an integer   
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { servo_YR=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { servo_RL=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}

    arg = SCmd.next(); 
    if (arg != NULL) { servo_RR=atoi(arg); }    // Converts a char string to an integer  
    else {error=true;}
    
    if(error==true){

      delay(2000);

    }else{ //Update Servo:

      int servoPos[4]={servo_YL, servo_YR, servo_RL, servo_RR}; 
      Ottobot._moveServos(200, servoPos);   //Move 200ms
      
    }
    Ottobot.detachServos();
    Ottobot.setRestState(true);
    sendFinalAck();


}


//-- Function to receive movement commands
void receiveMovement(){

    sendAck();
    Serial.print("Move Command: ");
    if (Ottobot.getRestState()==true){
        Ottobot.setRestState(false);
    }

    //Definition of Movement Bluetooth commands
    //M  MoveID  T   MoveSize  
    char *arg; 
    arg = SCmd.next(); 
    if (arg != NULL) {moveId=atoi(arg); Serial.println(moveId); Serial.print(" ");}
    else{
      Ottobot.putMouth(xMouth);
      delay(2000);
      Ottobot.clearMouth();
      moveId=0; //stop
    }
    
    arg = SCmd.next(); 
    if (arg != NULL) {T=atoi(arg); Serial.println(T); Serial.print(" ");}
    else{
      T=1000;
    }

    arg = SCmd.next(); 
    if (arg != NULL) {moveSize=atoi(arg); Serial.println(moveSize); Serial.print(" ");}
    else{
      moveSize =30;
    }
}


//-- Function to execute the right movement according the movement command received.
void move(int moveId){

  bool manualMode = false;

  switch (moveId) {
    case 0:
      Ottobot.home();
      break;
    case 1: //M 1 1000 
      Ottobot.walk(1,T,1);
      break;
    case 2: //M 2 1000 
      Ottobot.walk(1,T,-1);
      break;
    case 3: //M 3 1000 
      Ottobot.turn(1,T,1);
      break;
    case 4: //M 4 1000 
      Ottobot.turn(1,T,-1);
      break;
    case 5: //M 5 1000 30 
      Ottobot.updown(1,T,30);
      break;
    case 6: //M 6 1000 30
	  moveSize =30;
      Ottobot.moonwalker(1,T,moveSize,1);
      break;
    case 7: //M 7 1000 30
      moveSize =30;
      Ottobot.moonwalker(1,T,moveSize,-1);
      break;
    case 8: //M 8 1000 30
	  moveSize =30;
      Ottobot.swing(1,T,moveSize);
      break;
    case 9: //M 9 1000 30 
	  moveSize =30;
      Ottobot.crusaito(1,T,moveSize,1);
      break;
    case 10: //M 10 1000 30
      moveSize =30;	
      Ottobot.crusaito(1,T,moveSize,-1);
      break;
    case 11: //M 11 1000 
      Ottobot.jump(1,T);
      break;
    case 12: //M 12 1000 30
	  moveSize =30;
      Ottobot.flapping(1,T,moveSize,1);
      break;
    case 13: //M 13 1000 30
	  moveSize =30;
      Ottobot.flapping(1,T,moveSize,-1);
      break;
    case 14: //M 14 1000 20
	  moveSize = 20;
      Ottobot.tiptoeSwing(1,T,moveSize);
      break;
    case 15: //M 15 500 
      Ottobot.bend(1,T,1);
      break;
    case 16: //M 16 500 
      Ottobot.bend(1,T,-1);
      break;
    case 17: //M 17 500 
      Ottobot.shakeLeg(1,T,1);
      break;
    case 18: //M 18 500 
      Ottobot.shakeLeg(1,T,-1);
      break;
    case 19: //M 19 500 20
	  moveSize =30;
      Ottobot.jitter(1,T,moveSize);
      break;
    case 20: //M 20 500 15
	  moveSize =30;
      Ottobot.ascendingTurn(1,T,moveSize);
      break;
    default:
        manualMode = true;
      break;
  }

  if(!manualMode){
    sendFinalAck();
  }
       
}


//-- Function to receive gesture commands
void receiveGesture(){

    //sendAck & stop if necessary
    sendAck();
    Ottobot.home(); 

    //Definition of Gesture Bluetooth commands
    //H  GestureID  
    int gesture = 0;
    char *arg; 
    arg = SCmd.next(); 
    if (arg != NULL) {gesture=atoi(arg);}
    else 
    {
      Ottobot.putMouth(xMouth);
      delay(2000);
      Ottobot.clearMouth();
    }

    switch (gesture) {
      case 1: //H 1 
        Ottobot.playGesture(OttoHappy);
        break;
      case 2: //H 2 
        Ottobot.playGesture(OttoSuperHappy);
        break;
      case 3: //H 3 
        Ottobot.playGesture(OttoSad);
        break;
      case 4: //H 4 
        Ottobot.playGesture(OttoSleeping);
        break;
      case 5: //H 5  
        Ottobot.playGesture(OttoFart);
        break;
      case 6: //H 6 
        Ottobot.playGesture(OttoConfused);
        break;
      case 7: //H 7 
        Ottobot.playGesture(OttoLove);
        break;
      case 8: //H 8 
        Ottobot.playGesture(OttoAngry);
        break;
      case 9: //H 9  
        Ottobot.playGesture(OttoFretful);
        break;
      case 10: //H 10
        Ottobot.playGesture(OttoMagic);
        break;  
      case 11: //H 11
        Ottobot.playGesture(OttoWave);
        break;   
      case 12: //H 12
        Ottobot.playGesture(OttoVictory);
        break; 
      case 13: //H 13
        Ottobot.playGesture(OttoFail);
        break;         
      default:
        break;
    }

    sendFinalAck();
}

//-- Function to receive sing commands
void receiveSing(){

    //sendAck & stop if necessary
    sendAck();
    Ottobot.home(); 

    //Definition of Sing Bluetooth commands
    //K  SingID    
    int sing = 0;
    char *arg; 
    arg = SCmd.next(); 
    if (arg != NULL) {sing=atoi(arg);}
    else 
    {
      delay(2000);

    }

    switch (sing) {
      case 1: //K 1 
        Ottobot.sing(S_connection);
        break;
      case 2: //K 2 
        Ottobot.sing(S_disconnection);
        break;
      case 3: //K 3 
        Ottobot.sing(S_surprise);
        break;
      case 4: //K 4 
        Ottobot.sing(S_OhOoh);
        break;
      case 5: //K 5  
        Ottobot.sing(S_OhOoh2);
        break;
      case 6: //K 6 
        Ottobot.sing(S_cuddly);
        break;
      case 7: //K 7 
        Ottobot.sing(S_sleeping);
        break;
      case 8: //K 8 
        Ottobot.sing(S_happy);
        break;
      case 9: //K 9  
        Ottobot.sing(S_superHappy);
        break;
      case 10: //K 10
        Ottobot.sing(S_happy_short);
        break;  
      case 11: //K 11
        Ottobot.sing(S_sad);
        break;   
      case 12: //K 12
        Ottobot.sing(S_confused);
        break; 
      case 13: //K 13
        Ottobot.sing(S_fart1);
        break;
      case 14: //K 14
        Ottobot.sing(S_fart2);
        break;
      case 15: //K 15
        Ottobot.sing(S_fart3);
        break;    
      case 16: //K 16
        Ottobot.sing(S_mode1);
        break; 
      case 17: //K 17
        Ottobot.sing(S_mode2);
        break; 
      case 18: //K 18
        Ottobot.sing(S_mode3);
        break;   
      case 19: //K 19
        Ottobot.sing(S_buttonPushed);
        break;                      
      default:
        break;
    }

    sendFinalAck();
}


//-- Function to receive Name command
void receiveName(){

    //sendAck & stop if necessary
    sendAck();
    Ottobot.home(); 
    Ottobot.sing(S_confused); //deny to receive command changing the name. 
    /*
    char newOttobotName[11] = "";  //Variable to store data read from Serial.
    int eeAddress = 5;          //Location we want the data to be in EEPROM.
    char *arg; 
    arg = SCmd.next(); 
    
    if (arg != NULL) {

      //Complete newOttobotName char string
      int k = 0;
      while((*arg) && (k<11)){ 
          newOttobotName[k]=*arg++;
          k++;
      }
      
      EEPROM.put(eeAddress, newOttobotName); 
    }
    else 
    {
      //Ottobot.putMouth(xMouth);
      delay(2000);
     // Ottobot.clearMouth();
    }
      */
  //  sendFinalAck();

}
////
//-- Function to receive LED Matrix data
void receiveLED(){  

    //sendAck & stop if necessary
    sendAck();
    Ottobot.home();

    //Examples of receiveLED Bluetooth commands
    //L 000000001000010100100011000000000
    //L 001111111111111111111111111111111 (todos los LED encendidos)
    unsigned long int matrix;
    char *arg;
    char *endstr;
    arg=SCmd.next();
    //Serial.println (arg);
    if (arg != NULL) {
      matrix=strtoul(arg,&endstr,2);    // Converts a char string to unsigned long integer
      Ottobot.putMouth(matrix,false);
    }else{
      Ottobot.putMouth(xMouth);
      delay(2000);
      Ottobot.clearMouth();
    }

    sendFinalAck();

}

//-- Function to send Ottobot's name
void requestName(){

    Ottobot.home(); //stop if necessary

    char actualOttobotName[11]= "Zowi";  //Variable to store data read from EEPROM.
 //   int eeAddress = 5;            //EEPROM address to start reading from

    //Get the float data from the EEPROM at position 'eeAddress'
  //  EEPROM.get(eeAddress, actualOttobotName);

    BT.print(F("&&"));
    BT.print(F("E "));
    BT.print(actualOttobotName);
    BT.println(F("%%"));
    BT.flush();
}


//-- Function to send ultrasonic sensor measure (distance in "cm")
void requestDistance(){

    Ottobot.home();  //stop if necessary  

    int distance = Ottobot.getDistance();
    BT.print(F("&&"));
    BT.print(F("D "));
    BT.print(distance);
    BT.println(F("%%"));
    BT.flush();
}


//-- Function to send noise sensor measure
void requestNoise(){

    Ottobot.home();  //stop if necessary

    int microphone= Ottobot.getNoise(); //analogRead(PIN_NoiseSensor);
    BT.print(F("&&"));
    BT.print(F("N "));
    BT.print(microphone);
    BT.println(F("%%"));
    BT.flush();
}


//-- Function to send battery voltage percent
void requestBattery(){

    Ottobot.home();  //stop if necessary

    //The first read of the batery is often a wrong reading, so we will discard this value. 
    double batteryLevel = Ottobot.getBatteryLevel();

    BT.print(F("&&"));
    BT.print(F("B "));
    BT.print(batteryLevel);
    BT.println(F("%%"));
    BT.flush();
}


//-- Function to send program ID
void requestProgramId(){

    Ottobot.home();   //stop if necessary

    BT.print(F("&&"));
    BT.print(F("I "));
    BT.print(programID);
    BT.println(F("%%"));
    BT.flush();
}


//-- Function to send Ack comand (A)
void sendAck(){

  delay(30);

  BT.print(F("&&"));
  BT.print(F("A"));
  BT.println(F("%%"));
  BT.flush();
}


//-- Function to send final Ack comand (F)
void sendFinalAck(){

  delay(30);

  BT.print(F("&&"));
  BT.print(F("F"));
  BT.println(F("%%"));
  BT.flush();
}

//-- Functions with animatics
void LowBatteryAlarm(){

    double batteryLevel = Ottobot.getBatteryLevel();

    if(batteryLevel<45){
      Ottobot.bendTones (880, 2000, 1.04, 8, 3);  //A5 = 880
      
      delay(30);
      
      Ottobot.bendTones (2000, 880, 1.02, 8, 3);  //A5 = 880

      delay(500);     
    }
}
