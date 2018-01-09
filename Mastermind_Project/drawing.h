//Michelle Livojevic SN:1415213
//Aaron Wang SN: 1504533

#ifndef _drawing_H_
#define _drawing_H_

void redrawCursor(int mode,int cursori);
void redrawScreen(int mode, int cursori);
void redrawCircle(int mode,int cursori, colourCode* code);
int CodeSort(int& attempts, colourCode* code);
void processJoystick(int mode ,int& attempts, int& numred, int& cursori, colourCode* code);
void displayMode0(int mode, colourCode* code);
void displayMenu();
void displayMode1(int mode, colourCode* code);

#endif
