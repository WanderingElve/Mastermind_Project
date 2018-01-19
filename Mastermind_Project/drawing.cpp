//Michelle Livojevic SN:1415213
//Aaron Wang SN: 1504533

#include <Arduino.h>
#include <Adafruit_ILI9341.h>
#include "display.h"
#include "common.h"
#include "drawing.h"

#define JOY_CENTER   512
#define JOY_DEADZONE 64

#define JOY_VERT  A1 // should connect A1 to pin VRx
#define JOY_HORIZ A0 // should connect A0 to pin VRy
#define JOY_SEL   2

//Redraws the cursor around the circle
void redrawCursor(int mode,int cursori) {
	if(mode == 0){
		Display::tft. drawCircle((cursori + 1)*50-5, 210, 21, Display::tft.color565(255, 255, 255));
		delay(20);
	}
	else if (mode == 1){
		Display::tft.drawCircle(30*(1 + cursori)+5 , 14 + 29*(10), 14, Display::tft.color565(255, 255, 255));
		delay(20);
	}
}
//Draws a black circle over where the cursor was.
void redrawScreen(int mode, int cursori) {
	if(mode == 0){
		Display::tft. drawCircle((cursori + 1)*50-5, 210, 21, Display::tft.color565(0, 0, 0));
		delay(20);
	}
	else if(mode == 1){
		Display::tft.drawCircle(30*(1 + cursori)+5 , 14 + 29*(10), 14, Display::tft.color565(0, 0, 0));
		delay(20);
	}
}

/* redrawCircle redraws the selected circle with a different colourCode
	 the size and location of the redrawn circle is dependant on mode.
*/
void redrawCircle(int mode,int cursori, colourCode* code) {
	if (mode == 0){
		if ( code[cursori].colourvali == 0 ) {
			code[cursori].mycode = ILI9341_RED;
		}
		else if (code[cursori].colourvali  == 1 ) {
			code[cursori].mycode = ILI9341_ORANGE;
		}
		else if (code[cursori].colourvali  == 2 ) {
			code[cursori].mycode = ILI9341_YELLOW;
		}
		else if (code[cursori].colourvali  == 3 ) {
			code[cursori].mycode = ILI9341_GREEN;
		}
		else if (code[cursori].colourvali  == 4 ) {
			code[cursori].mycode = ILI9341_OLIVE;
		}
		else if (code[cursori].colourvali  == 5 ) {
			code[cursori].mycode = ILI9341_DARKCYAN;
		}
		else if (code[cursori].colourvali  == 6 ) {
			code[cursori].mycode = ILI9341_BLUE;
		}
		else if (code[cursori].colourvali  == 7 ) {
			code[cursori].mycode = ILI9341_PURPLE;
		}
		Display::tft.fillCircle((cursori + 1)*50 -5, 210, 20, code[cursori].mycode);
	}
	else if (mode == 1){
		if ( code[cursori].colourvali == 0 ) {
			code[cursori].keycode = ILI9341_RED;
		}
		else if (code[cursori].colourvali  == 1 ) {
			code[cursori].keycode = ILI9341_ORANGE;
		}
		else if (code[cursori].colourvali  == 2 ) {
			code[cursori].keycode = ILI9341_YELLOW;
		}
		else if (code[cursori].colourvali  == 3 ) {
			code[cursori].keycode = ILI9341_GREEN;
		}
		else if (code[cursori].colourvali  == 4 ) {
			code[cursori].keycode = ILI9341_OLIVE;
		}
		else if (code[cursori].colourvali  == 5 ) {
			code[cursori].keycode = ILI9341_DARKCYAN;
		}
		else if (code[cursori].colourvali  == 6 ) {
			code[cursori].keycode = ILI9341_BLUE;
		}
		else if (code[cursori].colourvali  == 7 ) {
			code[cursori].keycode = ILI9341_PURPLE;
		}
		Display::tft.fillCircle(30*(1 + cursori) +5, 14 + 29*(10), 13, code[cursori].keycode);
	}
	delay(500);
}

/* CodeSort prints a red circle for every correct colour in correct position
   and one white circle for every correct color in incorrect position.
*/
//NOTE: Color Codes have four parts, lockcodes are restricted
//      to being four unique colours.  keycodes however can be
// 			any variation of four colours.

