//Michelle Livojevic SN:1415213
//Aaron Wang SN: 1504533

#ifndef _tft_H_
#define _tft_H_

#include <Adafruit_ILI9341.h>
// purpose of Display is to make tft variable useable in mulitple files
// this variable is vital for for the use of Adafruit graphics functions
class Display{
	public:
		static Adafruit_ILI9341 tft;
};



#endif
