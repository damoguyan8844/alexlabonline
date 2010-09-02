# Microsoft Developer Studio Project File - Name="EIM02" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=EIM02 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "EIM02.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "EIM02.mak" CFG="EIM02 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EIM02 - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "EIM02 - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "EIM02 - Win32 Release"

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
# ADD LINK32 /nologo /subsystem:windows /machine:I386 /out:"Release/freeeim.exe"

!ELSEIF  "$(CFG)" == "EIM02 - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
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
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /out:"Debug/freeeim.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "EIM02 - Win32 Release"
# Name "EIM02 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\EIM02.cpp
# End Source File
# Begin Source File

SOURCE=.\EIM02.rc
# End Source File
# Begin Source File

SOURCE=.\EIM02Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EIM_ViewHistory.cpp
# End Source File
# Begin Source File

SOURCE=.\EIMChatingWaiter.cpp
# End Source File
# Begin Source File

SOURCE=.\em\EM_FileReceiving.cpp
# End Source File
# Begin Source File

SOURCE=.\em\EM_FileSendingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\em\EM_GroupInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\EM_MsgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EM_RecvChat.cpp
# End Source File
# Begin Source File

SOURCE=.\EM_SendChat.cpp
# End Source File
# Begin Source File

SOURCE=.\em\EM_UserInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\em\EM_UserLogin.cpp
# End Source File
# Begin Source File

SOURCE=.\EM_UserProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\em\EM_UserTree.cpp
# End Source File
# Begin Source File

SOURCE=.\em\EMRichEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\freeeim_config.cpp
# End Source File
# Begin Source File

SOURCE=.\freeeim_history.cpp
# End Source File
# Begin Source File

SOURCE=.\freeeim_message_logic.cpp
# End Source File
# Begin Source File

SOURCE=.\freeeim_package.cpp
# End Source File
# Begin Source File

SOURCE=.\freeeim_user_logic.cpp
# End Source File
# Begin Source File

SOURCE=.\thirdpartclass\GetHardSoftInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageDataObject.cpp
# End Source File
# Begin Source File

SOURCE=.\thirdpartclass\INI.cpp
# End Source File
# Begin Source File

SOURCE=.\MSGReceiver.cpp
# End Source File
# Begin Source File

SOURCE=.\thirdpartclass\PictureEx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\em\stdemdef.cpp
# End Source File
# Begin Source File

SOURCE=.\thirdpartclass\SystemTray.cpp
# End Source File
# Begin Source File

SOURCE=.\UserSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveIn.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveOut.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\thirdpartclass\WinAppEx.cpp
# End Source File
# Begin Source File

SOURCE=.\XEIM_Database.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\EIM02.h
# End Source File
# Begin Source File

SOURCE=.\EIM02Dlg.h
# End Source File
# Begin Source File

SOURCE=.\EIM_ViewHistory.h
# End Source File
# Begin Source File

SOURCE=.\EIMChatingWaiter.h
# End Source File
# Begin Source File

SOURCE=.\em\EM_FileReceiving.h
# End Source File
# Begin Source File

SOURCE=.\em\EM_FileSendingDlg.h
# End Source File
# Begin Source File

SOURCE=.\em\EM_GroupInfo.h
# End Source File
# Begin Source File

SOURCE=.\EM_MsgDlg.h
# End Source File
# Begin Source File

SOURCE=.\EM_RecvChat.h
# End Source File
# Begin Source File

SOURCE=.\EM_SendChat.h
# End Source File
# Begin Source File

SOURCE=.\em\EM_UserInfo.h
# End Source File
# Begin Source File

SOURCE=.\em\EM_UserLogin.h
# End Source File
# Begin Source File

SOURCE=.\EM_UserProperty.h
# End Source File
# Begin Source File

SOURCE=.\em\EM_UserTree.h
# End Source File
# Begin Source File

SOURCE=.\em\EMRichEdit.h
# End Source File
# Begin Source File

SOURCE=.\freeeim_config.h
# End Source File
# Begin Source File

SOURCE=.\freeeim_history.h
# End Source File
# Begin Source File

SOURCE=.\freeeim_message_logic.h
# End Source File
# Begin Source File

SOURCE=.\freeeim_package.h
# End Source File
# Begin Source File

SOURCE=.\freeeim_user_logic.h
# End Source File
# Begin Source File

SOURCE=.\thirdpartclass\GetHardSoftInfo.h
# End Source File
# Begin Source File

SOURCE=.\ImageDataObject.h
# End Source File
# Begin Source File

SOURCE=.\thirdpartclass\INI.h
# End Source File
# Begin Source File

SOURCE=.\MSGReceiver.h
# End Source File
# Begin Source File

SOURCE=.\thirdpartclass\PictureEx.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\em\stdemdef.h
# End Source File
# Begin Source File

SOURCE=.\thirdpartclass\SystemTray.h
# End Source File
# Begin Source File

SOURCE=.\UserSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\WaveIn.h
# End Source File
# Begin Source File

SOURCE=.\WaveOut.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# Begin Source File

SOURCE=.\thirdpartclass\WinAppEx.h
# End Source File
# Begin Source File

SOURCE=.\XEIM_Database.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\chat.ico
# End Source File
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\edit_user.ico
# End Source File
# Begin Source File

SOURCE=.\res\EIM02.ico
# End Source File
# Begin Source File

SOURCE=.\res\EIM02.rc2
# End Source File
# Begin Source File

SOURCE=.\res\history.ico
# End Source File
# Begin Source File

SOURCE=.\res\i_animated_loading_32_2.gif
# End Source File
# Begin Source File

SOURCE=.\res\List.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MAINFRAME_CONTACT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MAINFRAME_LINE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\usericon.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section EIM02 : {D30C1661-CDAF-11D0-8A3E-00C04FC9E26E}
# 	2:5:Class:CWebBrowser2
# 	2:10:HeaderFile:webbrowser2.h
# 	2:8:ImplFile:webbrowser2.cpp
# End Section
# Section EIM02 : {8856F961-340A-11D0-A96B-00C04FD705A2}
# 	2:21:DefaultSinkHeaderFile:webbrowser2.h
# 	2:16:DefaultSinkClass:CWebBrowser2
# End Section