int CodeSort(int& attempts, colourCode* code) {

	int numred = 0;
	int redi[4] = {0};
	uint16_t a[4];
	uint16_t b[4];
	for ( int k = 0; k < 4; k++) {
		if ( code[k].lockcode == code[k].keycode) {
		  numred++;
			redi[k]=1;
			Display::tft.fillCircle(20*(numred) + 130,15 + 29*attempts, 10, ILI9341_RED);
		}
	}
	for (int j=0;j<4;j++){
		a[j]=code[j].lockcode;
		b[j]=code[j].keycode;
	}

	int key0code[4] = {0};
	int key1code[4] = {0};
	int key2code[4] = {0};
	int key3code[4] = {0};

	for (int i = 0; i < 4; i++ ) {
		if (a[0] == b[i]) {key0code[i]++; }
		if (a[1] == b[i]) {key1code[i]++; }
		if (a[2] == b[i]) {key2code[i]++; }
		if (a[3] == b[i]) {key3code[i]++; }
	}

	int circle[4] = {0};
	circle[0] = key0code[0] + key0code[1] + key0code[2] + key0code[3];
	circle[1] = key1code[0] + key1code[1] + key1code[2] + key1code[3];
	circle[2] = key2code[0] + key2code[1] + key2code[2] + key2code[3];
	circle[3] = key3code[0] + key3code[1] + key3code[2] + key3code[3];

	circle[0] = constrain(circle[0], 0, 1);
	circle[1] = constrain(circle[1], 0, 1);
	circle[2] = constrain(circle[2], 0, 1);
	circle[3] = constrain(circle[3], 0, 1);
	for ( int i = 0; i < 4; i++) {
		circle[i] = circle[i] - redi[i];
	}
	// numwhite indicates how many white circles should be printed.
	int numwhite = circle[0] + circle[1] + circle[2] + circle[3];

	Serial.print("Numred: ");
	Serial.println(numred);
	Serial.print("Numwhite: ");
	Serial.println(numwhite);

	for ( int k = 0; k < numwhite; ++k ) {
		Display::tft.fillCircle(20*(k+numred) + 150,15 + 29*attempts, 10, ILI9341_WHITE);
	}
	// everytime CodeSort is called one attempt has been made
	attempts++;
	while(digitalRead(12) == LOW) {}
	return numred;
}

/* processJoystick processes the input for both mode0 and mode1
   changing the colour of the selected circle and allowing the user
	 to change which circle is selected
*/
void processJoystick(int mode ,int& attempts, int& numred, int& cursori, colourCode* code) {

	int yVal = analogRead(JOY_HORIZ);
	int xVal = analogRead(JOY_VERT);
	int buttonVal = digitalRead(JOY_SEL);

	while (digitalRead(JOY_SEL) == LOW) {
		redrawCircle(mode,cursori, code);
		code[cursori].colourvali++;
		code[cursori].colourvali = constrain(code[cursori].colourvali,0,7);
	}

	if ((abs(yVal- JOY_CENTER) > JOY_DEADZONE)) {
		
		// remember the y-reading increases as we push left
		if ((yVal > JOY_CENTER + JOY_DEADZONE) && code[cursori].colourvali != 7) {
			(code[cursori].colourvali)++;
			code[cursori].colourvali = constrain(code[cursori].colourvali,0,7);
			redrawCircle(mode,cursori, code);
		}
		else if ((yVal < JOY_CENTER - JOY_DEADZONE)&& code[cursori].colourvali != 0) {
			(code[cursori].colourvali)--;
			code[cursori].colourvali = constrain(code[cursori].colourvali,0,7);
			redrawCircle(mode,cursori, code);
		}
		Serial.print("colourvali: ");
		Serial.println(code[cursori].colourvali);
		delay(300);
	}

	if ( abs(xVal- JOY_CENTER) > JOY_DEADZONE ) {
		redrawScreen(mode, cursori);
		// remember the x-reading increases as we push left
		if (xVal < JOY_CENTER - JOY_DEADZONE) {
			Serial.print("cursori1: ");
			Serial.println(cursori);
		(cursori)--;
		Serial.print("cursori2: ");
		Serial.println(cursori);
			cursori = constrain(cursori, 0, 3);
		}
		else if (xVal > JOY_CENTER + JOY_DEADZONE) {
			Serial.print("cursori3: ");
			Serial.println(cursori);
			(cursori)++;
			Serial.print("cursori4: ");
			Serial.println(cursori);
			cursori = constrain(cursori, 0, 3);
		}
		redrawCursor(mode, cursori);
		delay(150);
		Serial.print("cursori5: ");
		Serial.println(cursori);
	}

	if (mode == 1){
		while(digitalRead(12) == LOW) {
			for ( int k = 0; k < 4; k++) {
				Display::tft.fillCircle(30*(k)+35, 14 + 29*(attempts), 13, code[k].keycode);
			}
			numred = CodeSort(attempts, code);
		}
	}
}

