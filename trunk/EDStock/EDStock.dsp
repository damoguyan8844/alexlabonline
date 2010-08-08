# Microsoft Developer Studio Project File - Name="EDStock" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EDStock - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EDStock.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EDStock.mak" CFG="EDStock - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EDStock - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EDStock - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
F90=df.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EDStock - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE F90 /compile_only /nologo /warn:nofileopt /winapp
# ADD F90 /compile_only /nologo /warn:nofileopt /winapp
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "EDStock - Win32 Debug"

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
# ADD BASE F90 /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt /winapp
# ADD F90 /browser /check:bounds /compile_only /dbglibs /debug:full /nologo /traceback /warn:argument_checking /warn:nofileopt /winapp
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EDStock - Win32 Release"
# Name "EDStock - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat;f90;for;f;fpp"
# Begin Source File

SOURCE=.\AfxCore.cpp
# End Source File
# Begin Source File

SOURCE=.\Alarm.cpp
# End Source File
# Begin Source File

SOURCE=.\BaseData.cpp
# End Source File
# Begin Source File

SOURCE=.\CommFuns.cpp
# End Source File
# Begin Source File

SOURCE=.\Container.cpp
# End Source File
# Begin Source File

SOURCE=.\Database.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawTech.cpp
# End Source File
# Begin Source File

SOURCE=.\DRData.cpp
# End Source File
# Begin Source File

SOURCE=.\EDStock.cpp
# End Source File
# Begin Source File

SOURCE=.\EDStock.rc
# End Source File
# Begin Source File

SOURCE=.\EDStockDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\EDStockView.cpp
# End Source File
# Begin Source File

SOURCE=.\Express.cpp
# End Source File
# Begin Source File

SOURCE=.\GraphView.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=.\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=.\IMCode.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InPlaceList.cpp
# End Source File
# Begin Source File

SOURCE=.\KData.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MathParser.cpp
# End Source File
# Begin Source File

SOURCE=.\Minute.cpp
# End Source File
# Begin Source File

SOURCE=.\Outline.cpp
# End Source File
# Begin Source File

SOURCE=.\Packets.cpp
# End Source File
# Begin Source File

SOURCE=.\Profile.cpp
# End Source File
# Begin Source File

SOURCE=.\QianLong.cpp
# End Source File
# Begin Source File

SOURCE=.\RealTime.cpp
# End Source File
# Begin Source File

SOURCE=.\RealTimeView.cpp
# End Source File
# Begin Source File

SOURCE=.\Report.cpp
# End Source File
# Begin Source File

SOURCE=.\SelfDB.cpp
# End Source File
# Begin Source File

SOURCE=.\ShengLong.cpp
# End Source File
# Begin Source File

SOURCE=.\SListView.cpp
# End Source File
# Begin Source File

SOURCE=.\SpArchive.cpp
# End Source File
# Begin Source File

SOURCE=.\SpAssert.cpp
# End Source File
# Begin Source File

SOURCE=.\SpColl.cpp
# End Source File
# Begin Source File

SOURCE=.\SpDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\SpFile.cpp
# End Source File
# Begin Source File

SOURCE=.\SpMemory.cpp
# End Source File
# Begin Source File

SOURCE=.\SpObject.cpp
# End Source File
# Begin Source File

SOURCE=.\SpString.cpp
# End Source File
# Begin Source File

SOURCE=.\SpTime.cpp
# End Source File
# Begin Source File

SOURCE=.\SpValid.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\STKDRV.cpp
# End Source File
# Begin Source File

SOURCE=.\StkReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\Stock.cpp
# End Source File
# Begin Source File

SOURCE=.\Stock.inl
# End Source File
# Begin Source File

SOURCE=.\StockGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\Strategy.cpp
# End Source File
# Begin Source File

SOURCE=.\Strings.cpp
# End Source File
# Begin Source File

SOURCE=.\StStore.cpp
# End Source File
# Begin Source File

SOURCE=.\TechCLK.cpp
# End Source File
# Begin Source File

SOURCE=.\TechEnergy.cpp
# End Source File
# Begin Source File

SOURCE=.\TechKLine.cpp
# End Source File
# Begin Source File

