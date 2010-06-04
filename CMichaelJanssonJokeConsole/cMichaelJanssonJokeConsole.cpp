
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

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <iostream.h>
#include <math.h>
#include <fstream.h>

#include "cMichaelJanssonJokeConsole.h"

//*********************************************
//*                                           *
//*********************************************

#define ver " "
#define prgdate "    "
#define author "Written by TopCoder"
#define website "  "

	int     keyCode,key;
	struct myFileData{
		   char name[49];
		   char name2[13];
		   long size;
	} files[50];

	//myFileData files[50];
	int cnt=0;
	char line[200][8192];
	char JokeTitle[40]={' '};
	int randomLine=0,maxLines=0,maxFiles=0,JokeLine=0,JokeFile=0;
	int doneFiles[100]={0};
	int doneLines[200]={0};
	int randomSwitch=-1,linearSwitch=-1;
	int TimeDelay=0,TimeWait=1;
	int currentFile=0,currentLine=0,unreadLines=0,unreadFiles=0;
	int mouseX=0,mouseY=0,buttonState=0;
	int fileListScroll=0,lineListScroll=0;
	int fileListSelect=0,lineListSelect=0;
	int milliseconds,seconds=0,minutes=0,hours=0;
	int randomMode=-1,linearMode=-1;
	clock_t startTime,countTime;
	HANDLE out_handle; // Handle to the "screen"
	HANDLE in_handle; // Handle to the "keyboard"
	INPUT_RECORD input_record;
	DWORD events;

	//**************************************
	//**                                  **
	//**************************************
	cMichaelJanssonJokeConsole::cMichaelJanssonJokeConsole ()  :   cMichaelJanssonTextMode()
	{
	}
	//**************************************
	//**                                  **
	//**************************************

	cMichaelJanssonJokeConsole::cMichaelJanssonJokeConsole ( int icode)  :   cMichaelJanssonTextMode()
	{
	   getReady();
	   initProgram();
	   while(keyCode!=27)   ReadConsoleKeyboardMouseInput();
	   setcolor(7);clrscr();gotoxy(1,23);
	}
	//**************************************
	//**                                  **
	//**************************************

	cMichaelJanssonJokeConsole::~cMichaelJanssonJokeConsole ()
	{
	}
	//**************************************
	//**                                  **
	//**************************************


//*********************************************
//*                                           *
//*********************************************


