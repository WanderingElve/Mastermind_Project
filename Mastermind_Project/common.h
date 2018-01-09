//Michelle Livojevic SN:1415213
//Aaron Wang SN: 1504533


#ifndef _common_H_
#define _common_H_

// This headerfile declares the struct colourCode
// coulourCode contains four parts
// mycode:  The code you create in mode0 to send to the other Arduino
// lockcode: The code you recieve from the other Arduino
// keycode:  The code you input as a guess at what lockcode is
// colourvali: The index of the selected colour

struct colourCode {
	uint16_t mycode;
	uint16_t lockcode;
	uint16_t keycode;
	int colourvali;
};

extern colourCode code[4];

#endif