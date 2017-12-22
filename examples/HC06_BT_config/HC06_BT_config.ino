/* 

FIRMWARE TO CONFIG HC 06 FOR COMPATIBLE ZOWI APP
After upload code you have 10 seconds to connect the module
HC06---- Arduino
TX ----> RX (D6)
RX <---- TX (D7)
Turn on  Serial MOnitor (Baud: 9600) to view Process, if configuration unsuccess, please try another baurate
Once the LED off configuration will start and at the end LED will blink
After this you can pair your module////////////////////////////////////////////////////////////
///////////////////////////////////////
////////////////////////////////////////
CHƯƠNG TRÌNH NÀY ĐƯỢC VIẾT RA ĐỂ CONFIG CHO MODULE BLUETOOTH HC-06 GỐC ĐỂ ROBOT ZOWI CÓ THỂ TƯƠNG THÍCH VỚI ANDROID ZOWI APP 
LUU Ý: VỚI MODULE ĐI KÈM THEO COMBO DO STEMBOT.VN CUNG CẤP, MODULE HC-06 ĐÃ ĐƯỢC CONFIG SẴN, KHÔNG CẦN PHẢI LOAD CHƯƠNG TRÌNH NÀY
SỬ DỤNG:
UPLOAD CHƯƠNG TRÌNH VÀO ROBOT
BẬT SERIAL MONITOR LÊN ĐỂ THEO DÕI QUÁ TRÌNH. 

Created by Hien Phan, Aug 2017
*/
// Options:
#include <SoftwareSerial.h>
	int ArduLED=13;				//Internal Arduino´s LED
	char ssid[10]		= "Zowi";	// Name for Bluetooth.
	char baudios		 = '4';		   // 1=>1200 baudios, 2=>2400, 3=>4800, 4=>9600 (default), 5=>19200, 6=>38400, 7=>57600, 8=>115200
	char password[10]	= "1234";		// Password for pairing
	
SoftwareSerial BT(6,7); //RX,TX
void setup()
{
	BT.begin(9600);		//9600bauds is the deafult baudrate for these modules.
					//if it´s not working try changing this baudrate to match your HC-06 initial setup
	Serial.begin(9600);
	    // Waiting time (10 seconds) onboard LED is ON:
     Serial.println("CONFIG HC06 BLUETOOTH MODULE");
		pinMode(ArduLED,OUTPUT);
		digitalWrite(ArduLED,HIGH);
		delay(10000);
		digitalWrite(ArduLED,LOW);
	
BT.print("AT"); 
waitResponse();
//delay(1000); // Now configuration start
Serial.println("SET NAME: Zowi");
BT.print("AT+NAME"); BT.print(ssid); delay(1000);   // Change Name of BT
waitResponse();
Serial.println("SET BAURATE: 9600");

BT.print("AT+BAUD"); BT.print(baudios); delay(1000);    // Change Baud
waitResponse();
Serial.println("SET PASS: 1234");

BT.print("AT+PIN"); BT.print(password); delay(1000);	    // Change Password
waitResponse();
}

void loop()
{
	// After programing bluetooth, onboard LED will Blink.
	digitalWrite(ArduLED, !digitalRead(ArduLED));
	delay(500);
}

void waitResponse()
{
  // BT.flush();
  String command = "";
  while (BT.available()==0); 
  while(BT.available()) { // While there is more to be read, keep reading.
      command += (char)BT.read();    
     // while (BT.available()==0);
     delay(100);
    } 
   Serial.println(command);  


}
