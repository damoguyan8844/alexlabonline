

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
//*                                                                    *
//**********************************************************************

#ifndef  MICHAELJANSSONTextMode_H 
#define MICHAELJANSSONTextMode_H 

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <iostream.h>
#include <math.h>
#include <fstream.h>

class cMichaelJanssonTextMode
{
	public:

		cMichaelJanssonTextMode(){}
		cMichaelJanssonTextMode(int startUp){}
		~cMichaelJanssonTextMode(){}

		int temp,kbChar;


		/*
		void getKey();
		void gotoxy(int x, int y);
		void setcolor(WORD color);
		void textColor(unsigned char fColor,unsigned char bColor);
		void clrscr();
		void delay(int milliseconds);
		void txtPlot(int x, int y,int color);
		void txtLine( int xx1, int yy1, int xx2, int yy2 ,int color);


		*/
		//*****************************************************************************
		//*                                                                           *
		//*****************************************************************************

		void gotoxy(int x, int y)
		{
			COORD coord;
			coord.X = x; coord.Y = y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			return;
		}
		//*****************************************************************************
		//*                                                                           *
		//*****************************************************************************

		void setcolor(WORD color)
		{
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),color);
			return;
		}
		//*****************************************************************************
		//*                                                                           *
		//*****************************************************************************

		void clrscr()
		{
			COORD coordScreen = { 0, 0 };
			DWORD cCharsWritten;
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			DWORD dwConSize;
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

			GetConsoleScreenBufferInfo(hConsole, &csbi);
			dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
			FillConsoleOutputCharacter(hConsole, TEXT(' '), dwConSize, coordScreen, &cCharsWritten);
			GetConsoleScreenBufferInfo(hConsole, &csbi);
			FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
			SetConsoleCursorPosition(hConsole, coordScreen);
			return;
		}
		//*****************************************************************************
		//*                                                                           *
		//*****************************************************************************

		void box(unsigned x,unsigned y,unsigned sx,unsigned sy,unsigned char col,unsigned char col2,char text_[])
		{  unsigned i,j,m;
			{

			   m=(sx-x);                       //differential
			   j=m/8;                          //adjust
			   j=j-1;                          //more adjustment
			   gotoxy(x,y);cprintf("É");       //Top left corner of box
			   gotoxy(sx,y);cprintf("»");      //Top right corner of box
			   gotoxy(x,sy);cprintf("È");      //Bottom left corner of box
			   gotoxy(sx,sy);cprintf("¼");     //Bottom right corner of box

			   for (i=x+1;i<sx;i++)
			   {
				  gotoxy(i,y);cprintf("Í");     // Top horizontol line
				  gotoxy(i,sy);cprintf("Í");    // Bottom Horizontal line
			   }

			   for (i=y+1;i<sy;i++)
			   {
				  gotoxy(x,i);cprintf("º");     //Left Vertical line
				  gotoxy(sx,i);cprintf("º");    //Right Vertical Line
			   }

				  gotoxy(x+j,y);cprintf(text_); //put Title
				  gotoxy(1,24);
			}
		}
		//*****************************************************************************
		//*                                                                           *
		//*****************************************************************************

		void clrbox(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2,unsigned char bkcol)
		{
			int x,y;
			setcolor(bkcol);                       //Set to color bkcol

			for (y=y1;y<y2;y++)                    //Fill Y Region Loop
			{
				for (x=x1;x<x2;x++)               //Fill X region Loop
				{
				  gotoxy(x,y);cprintf(" ");       //Draw Solid space
				}
			}
		}
		//*****************************************************************************
		//*                                                                           *
		//*****************************************************************************

		void putbox(unsigned x,unsigned y,unsigned sx,unsigned sy,
				 unsigned char col, unsigned char col2,unsigned char bkcol,char text_[])
		{
			clrbox(x,y,sx,sy,bkcol);
			box(x,y,sx,sy,col,col2,text_);
		}

		//*****************************************************************************
		//*                                                                           *
		//*****************************************************************************

};


#endif MICHAELJANSSONTextMode_H 