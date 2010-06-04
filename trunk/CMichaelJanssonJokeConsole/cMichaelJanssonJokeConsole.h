
// *********************************************************************
// *     This software is made available only to individuals and only  *
// *     for educational purposes. Any and all commercial use is       *
// *     stricly prohibited.                                           *
// *********************************************************************
//**********************************************************************
//* Disclaimer: Any borrowed code used in this                         *
//*             program is the property of the                         *
//*             code originator. credit to them.                       *
//*                                                                    *
//*                                                                    *
//*   Unfinished                                                       *
//*   WARNING:                                                         *
//*                                                                    *
//*                                                                    *
//**********************************************************************

#ifndef CMICHAELJANSSONJOKECONSOLE_H
#define CMICHAELJANSSONJOKECONSOLE_H

#include "cMichaelJanssonTextMode.h"


class cMichaelJanssonJokeConsole : public  cMichaelJanssonTextMode
{
	public:
		cMichaelJanssonJokeConsole ();
		cMichaelJanssonJokeConsole ( int iCon);
		~cMichaelJanssonJokeConsole ();

void ReadConsoleKeyboardMouseInput();
void computeTime();
void delay(unsigned int milliseconds);
void initProgram();
void FileScrollUp();
void FileScrollDown();
void LineScrollDown();
void LineScrollUp();
void selectFile();
void selectLine();
void fileSelected();
void lineSelected();
void updateStatus();
void putFilesList();
void putLinesList();
void createRandomFiles();
void createRandomLines();
void getfiles(void);
void loadFile(char *file);
void displayJokeLine();
void getReady(void);

};

#endif CMICHAELJANSSONJOKECONSOLE_H