void cMichaelJanssonJokeConsole::ReadConsoleKeyboardMouseInput()
{
	 ReadConsoleInput(in_handle, &input_record, 1, &events);
     mouseX = input_record.Event.MouseEvent.dwMousePosition.X;
     mouseY = input_record.Event.MouseEvent.dwMousePosition.Y;
	 buttonState = input_record.Event.MouseEvent.dwButtonState;
	 keyCode = input_record.Event.KeyEvent.wVirtualKeyCode;
	 //setcolor(224);gotoxy(50,1);printf("mus=%02ix%02i,btn=%02i,kbd=%02i ",mouseX,mouseY,keyCode,buttonState);

	if(input_record.EventType == MOUSE_EVENT)
	{
		if(input_record.Event.MouseEvent.dwButtonState != FROM_LEFT_1ST_BUTTON_PRESSED) return;
		if(input_record.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
	      mouseX = input_record.Event.MouseEvent.dwMousePosition.X;
		  mouseY = input_record.Event.MouseEvent.dwMousePosition.Y;


          if ((mouseX>38) && (mouseX<42) && (mouseY==16))
		  {

			  FileScrollUp();
 		      currentFile=fileListSelect+fileListScroll;
			  unreadFiles=maxFiles-currentFile;
			  putFilesList();

               clrbox(39,16,42,17,15);
	           gotoxy(40,16);printf("<");
			   delay(200);
               clrbox(39,16,42,17,255);
	           gotoxy(40,16);printf("<");
		  }
          if ((mouseX>38) && (mouseX<42) && (mouseY==23))
		  {
				FileScrollDown();
 		        currentFile=fileListSelect+fileListScroll;
			    unreadFiles=maxFiles-currentFile;
                putFilesList();

			  	clrbox(39,23,42,24,15);
	            gotoxy(40,23);printf(">");
				delay(200);
			  	clrbox(39,23,42,24,240);
	            gotoxy(40,23);printf(">");
		  }

		  if ((mouseX>23) && (mouseX<37) && (mouseY>15) && (mouseY<24))
		  {
			  lineListScroll=0;
		      lineListSelect=0;
			  fileListSelect=(mouseY-16);//+fileListScroll;
	          fileSelected();
		  }

          if ((mouseX>48) && (mouseX<52) && (mouseY==16))
		  {
              LineScrollUp();
			  clrbox(49,16,52,17,15);
	          gotoxy(50,16);printf("<");
			  delay(200);
			  clrbox(49,16,52,17,241);
	          gotoxy(50,16);printf("<");
              lineSelected();
		  }
          if ((mouseX>48) && (mouseX<52) && (mouseY==23))
		  {
              LineScrollDown();
			  clrbox(49,23,52,24,15);
	          gotoxy(50,23);printf(">");
	          delay(200);
			  clrbox(49,23,52,24,241);
	          gotoxy(50,23);printf(">");
			  lineSelected();
		  }

		  if ((mouseX>43) && (mouseX<49) && (mouseY>15) && (mouseY<24))
		  {
			  lineListSelect=(mouseY-16);
			  lineSelected();
		  }

		  if ((mouseX>53) && (mouseX<65) && (mouseY>15) && (mouseY<19))
		  {
	        clrbox(54,16,65,19,15 );gotoxy(55,17);printf("Prev Line");
			delay(200);
			clrbox(54,16,65,19,224);gotoxy(55,17);printf("Prev Line");

			LineScrollUp();
 		    currentLine=lineListSelect+lineListScroll;
			unreadLines=maxLines-currentLine;
			lineSelected();
		  }

		  if ((mouseX>66) && (mouseX<77) && (mouseY>15) && (mouseY<19))
		  {
	        clrbox(66,16,77,19,15);gotoxy(67,17);printf("Next Line");
			delay(200);
			clrbox(66,16,77,19,224);gotoxy(67,17);printf("Next Line");
			LineScrollDown();
 		    currentLine=lineListSelect+lineListScroll;
			unreadLines=maxLines-currentLine;
			lineSelected();
		  }

		  if ((mouseX>53) && (mouseX<65) && (mouseY>18) && (mouseY<21))
		  {
	        clrbox(54,19,65,22,15);gotoxy(55,20);printf("Prev File");
			delay(200);
			clrbox(54,19,65,22,31);gotoxy(55,20);printf("Prev File");

			FileScrollUp();
			fileSelected();
		  }


		  if ((mouseX>66) && (mouseX<77) && (mouseY>18) && (mouseY<21))
		  {
	        clrbox(66,19,77,22,15);gotoxy(67,20);printf("Next File");
			delay(200);
			clrbox(66,19,77,22,31);gotoxy(67,20);printf("Next File");

			FileScrollDown();
			fileSelected();
		  }

		  if ((mouseX>53) && (mouseX<65) && (mouseY>21) && (mouseY<25))
		  {
	        clrbox(54,22,65,25,15);gotoxy(55,23);printf("Rand Line");
			delay(200);
			clrbox(54,22,65,25,79);gotoxy(55,23);printf("Rand Line");

			 currentLine++;
             JokeLine=doneLines[currentLine];
			 //JokeLine=currentLine;
			 unreadLines=maxLines-currentLine;
			 selectLine();
             lineSelected();
			if (currentLine>maxLines)
			{
			  currentFile++;
              JokeFile=doneFiles[currentFile];
			  unreadFiles=maxFiles-currentFile;
			  selectFile();
			  putFilesList();

			  maxLines=0;
			  loadFile(files[JokeFile].name);
 	          //createRandomLines();
			  lineListScroll=0;
		      lineListSelect=0;
              currentLine=0;
			  currentLine++;
              JokeLine=doneLines[currentLine];
			  unreadLines=maxLines-currentLine;
			  selectLine();
	          lineSelected();
			}
		  }

		  if ((mouseX>66) && (mouseX<77) && (mouseY>21) && (mouseY<25))
		  {
	        clrbox(66,22,77,25,15);gotoxy(67,23);printf("Rand File");
			delay(200);
			clrbox(66,22,77,25,79);gotoxy(67,23);printf("Rand File");
			//linearMode=-linearMode;
			  lineListScroll=0;
		      lineListSelect=0;
			currentFile++;
             JokeFile=doneFiles[currentFile];
			 unreadFiles=maxFiles-currentFile;
			 selectFile();
             fileSelected();
			 if (currentFile>maxFiles)
			 {
				currentFile=-1;
				fileListScroll=0;
				fileListSelect=0;
			 }

		  }

          if ((mouseX>1) && (mouseX<9) && (mouseY==0))
		  {
			  setcolor(15);gotoxy(2,0);printf(" EXIT ");
			  delay(500);
			  setcolor(79);gotoxy(2,0);printf("EXIT  ");
			  delay(500);
			  setcolor(15);
			  clrscr();
			  exit(0);
		  }

			return;

		}

	}

}