// Display Mode0: the screen where mycode- the first inputted code
// that the will be sent to the second Arduino.
void displayMode0(int mode, colourCode* code){
	int cursori = 0;
	Display::tft.fillScreen(ILI9341_BLACK);
	Display::tft.setCursor(6, 60);
	Display::tft.setTextWrap(false);
	Display::tft.setTextColor(0xFFFF, 0x0000);
	Display::tft.setTextSize(2);
	Display::tft.print("Input a colour code");
	Display::tft.setCursor(6, 90);
	Display::tft.print("(No repeat colours)");
	Display::tft.setCursor(24, 140);
	Display::tft.print("Press the button");
	Display::tft.setCursor(42, 155);
	Display::tft.print("to enter code");
	for (int i = 0; i < 4; i++){
		cursori = i;
		redrawScreen(mode, cursori);
		redrawCircle(mode, cursori, code);
	}
	cursori = 0;
	Serial.print("cursori: ");
	Serial.println(cursori);
	redrawCursor(mode, cursori);
	while (digitalRead(12) == HIGH) {
		// if joystick has been moved passed deadzone
		int attempts = 0;
		int numred=0;
		processJoystick(mode,attempts, numred, cursori, code);
	}
}

// Display Game Menu
void displayMenu(){
	Display::tft.fillScreen(ILI9341_BLACK);
	Display::tft.setCursor(21	, 75);
	Display::tft.setTextWrap(false);
	Display::tft.setTextSize(3);
	Display::tft.setTextColor(ILI9341_WHITE, 0x0000);
	Display::tft.print("Master Mind");
	Display::tft.setTextColor(ILI9341_RED, 0x0000);
	Display::tft.setCursor(30, 100);
	Display::tft.setTextSize(2);
	Display::tft.print("(Arduino Style)");
	Display::tft.setCursor(24,160);
	Display::tft.setTextColor(ILI9341_BLUE, 0x0000);
	Display::tft.print("Press the button");
	Display::tft.setCursor(72,185);
	Display::tft.print("to begin");
	while (digitalRead(12) == HIGH){};
}

// the screen where you try to guess lockcode with keycode
void displayMode1(int mode,colourCode* code) {
	Display::tft.fillScreen(ILI9341_BLACK);
	int cursori = 0;
	int i = 1;
	int count = 0;
	int m = 0;
	int end = 0;
	int end2 = 0;
	int numred = 0;
	int colour = ILI9341_RED;

	// draw the input circles
	for (int i=0;i<4;i++){
		cursori=i;
		redrawCircle(mode , cursori, code);
	}
	cursori=0;
	redrawCursor(mode , cursori);

	//draw the background circles for each attempt
	while( true ) {
		i++;
		end = end + 1;
		for ( int j = 0; j < 10; j++) {
			m++;
			count = m % 16;
			Display::tft.fillCircle(30*i -25,14 + 29*j, 13, colour);
		}
		if (end >= 4) {
		break;
		}
	}
	i=0;
	m=0;
	// draw the background circles to tell the user
	// how keycode compares to lockcode
	while( true ) {
		i++;
		end2 = end2 + 1;
		for ( int j = 0; j < 10; j++) {
			colour = ILI9341_DARKGREY;
			Display::tft.fillCircle(20*i + 130,15 + 29*j, 10, colour);
		}
		if (end2 >= 4) {
		break;
		}
	}

	int attempts = 0;
	// Continue processing input
	while (attempts<10) {
		processJoystick(mode, attempts , numred , cursori, code);
		if (numred == 4){
			break;
		}
	}
	// Display GAME OVER screen
	if (attempts >= 10){
		Display::tft.fillScreen(ILI9341_MAGENTA);
		Display::tft.setCursor(12, 75);
		Display::tft.setTextWrap(false);
		Display::tft.setTextSize(4);
		Display::tft.setTextColor(ILI9341_RED, ILI9341_MAGENTA);
		Display::tft.print("GAME OVER");
		Display::tft.setTextSize(2);
		Display::tft.setCursor(12,150);
		Display::tft.setTextColor(ILI9341_GREEN, ILI9341_MAGENTA);
		Display::tft.print("Press reset button");
		Display::tft.setCursor(42,175);
		Display::tft.print("to play again");
		Display::tft.setCursor(6,220);
		Display::tft.setTextColor(ILI9341_BLACK, ILI9341_MAGENTA);
		Display::tft.print("The secret code was");
		for (int i = 0; i < 4 ; i++){
		Display::tft.fillCircle(i*50 + 45,270, 20, code[i].lockcode);
		}
  }
	// Display YOU WIN screen
	else {
		Display::tft.fillScreen(ILI9341_BLUE);
		Display::tft.setCursor(24, 75);
		Display::tft.setTextWrap(false);
		Display::tft.setTextSize(4);
		Display::tft.setTextColor(ILI9341_ORANGE, ILI9341_BLUE);
		Display::tft.print("YOU WIN!");
		Display::tft.setTextSize(2);
		Display::tft.setCursor(12,150);
		Display::tft.setTextColor(ILI9341_LIGHTGREY, ILI9341_BLUE);
		Display::tft.print("Press reset button");
		Display::tft.setCursor(42,175);
		Display::tft.print("to play again");
	}
}