SOURCE=.\Technique.cpp
# End Source File
# Begin Source File

SOURCE=.\TechOther.cpp
# End Source File
# Begin Source File

SOURCE=.\TechSwing.cpp
# End Source File
# Begin Source File

SOURCE=.\TechTrend.cpp
# End Source File
# Begin Source File

SOURCE=.\TechUser.cpp
# End Source File
# Begin Source File

SOURCE=.\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Tongshi.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl;fi;fd"
# Begin Source File

SOURCE=.\AfxCore.h
# End Source File
# Begin Source File

SOURCE=.\Alarm.h
# End Source File
# Begin Source File

SOURCE=.\CellRange.h
# End Source File
# Begin Source File

SOURCE=.\CommFuns.h
# End Source File
# Begin Source File

SOURCE=.\Container.h
# End Source File
# Begin Source File

SOURCE=.\Database.h
# End Source File
# Begin Source File

SOURCE=.\EDStock.h
# End Source File
# Begin Source File

SOURCE=.\EDStockDoc.h
# End Source File
# Begin Source File

SOURCE=.\EDStockView.h
# End Source File
# Begin Source File

SOURCE=.\Express.h
# End Source File
# Begin Source File

SOURCE=.\GraphView.h
# End Source File
# Begin Source File

SOURCE=.\GridCell.h
# End Source File
# Begin Source File

SOURCE=.\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=.\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=.\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\InPlaceList.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MathParser.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\Packets.h
# End Source File
# Begin Source File

SOURCE=.\Profile.h
# End Source File
# Begin Source File

SOURCE=.\QianLong.h
# End Source File
# Begin Source File

SOURCE=.\RealTime.h
# End Source File
# Begin Source File

SOURCE=.\RealTimeView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SCStkdrv.h
# End Source File
# Begin Source File

SOURCE=.\SelfDB.h
# End Source File
# Begin Source File

SOURCE=.\ShengLong.h
# End Source File
# Begin Source File

SOURCE=.\SListView.h
# End Source File
# Begin Source File

SOURCE=.\SpArchive.h
# End Source File
# Begin Source File

SOURCE=.\SpArchive.inl
# End Source File
# Begin Source File

SOURCE=.\SpAssert.h
# End Source File
# Begin Source File

SOURCE=.\SpColl.h
# End Source File
# Begin Source File

SOURCE=.\SpColl.inl
# End Source File
# Begin Source File

SOURCE=.\SpDebug.h
# End Source File
# Begin Source File

SOURCE=.\SpDefs.h
# End Source File
# Begin Source File

SOURCE=.\SpFile.h
# End Source File
# Begin Source File

SOURCE=.\SpLock.h
# End Source File
# Begin Source File

SOURCE=.\SpMemory.h
# End Source File
# Begin Source File

SOURCE=.\SpObject.h
# End Source File
# Begin Source File

SOURCE=.\SpObject.inl
# End Source File
# Begin Source File

SOURCE=.\SpObjErr.inl
# End Source File
# Begin Source File

SOURCE=.\SpPlat.h
# End Source File
# Begin Source File

SOURCE=.\SpString.h
# End Source File
# Begin Source File

SOURCE=.\SpString.inl
# End Source File
# Begin Source File

SOURCE=.\SpTime.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\STKDRV.h
# End Source File
# Begin Source File

SOURCE=.\StkReceiver.h
# End Source File
# Begin Source File

SOURCE=.\Stock.h
# End Source File
# Begin Source File

SOURCE=.\Stockdrv.h
# End Source File
# Begin Source File

SOURCE=.\StockGraph.h
# End Source File
# Begin Source File

SOURCE=.\Strategy.h
# End Source File
# Begin Source File

SOURCE=.\Strings.h
# End Source File
# Begin Source File

SOURCE=.\Technique.h
# End Source File
# Begin Source File

SOURCE=.\TechUser.h
# End Source File
# Begin Source File

SOURCE=.\TitleTip.h
# End Source File
# Begin Source File

SOURCE=.\Tongshi.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\EDStock.ico
# End Source File
# Begin Source File

SOURCE=.\res\EDStock.rc2
# End Source File
# Begin Source File

SOURCE=.\res\EDStockDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