//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::computeTime()
{
	countTime=clock();

	milliseconds=countTime-startTime;
	seconds=(milliseconds/(CLOCKS_PER_SEC))-(minutes*60);
	minutes=(milliseconds/(CLOCKS_PER_SEC))/60;
	hours=minutes/60;

   gotoxy(64,17);printf("time %02i:%02i:%02i ",hours,minutes,seconds);
   gotoxy(64,18);printf("ms  = %i ",milliseconds);

}
//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::delay(unsigned int milliseconds)
{
	clock_t ticks1, ticks2;
	unsigned int tick=0;

	ticks1=clock();
	while(tick<milliseconds)
	{
		ticks2=clock();
		tick=ticks2-ticks1;
	}
	ticks2=clock();
}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::initProgram()
{
	out_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	in_handle = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(in_handle,ENABLE_MOUSE_INPUT);

	srand(time(0));
	srand(GetTickCount()); // Seed random number generator
    getfiles();

	createRandomFiles();

	JokeFile=doneFiles[0];
	JokeLine=doneLines[0];
    loadFile(files[JokeFile].name);
	createRandomLines();

	clrbox(1,2,79,15,31);
	//updateStatus();

    selectFile();
	selectLine();
    putFilesList();
    putLinesList();
    displayJokeLine();

    clrbox(1,0,79,1,79);
	gotoxy(2,0);printf("EXIT");
	//gotoxy(12,0);printf("Info");

	clrbox(54,16,65,19,224);gotoxy(55,17);printf("Prev Line");
	clrbox(66,16,77,19,224);gotoxy(67,17);printf("Next Line");

	clrbox(54,19,65,22,31);gotoxy(55,20);printf("Prev File");
	clrbox(66,19,77,22,31);gotoxy(67,20);printf("Next File");

	clrbox(54,22,65,25,79);gotoxy(55,23);printf("Rand Line");
	clrbox(66,22,77,25,79);gotoxy(67,23);printf("Rand File");

}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::FileScrollUp()
{
  if (fileListScroll>7) fileListScroll--;
  else
  {
      fileListSelect--;
      if (fileListSelect<1) fileListSelect=0;
  	  fileListScroll=0;
  }

}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::FileScrollDown()
{
  if (fileListSelect<7) fileListSelect++;
  else
  if ((fileListSelect+fileListScroll)<maxFiles-1) fileListScroll++;
}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::LineScrollDown()
{
  if (lineListSelect<7) lineListSelect++;
  else
  if ((lineListSelect+lineListScroll)<maxLines-1) lineListScroll++;
}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::LineScrollUp()
{
	  if (lineListScroll>7) lineListScroll--;
	  else
	  {
         lineListSelect--;
  	     if (lineListSelect<1) lineListSelect=0;
		 lineListScroll=0;
	  }

}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::selectFile()
{
    fileListSelect=JokeFile;
	if (fileListSelect>6)
	{
		fileListSelect=7;
		fileListScroll=JokeFile-7;
	}

}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::selectLine()
{
    lineListSelect=JokeLine;
	if (lineListSelect>6)
	{
		lineListSelect=7;
		lineListScroll=JokeLine-7;
	}

}


