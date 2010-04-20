# Microsoft Developer Studio Project File - Name="EMX" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=EMX - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EMX.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EMX.mak" CFG="EMX - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EMX - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "EMX - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EMX - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 xlcall32.lib ug97rndm.lib wininet.lib /nologo /subsystem:windows /dll /machine:I386 /out:"Release/EasyIF.xll"

!ELSEIF  "$(CFG)" == "EMX - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EMX - Win32 Release"
# Name "EMX - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\CFGOpts.cpp
# End Source File
# Begin Source File

SOURCE=.\ColEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\ColGen.cpp
# End Source File
# Begin Source File

SOURCE=.\ColPList.cpp
# End Source File
# Begin Source File

SOURCE=.\ColPos.cpp
# End Source File
# Begin Source File

SOURCE=.\DupMtx.cpp
# End Source File
# Begin Source File

SOURCE=.\EMX.cpp
# End Source File
# Begin Source File

SOURCE=.\EMX.def
# End Source File
# Begin Source File

SOURCE=.\EMX.rc
# End Source File
# Begin Source File

SOURCE=.\excel9.cpp
# End Source File
# Begin Source File

SOURCE=.\FLDProps.cpp
# End Source File
# Begin Source File

SOURCE=.\FlotGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\FlotThrd.cpp
# End Source File
# Begin Source File

SOURCE=.\FlotValP.cpp
# End Source File
# Begin Source File

SOURCE=.\Framewrk.cpp
# End Source File
# Begin Source File

SOURCE=.\FSGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\GFCMatrixDetails.cpp
# End Source File
# Begin Source File

SOURCE=.\GFRSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\GridFromCellWiz.cpp
# End Source File
# Begin Source File

SOURCE=.\GridList.cpp
# End Source File
# Begin Source File

SOURCE=.\GridWiz.cpp
# End Source File
# Begin Source File

SOURCE=.\GridWizChooser.cpp
# End Source File
# Begin Source File

SOURCE=.\HComWin.cpp
# End Source File
# Begin Source File

SOURCE=.\MATRIX.CPP
# End Source File
# Begin Source File

SOURCE=.\MtrxSel.cpp
# End Source File
# Begin Source File

SOURCE=.\MtxEdCol.cpp
# End Source File
# Begin Source File

SOURCE=.\MtxEdDat.cpp
# End Source File
# Begin Source File

SOURCE=.\MtxEdGen.cpp
# End Source File
# Begin Source File

SOURCE=.\MtxEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\MtxGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\PswDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\..\Dundas\ug97mfc\Addons\UGMEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\WizGrPos.cpp
# End Source File
# Begin Source File

SOURCE=.\WizGrRng.cpp
# End Source File
# Begin Source File

SOURCE=.\WizGrSel.cpp
# End Source File
# Begin Source File

SOURCE=.\Xlauto.cpp
# End Source File
# Begin Source File

SOURCE=.\xlCoerce.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\CFGOpts.h
# End Source File
# Begin Source File

SOURCE=.\ColEdit.h
# End Source File
# Begin Source File

SOURCE=.\ColGen.h
# End Source File
# Begin Source File

SOURCE=.\ColPList.h
# End Source File
# Begin Source File

SOURCE=.\ColPos.h
# End Source File
# Begin Source File

SOURCE=.\DupMtx.h
# End Source File
# Begin Source File

SOURCE=.\EMX.h
# End Source File
# Begin Source File

SOURCE=.\excel9.h
# End Source File
# Begin Source File

SOURCE=.\FLDProps.h
# End Source File
# Begin Source File

SOURCE=.\FlotGrid.h
# End Source File
# Begin Source File

SOURCE=.\FlotThrd.h
# End Source File
# Begin Source File

SOURCE=.\FlotValP.h
# End Source File
# Begin Source File

SOURCE=.\FRAMEWRK.H
# End Source File
# Begin Source File

SOURCE=.\FSGrid.h
# End Source File
# Begin Source File

SOURCE=.\GFCMatrixDetails.h
# End Source File
# Begin Source File

SOURCE=.\GFRSetup.h
# End Source File
# Begin Source File

SOURCE=.\GridFromCellWiz.h
# End Source File
# Begin Source File

SOURCE=.\GridList.h
# End Source File
# Begin Source File

SOURCE=.\GridWiz.h
# End Source File
# Begin Source File

SOURCE=.\GridWizChooser.h
# End Source File
# Begin Source File

SOURCE=.\HComWin.h
# End Source File
# Begin Source File

SOURCE=.\MATRIX.H
# End Source File
# Begin Source File

SOURCE=.\MtrxSel.h
# End Source File
# Begin Source File

SOURCE=.\MtxEdCol.h
# End Source File
# Begin Source File

SOURCE=.\MtxEdDat.h
# End Source File
# Begin Source File

SOURCE=.\MtxEdGen.h
# End Source File
# Begin Source File

SOURCE=.\MtxEdit.h
# End Source File
# Begin Source File

SOURCE=.\MtxGrid.h
# End Source File
# Begin Source File

SOURCE=.\PswDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\..\Dundas\ug97mfc\Addons\UGMEdit.h
# End Source File
# Begin Source File

SOURCE=.\WizGrPos.h
# End Source File
# Begin Source File

SOURCE=.\WizGrRng.h
# End Source File
# Begin Source File

SOURCE=.\WizGrSel.h
# End Source File
# Begin Source File

SOURCE=.\xlCoerce.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\EMX.rc2
# End Source File
# Begin Source File

SOURCE=.\res\fghide.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fgnew.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fgnew1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fullscre.bmp
# End Source File
# Begin Source File

SOURCE=.\res\gridwiz_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\insertro.bmp
# End Source File
# Begin Source File

SOURCE=.\res\nsresize.cur
# End Source File
# Begin Source File

SOURCE=.\res\recalcno.bmp
# End Source File
# Begin Source File

SOURCE=.\res\weresize.cur
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
