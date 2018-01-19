//Michelle Livojevic SN:1415213
//Aaron Wang SN: 1504533

/*Collaborations:

Some functions used for the the handshaking procedure
use Assignment 1 code created by Prof. Zachary Friggstad namely:
Wait_on_Serial3, uint32_to_Serial3, and uint32_from_Serial3.
An adapted version of these functions is used in this code.

*/


#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <SD.h>
#include "display.h"
#include "common.h"
#include "drawing.h"

#define DISPLAY_WIDTH  240
#define DISPLAY_HEIGHT 320

#define JOY_CENTER   512
#define JOY_DEADZONE 64

#define JOY_VERT  A1 // should connect A1 to pin VRx
#define JOY_HORIZ A0 // should connect A0 to pin VRy
#define JOY_SEL   2

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);


int idPin = 13;
int mode = 0;
//NOTE: declaration of struct colourCode is in the common.h file
colourCode code[4];

// This code sets up the serial port communication
// and the pushbuttons
void setup() {
	init();

	Serial.begin(9600);
	Serial3.begin(9600);
	tft.begin();

	tft.fillScreen(ILI9341_BLACK);
	pinMode(JOY_SEL, INPUT_PULLUP);
	pinMode(12, INPUT_PULLUP);
}

/* Sends an uint16_t to Serial3 starting with the most-significant
	 and finishing with the least-significant byte.
*/
void uint32_to_serial3(uint16_t num) {
  Serial3.write((char) (num >> 0));
  Serial3.write((char) (num >> 8));

}

/* Reads an uint32_t from Serial3, starting from the least-significant
   and finishing with the most significant byte.
 */
uint16_t uint32_from_serial3() {
  uint16_t num = 0;
  num = num | ((uint16_t) Serial3.read()) << 0;
  num = num | ((uint16_t) Serial3.read()) << 8;
  return num;
}

// Waits for the number of bytes, nbytes; for time, timeout

bool wait_on_serial3( uint8_t nbytes, long timeout ) {
  unsigned long deadline = millis() + timeout;//wraparound not a problem
  while (Serial3.available()<nbytes && (timeout<0 || millis()<deadline))
  {
    delay(1); // be nice, no busy loop
  }
  return Serial3.available()>=nbytes;
}

//NOTE: 10 ms delay is critial for client functionality
/* The client function is only called if
	 idPin on the Arduino is wired to 5V

	 summary of funtion behaviour
	 client sends C
 	 client waits for A
	 if client receieves A, then send A and mycode
	 	wait 10 ms
	 	client waits for B and lockcode
	 	client recieves  B and lockcode
		send A
		break
*/
void client(){
	while(true){
		Serial3.print("C");
		if (wait_on_serial3(1,1000)){
			char byteA = Serial3.read();
			if(byteA == 'A'){
				Serial.print("ack recieved");
				Serial3.print("A");
				uint32_to_serial3(code[0].mycode);
				uint32_to_serial3(code[1].mycode);
				uint32_to_serial3(code[2].mycode);
				uint32_to_serial3(code[3].mycode);
				delay(10);
				if (wait_on_serial3(9,1000)){
				char 	byteB = Serial3.read();
					if (byteB =='B'){
						for (int i=0;i<4;i++){
							code[i].lockcode = uint32_from_serial3();
						}
						Serial.print("code recieved");
						Serial3.print("A");
						break;
					}
				}
			}
		}
	}
}

//NOTE: 10 ms delay is critial for server functionality
/* The server function is only called if
	 idPin on the Arduino is wired to GND

	 summary of server behaviour:
	 server waits for C
 	 server sends A
	 wait 10 ms
	 if server receives A, then recieve A and lockcode

	 	server sends B and mycode
	 	server waits for A
		if server receives A, then break
*/

void server(){
	while(true){
		if (wait_on_serial3(1,1000)){
			char byteC = Serial3.read();
			if(byteC == 'C'){
				Serial.print("C recieved");
				Serial3.print("A");
				if (wait_on_serial3(9,1000)){
					char byteA = Serial3.read();
					delay(10);
					if (byteA == 'A'){
						for (int i=0;i<4;i++){
							code[i].lockcode = uint32_from_serial3();
						}
						Serial.print("ack recieved");

						Serial3.print("B");
						uint32_to_serial3(code[0].mycode);
						uint32_to_serial3(code[1].mycode);
						uint32_to_serial3(code[2].mycode);
						uint32_to_serial3(code[3].mycode);
						if (wait_on_serial3(1,1000)){
							char byteA = Serial3.read();
							if(byteA == 'A'){
								Serial.print("final ack recieved");
								break;
							}
						}
					}
				}
			}
		}
	}
}
/* doubles checks to see if the same colour
	 appears more than once in mycode
*/

bool doubles(){
	for (int i = 0;i < 4; i++){
		for (int j = 0;j < 4; j++){
			if (code[j].mycode == code[i].mycode && i != j){
				return 1;
				break;
			}
		}
	}
	return 0;
}


int main() {

	setup();

	displayMenu();
	displayMode0( mode, code);
	if ( doubles() ){
		tft.fillScreen(ILI9341_BLACK);
		tft.setCursor(12, 80);
		tft.setTextColor(0xFFFF, 0x0000);
		tft.setTextSize(3);
		tft.print("Invalid Code");
		tft.setTextSize(2);
		tft.setCursor(12,160);
		tft.setTextColor(ILI9341_YELLOW, 0x0000);
		tft.print("Press reset button");
		tft.setCursor(42,185);
		tft.print("to play again");
	}
	else{
		if  (digitalRead(idPin)==LOW) {
		//screen displaying waiting on code
		Serial.println("I am the server");
		server();
		}
		else{
		Serial.println("I am the client");
		client();
		//waiting to see if code is cracked
		}

		for (int i = 0 ;i < 4; i++){
			code[i].colourvali = 0;
		}

		mode = 1;

		displayMode1( mode, code);
	}

	Serial.end();
	Serial3.end();
	return 0;
}