void cMichaelJanssonJokeConsole::fileSelected()
{
   putFilesList();
   JokeFile=fileListSelect+fileListScroll;
   loadFile(files[JokeFile].name);
   putLinesList();
   updateStatus();
   displayJokeLine();
}

void cMichaelJanssonJokeConsole::lineSelected()
{
   putLinesList();
   JokeLine=lineListSelect+lineListScroll;
   displayJokeLine();
   updateStatus();
}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::updateStatus()
{
	clrbox(1,16,22,24,31);
	gotoxy(1,16);printf("Files = %02i",maxFiles);
	gotoxy(1,17);printf("File = %02i of %02i",JokeFile+1,maxFiles);
    gotoxy(1,18);printf("Unread = %02i ",unreadFiles);
	gotoxy(1,19);printf("%s",files[JokeFile].name2);
	gotoxy(1,20);printf("Lines = %02i",maxLines);
    gotoxy(1,21);printf("Line  = %02i of %02i",JokeLine+1,maxLines);
	gotoxy(1,22);printf("Unread = %02i ",unreadLines);
	gotoxy(1,23);printf("TimeDelay = %02i",TimeDelay);

}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::putFilesList()
{
	clrbox(24,16,42,24,31);
	clrbox(39,16,42,24,224);

	clrbox(39,16,42,17,240);
	gotoxy(40,16);printf("<");

	clrbox(39,23,42,24,240);
	gotoxy(40,23);printf(">");
	setcolor(31);

	for (int f=0;f<8;f++)
	{
		gotoxy(24,16+f);printf(files[f+fileListScroll].name2);
	}

	 setcolor(240);
	 gotoxy(24,16+fileListSelect);printf(files[fileListSelect+fileListScroll].name2);
	 setcolor(31);
	 JokeFile=currentFile;
	 updateStatus();
}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::putLinesList()
{
	clrbox(44,16,52,24,31);
	clrbox(49,16,52,24,224);

	clrbox(49,16,52,17,241);
	gotoxy(50,16);printf("<");

	clrbox(49,23,52,24,241);
	gotoxy(50,23);printf(">");
	setcolor(31);

	for (int e=0;e<8;e++)
	{
		gotoxy(46,16+e);printf("%02i",(e+lineListScroll)+1);
	}
	 setcolor(240);
	 gotoxy(46,16+lineListSelect);printf("%02i",(lineListSelect+lineListScroll)+1);
	 setcolor(31);


}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::createRandomFiles()
{
  for (int i=0;i<maxFiles;i++)
  {
	  doneFiles[i]=rand() % maxFiles;
	  for(int j=0;j<i;j++) if (doneFiles[j]==doneFiles[i]) i--;
  }

}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::createRandomLines()
{
  for (int i=0;i<maxLines;i++)
  {
	  doneLines[i]=rand() % maxLines;
	  for(int j=0;j<i;j++) if (doneLines[j]==doneLines[i]) i--;
  }

}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::getfiles(void)
{
   WIN32_FIND_DATA fd;
   HANDLE hSearch;
   char pszSearchName[] = "jokes\\*.txt";
   char  buffer[80]={' '};
   int i,iSize=0;

   i=0;
   hSearch = FindFirstFile(pszSearchName, &fd);

   if(hSearch == INVALID_HANDLE_VALUE)
   {
	   cout<<" Error = INVALID_HANDLE_VALUE , No Joke files  found \n";
   }

   if(hSearch != INVALID_HANDLE_VALUE)
   {
      do
      {
		strcpy(files[i].name,"jokes\\");
		strcat(files[i].name,fd.cFileName);
		strcpy(files[i].name2,fd.cFileName);
		files[i].size=(fd.nFileSizeHigh * MAXDWORD) + fd.nFileSizeLow;
		//cout<<i<<".  "<<files[i].name<<"  "<<files[i].size<<" bytes\n";
        i++;
      }while(FindNextFile(hSearch, &fd));
      FindClose(hSearch);
   }
   maxFiles=i;

//*********************************************
//*                                           *
//*********************************************
}

