; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

[Setup]
AppName=FreeEIM
AppVerName=FreeEIM 2.0
AppPublisher=FEIM Studios
AppPublisherURL=http://www.freeeim.com/
AppSupportURL=http://www.freeeim.com/
AppUpdatesURL=http://www.freeeim.com/
DefaultDirName={pf}\FreeEIM
DefaultGroupName=FreeEIM ��ͨר��
LicenseFile=C:\Documents and Settings\skon\����\setup\Readme.txt
OutputDir=C:\Documents and Settings\skon\����\setup
OutputBaseFilename=setup
SetupIconFile=C:\Documents and Settings\skon\����\20080410-FreeEIM\res\EIM02.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinese"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "C:\Documents and Settings\skon\����\setup\freeeim.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Documents and Settings\skon\����\setup\Readme.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "C:\Documents and Settings\skon\����\setup\eimseh.dll"; DestDir: "{app}"; Flags: ignoreversion
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[INI]
Filename: "{app}\freeeim.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://www.freeeim.com/"

[Icons]
Name: "{group}\FreeEIM"; Filename: "{app}\freeeim.exe"
Name: "{group}\{cm:ProgramOnTheWeb,FreeEIM}"; Filename: "{app}\freeeim.url"
Name: "{group}\{cm:UninstallProgram,FreeEIM}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\FreeEIM"; Filename: "{app}\freeeim.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\FreeEIM"; Filename: "{app}\freeeim.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\freeeim.exe"; Description: "{cm:LaunchProgram,FreeEIM}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\freeeim.url"

