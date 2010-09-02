; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

[Setup]
AppName=FreeEIM
AppVerName=FreeEIM v 1.6
AppPublisher=GreatNet
AppPublisherURL=http://www.freeeim.com
AppSupportURL=http://www.freeeim.com
AppUpdatesURL=http://www.freeeim.com
DefaultDirName={pf}\FreeEIM
DefaultGroupName=FreeEIM
LicenseFile=D:\���� (4) skon01\Release\readme.txt
InfoBeforeFile=D:\���� (4) skon01\Release\readme.txt
InfoAfterFile=D:\���� (4) skon01\Release\readme.txt
OutputDir=C:\Documents and Settings\skon\����
OutputBaseFilename=setup
SetupIconFile=D:\���� (4) skon01\res\EIM02.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinese"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "D:\���� (4) skon01\Release\freeeim.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\���� (4) skon01\Release\locknt.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\���� (4) skon01\Release\fdelete.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\���� (4) skon01\Release\wininfo.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\���� (4) skon01\Release\notebook.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\���� (4) skon01\Release\LanSpy.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\���� (4) skon01\Release\panfu.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "D:\���� (4) skon01\Release\snow.exe"; DestDir: "{app}"; Flags: ignoreversion
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[INI]
Filename: "{app}\freeeim.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.freeeim.com"

[Icons]
Name: "{group}\FreeEIM"; Filename: "{app}\freeeim.exe"
Name: "{group}\{cm:ProgramOnTheWeb,FreeEIM}"; Filename: "{app}\freeeim.url"
Name: "{group}\{cm:UninstallProgram,FreeEIM}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\FreeEIM"; Filename: "{app}\freeeim.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\freeeim.exe"; Description: "{cm:LaunchProgram,FreeEIM}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\freeeim.url"