void cMichaelJanssonJokeConsole::loadFile(char *file)
{
	char buffer[256];
	ifstream in;

    for (int ii=0;ii<200;ii++) 	for (int jj=0;jj<1024;jj++)	line[ii][jj]=' ';

    unreadLines=0;
	in.open(file);
    in.getline(JokeTitle,40);

    //clrbox(0,2,80,15,0);
	clrbox(0,2,80,15,0);
    clrbox(1,1,79,2,224);
	gotoxy(2,1);printf("Random Jokes:");
	setcolor(224);gotoxy(25,1);printf("%s        ",JokeTitle);
	clrbox(1,2,79,15,31);

	if (!in.is_open())
	{
		cout << "Error opening file";
		exit(1);
	}
	int i=0;
	while (!(in.eof()))
	{
		in.getline(buffer,255);
	    strcpy(line[i],buffer);
		//cout<<line[i]<<"\n";
		i++;
	}
	maxLines=i;
	createRandomLines();
	in.close();
}

//*********************************************
//*                                           *
//*********************************************

void cMichaelJanssonJokeConsole::displayJokeLine()
{
	int pos=0,x=0,y=0;

	clrbox(1,2,79,15,31);
	setcolor(31);
	while((line[JokeLine][pos]!='\0') &&(pos<700))
	{
      gotoxy(x+2,y+3);printf("%c",line[JokeLine][pos]);
	  pos++;
	  x++;
	  if ((x>65) && (line[JokeLine][pos]==' '))
	  {
		  x=0 ;
		  y++;
	  }

	}

}




void cMichaelJanssonJokeConsole::getReady(void)
{
	setcolor(15);
	clrscr();

	clrbox(2,2,78,6,224);
    gotoxy(3,2);printf(" %s ",ver);
     gotoxy(3,3);printf(" %s ",prgdate);
	gotoxy(3,4);printf(" %s ",author);
	gotoxy(3,5);printf(" %s ",website);
	 //  box(1,1,78,23,15,15,"");

    clrbox(2,7,54,22,79);
    gotoxy(3,7); printf(" The tasteless jokes that appear in the program   ");
    gotoxy(3,8); printf(" are taken from the Internet");
    gotoxy(3,9); printf(" and the viewsexpressed in the jokes are not  ");
    gotoxy(3,10);printf(" the programmer's own principle views and takes ");
    gotoxy(3,12);printf(" a firm stand from these inhumane joke ....  ");
    gotoxy(3,13);printf(" Select Yes, because in their sole discretion ..");
    gotoxy(3,14);printf(" Sensitive persons are advised to Choose NO!  ");
    gotoxy(3,16);printf(" The Programmer would like to avoid  ");
    gotoxy(3,17);printf(" unwarranted criticism aimed at him  ");
    gotoxy(3,18);printf("  because the user selected Yes." );
    gotoxy(3,19);printf("        Press Y or N");
    gotoxy(3,20);printf(" Continue?  (Yes/No)  " );

 ;
    keyCode=0;
	key=getch();
	setcolor(0);
	clrscr();
    setcolor(7);
	if ((toupper(key)!='J') && (toupper(key)!='Y') ) exit(0);

}







